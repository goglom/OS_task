#include <unistd.h>	// fork(...), exec - family
#include <wait.h>	// wait(...)
#include <stdlib.h> // EXIT_... marcoses 
#include <stdio.h>

int wait_for_child_proc(pid_t pid)
{
    int child_status = 0;

    do
    {
        pid_t wait_res = waitpid(pid, &child_status, 0);

        if (wait_res == -1)
        {
            perror("Error while waiting for child process");
            return -1;
        }
		if (WIFEXITED(child_status))
		{
			printf("Child procees exited wiht code: %d\n", WEXITSTATUS(child_status));
		}
		else if(WIFSIGNALED(child_status))
		{
			printf("Child process killed by signal: %d\n", WTERMSIG(child_status));
		}

    } while (!WIFEXITED(child_status) && !WIFSIGNALED(child_status));

    return 0;
}

int main(int argc, char** argv)
{
	pid_t child_pid = fork();

	// Child branch
	if (child_pid == 0)
	{
		if (execvp(argv[1], argv + 1) == -1)
		{
			perror("Error while trying to execute child process");
			return EXIT_FAILURE;
		}
	}
	else
	{
		if (wait_for_child_proc(child_pid))
		{
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
