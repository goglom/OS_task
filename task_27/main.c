#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <wait.h>

#define BUFFER_SIZE 100

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
		if (exit_status != 0)
		{
			printf("Child exited wtih status: %d\n", exit_status);
			return -1;
		}
	}
	else if (WIFSIGNALED(status))
	{
		printf("killed by signal\n", WTERMSIG(status));
		return -1;
	}
	else if (WIFSTOPPED(status))
	{
		printf("stopped by signal: %d\n", WSTOPSIG(status));
		return -1;
	}
	else if (WIFCONTINUED(status))
	{
		printf("continued\n");
	}
	return 0;
}

int main(int argc, char* argv[])
{
	FILE* input = popen("grep -E '^( )*$' main.c | wc -l", "r");
	
	if (input == NULL)
	{
		perror("popen() error");
		return EXIT_FAILURE;
	}
	char buf[BUFFER_SIZE];
	char* num_of_lines = fgets(buf, BUFFER_SIZE, input);

	if (num_of_lines == NULL && !feof(input))
	{
		perror("fgets() error");
	}
    	if (my_pclose(input))
	{
		return EXIT_FAILURE;
	}
	printf("Number of blank lines: %u\n", (unsigned)strtol(num_of_lines, NULL, 10));

	return EXIT_SUCCESS;
}
