#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>


#define BUF_SIZE 64



int read_toupper_write(int read_fd, int write_fd)
{
	errno = 0;
	char buf[BUF_SIZE] = {0};
	ssize_t count;
	
	while(count = read(read_fd, buff, BUF_SIZE)== -1)
	{
		if(errno == EINTR)
		{
			errno = 0;
			continue;
		}
		else
		{
			perror("Error in reading from pipe");
		}
		write(STDOUT_FILENO, 
	}
	return count;
}
void makeUpper(char* buff, size_t n)
{
	for(size_t i = 0; i < n; ++i)
		buff[i] = toupper(buff[i]);
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

int main()
{
	int p_filedes[2];
//	const char* msg = "Hello, world!\n";
//	int len = strlen(msg) + 1;

	if(pipe(p_filedes) == -1)
	{
		perror("Error in initializing pipe");
		return -1;
	}
	close(STDIN_FILENO);
	dup2(p_filedes[0], STDIN_FILENO);

	pid_t rpid = fork();
	if(rpid == -1)
	{
		perror("Error in fork");
		close_pipe(p_filedes);
		return -1;
	}
	else if(rpid == 0)
	{
		char buffer[MAXBUFF];
		int len = readFromPipe(p_filedes[1], buffer);
		makeUpper(buffer, len);

		if(write(1, buffer, len) != len)
		{
			perror("Error in stdout print in read child process");
			close_pipe(p_filedes);
			return -1;
		}
		close_pipe(p_filedes);
		return 0;
	}
	int rstat;

	while(wait(&rstat) == -1)
	{
		if(errno == EINTR)
		{
			errno = 0;
			continue;
		}
		perror("Error in read");
		close_pipe(p_filedes);

		return 1;
	}
	close_pipe(p_filedes);
	return 0;
}
