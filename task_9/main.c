#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>

int wait_for_child_proc(pid_t pid)
{
	int child_status = 0;

	do
	{
		pid_t wait_res = waitpid(pid, &child_status, 0);

		if (wait_res == -1)
		{
			perror("Error while waiting for child process: ");
			return -1;
		}
	} while (!WIFEXITED(child_status) && !WIFSIGNALED(child_status));

	return 0;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Error: invalid argument\nUsage: %s filename\n",
			argv[0]
		);
		return EXIT_FAILURE;
	}
	pid_t child_pid = fork();

	if (child_pid == -1)
	{
		perror("Error while forking: ");
		return EXIT_FAILURE;
	}
	// Child branch
	if (child_pid == 0)
	{
		if (execlp("cat", "cat", argv[1], (char*)NULL) == -1)
		{
			perror("Error while executing /bin/cat: ");
			return EXIT_FAILURE;
		}
	}
	// Parent branch
	else
	{
		printf("1) Parent print some text\n");
		printf("2) Hello world\n");
		printf("3) Hellow world, again...\n");

		if (wait_for_child_proc(child_pid))
		{
			return EXIT_FAILURE;
		}
		printf("4) Last line printed by parent process\n");
	}
	return EXIT_SUCCESS;
}
