#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int wait_for_child_proc(pid_t pid)
{
	int child_status = 0;

	do
	{
		errno = 0;
		pid_t wait_res = waitpid(pid, &child_status, 0);

		if (wait_res == -1)
		{
			if (errno == EINTR)
			{
				continue;
			}
			perror("Error while waiting for child process: ");
			return -1;
		}
		if (WIFSIGNALED(child_status))
		{
			printf("Child process killed by signal\n");
		}
		else if (WIFSTOPPED(child_status))
		{
			printf("Clid process was stopped by signal: %d\n",
										 WSTOPSIG(child_status));
		}
		else if (WIFCONTINUED(child_status))
		{
			printf("Child procees was continued\n");
		}
		else if (WIFEXITED(child_status))
		{
			printf("Child process exited with code: %d\n", 
								WEXITSTATUS(child_status));
		}
	} while (!WIFEXITED(child_status) && !WIFSIGNALED(child_status));

	return 0;
}

int main(int argc, char** argv)
{
//	if (argc < 2)
//	{
//		printf("Error: invalid argument\nUsage: %s filename\n",
//			argv[0]
//		);
//		return EXIT_FAILURE;
//	}
	pid_t child_pid = fork();

	if (child_pid == -1)
	{
		perror("Error while forking");
		return EXIT_FAILURE;
	}
	// Child branch
	if (child_pid == 0)
	{
		//execlp("cat", "cat", argv[1], (char*)NULL);
		printf("Child process created:\t%d\n", getpid());
		sleep(10);
		printf("Child process wants to exit\n");
		return EXIT_SUCCESS;
	}
	// Parent branch
	//printf("Child's pid: %d\n", child_pid);
	//printf("1) Parent print some text\n");
	//printf("2) Hello world\n");
	//printf("3) Hellow world, again...\n");

	//if (wait_for_child_proc(child_pid))
	//{
	//	return EXIT_FAILURE;
	//}
	//printf("4) Last line printed by parent process\n");

	while(1)
		sleep(1);

	return EXIT_SUCCESS;
}
