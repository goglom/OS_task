#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdlib.h>

#define BUF_SIZE 64

void make_upper(char* buff, size_t n)
{
	for(size_t i = 0; i < n; ++i)
		buff[i] = toupper(buff[i]);
}
void void_editor(char* a, size_t b)
{
}
int redirect(int read_fd, int write_fd, void (*editor)(char*, size_t))
{
	errno = 0;
	char buf[BUF_SIZE] = {0};
	ssize_t count = 0;

	while ((count = read(read_fd, buf, BUF_SIZE)) == -1)
	{
		if(errno == EINTR)
		{
			errno = 0;
			continue;
		}
		perror("Error in reading from pipe");
		return -1;
	}
	editor(buf, count);

	if (write(write_fd, buf, count) == -1)
	{
		perror("redirect() - error while writing to write_fd");
		return -2;
	}
	return 0;
}
int close_pipe(int pfildes[2])
{
	errno = 0;
	if(close(pfildes[0]) || close(pfildes[1]) == -1)
	{
		perror("Error in closing pipe");
		return -1;
	}
	return 0;
}
int wait_for_child()
{
	while(wait(&ch_stat) == -1)
	{
		if(errno == EINTR)
		{
			errno = 0;
			continue;
		}
		return -1;
	}
	retunr 0;
}

int main()
{
	int p_filedes[2];

	if(pipe(p_filedes) == -1)
	{
		perror("Error in initializing pipe");
		return -1;
	}
	pid_t rpid = fork();
	
	if(rpid == -1)
	{
		perror("Error in fork");
		close_pipe(p_filedes);
		return -1;
	}
	//	*	Child branch
	//
	else if(rpid == 0)
	{
		return redirect(p_filedes[1], STDOUT_FILENO, make_upper)
			? EXIT_FAILURE
			: EXIT_SUCCESS;
	}
	//
	//	* Parent branch
	//
	if (redirect(STDIN_FILENO, p_filedes[0], void_editor))
	{
		close_pipe(p_filedes);
		return EXIT_FAILURE;
	}
	int ch_stat;

	if (wait_for_child())
	{
		perror("Error while waitin child process");
		close_pipe(p_filedes);
		return EXIT_FAILURE;
	}
	close_pipe(p_filedes);
	return EXIT_SUCCESS;
}
