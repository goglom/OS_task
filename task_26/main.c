#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <wait.h>

#define BUFFER_SIZE 100

void make_upper(char* buff, size_t n)
{
	for(size_t i = 0; i < n; ++i)
		buff[i] = toupper(buff[i]);
}

int my_pclose(FILE* pipe)
{
	int status = pclose(pipe);
	if (status == -1)
	{
		perror("pclose () error");
		return -1;
	}
	if (WIFEXITED(status))
	{
		int exit_status = WEXITSTATUS(status);
		printf("exited, status=%d:\tchild_pid=%d\n\n", exit_status, pid);
		if (exit_status != 0)
		{
			return -1;
		}
	}
	else if (WIFSIGNALED(status))
	{
		printf("killed by signal %d:\tchild_pid=%d\n\n", WTERMSIG(status), pid);
		return -1;
	}
	else if (WIFSTOPPED(status))
	{
		printf("stopped by signal %d:\tchild_pid=%d\n\n", WSTOPSIG(status), pid);
		return -1;
	}
	else if (WIFCONTINUED(status))
	{
		printf("continued\n\n");
	}
	return 0;
}
	
int main()
{
	FILE* input = popen("cat input.txt", "r");
	
	if (input == NULL)
	{
		perror("Error: popen()");
		return EXIT_FAILURE;
	}
   	char buf[BUFFER_SIZE];
	size_t read_count = fread(buf, sizeof(char), BUFFER_SIZE, input);
	
	if (ferror(input) == -1)
	{
		perror("Error: fread() from out end of pipe");
		my_pclose(input);
		return EXIT_FAILURE;
	}
	if (my_pclose(input))
	{
		return EXIT_FAILURE;
	}
	make_upper(buf, read_count);
	fwrite(buf, read_count, 1, stdout);
	
	if (ferror(input) == -1)
	{
		perror("Error: fwrite() in stdout");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
