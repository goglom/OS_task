#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <ulimit.h>
#include <errno.h>

void print_user_and_group_id(void)
{
	printf("Real user id:       %u\n", getuid());
	printf("Effective user id:  %u\n", geteuid());
	printf("Real group id:	    %u\n", getgid());	
	printf("Effective group id: %u\n", getegid());
}

void print_process_parrent_group_id(void)
{
	errno = 0;

	pid_t process_id = getpid();
	pid_t parent_process_id = getppid();
	pid_t process_group_id = getpgrp();

	if (errno != 0)
	{
		perror("Error while get id: ");
		return;
	}
	printf("Current process id:          %u\n", process_id);
	printf("Parent's process id:         %u\n", parent_process_id);
	printf("Group of current process id: %u\n", process_group_id);
}

void make_process_leader(void)
{
	errno = 0;

	pid_t cur_pid = getpid();
	pid_t cur_group_id = getpgrp();	

	if (errno != 0)
	{
		perror("Error while get id: ");
		return;
	}
	int result = setpgid(cur_pid, cur_group_id);

	if (result == 0)
	{
		printf("Pocess successful become leader of the group\n");
	}
	else
	{
		perror("Error while set process group id\n");
	}
}

void print_ulimit(void)
{
	struct rlimit limit;
	int result = getrlimit(RLIMIT_FSIZE, &limit);

	if (result != 0)
	{
		perror("Failed to get file size  limits: ");
		return;
	}
	printf(
			"Ulimits (in bytes):\n"
			"-soft limit: %ju\n"
			"-hard limit: %ju\n",
			limit.rlim_cur, 
			limit.rlim_max
	);
}

void set_ulimit(char* arg)
{
	errno = 0;
	
	long ulim = atol(arg);

	if (ulim == 0)
	{
		printf("Error invalid argument for ulimit\n");
		return;
	}
	struct rlimit new_limit = { 
							.rlim_cur = ulim, 
							.rlim_max = ulim
	};
	int result = setrlimit(RLIMIT_FSIZE, &new_limit);

	if (result == 0)
	{
		printf("Soft and hard ulimits set at: %ld\n", ulim);
	}
	else
	{
		perror("Error while set new ulimit: ");
	}
}

void print_all_env_var(char** envp)
{
	for (size_t i = 0; envp[i] != NULL; ++i)
	{
		printf("%s\n", envp[i]);
	}
}

void set_or_add_env_val(char* new_val)
{
	int result = putenv(new_val);

	if (result == 0)
	{
		printf("Successfully set: %s\n", new_val);
	}
	else
	{
		perror("Setting or adding environ error: ");
	}
}

void print_path()
{
	char const* working_dir_name = getenv("PWD");

	if (working_dir_name != NULL)
	{
		printf("Current working directory name:\n%s\n", working_dir_name);
	}
	else
	{
		printf("Error: couldn't  get current working directory name\n");
	}
}

void set_core_file_limits(char* arg)
{
	errno = 0;
	long clim = atol(arg);

	if (clim == 0)
	{
		printf("Error invalid argument for core-file size limit\n");
		return;
	}
	struct rlimit new_limit = {
		.rlim_cur = clim,
		.rlim_max = clim
	};
	int result = setrlimit(RLIMIT_CORE, &new_limit);

	if (result == 0)
	{
		printf("Soft and hard core-file limits set at: %ld\n", clim);
	}
	else
	{
		perror("Error while set new core-file size limit: ");
	}
}

void print_core_file_limits()
{
	struct rlimit limit;
	int result = getrlimit(RLIMIT_CORE, &limit);

	if (result != 0)
	{
		perror("Failed to get core-file limits: ");
		return;
	}
	printf(
			"Core-file limits (in bytes):\n"
			"-soft limit: %lu\n"
			"-hard limit: %lu\n",
			limit.rlim_cur, 
			limit.rlim_max
	);
}

int main(int argc, char** argv, char** envp)
{
	if (argc == 1)
	{
		printf("No arguments passed\n");
		return EXIT_SUCCESS;
	}	

	int c;	
	while(optind < argc)
	{
		if ((c = getopt(argc, argv, "ispuU:vV:dcC:")) != -1)
		{
			switch(c)
			{
			case 'i':
				print_user_and_group_id();
				break;
			case 's':
				make_process_leader();
				break;
			case 'p':
				print_process_parrent_group_id();
				break;
			case 'u':
				print_ulimit();
				break;
			case 'U':
				set_ulimit(optarg);
				break;
			case 'v':
				print_all_env_var(envp);
				break;
			case 'V':
				set_or_add_env_val(optarg);
				break;
			case 'd':
				print_path();
				break;
			case 'c':
				print_core_file_limits();
				break;
			case 'C':
				set_core_file_limits(optarg);
				break;
			case ':':
				fprintf(stderr, "-%c without operand\n", optopt);
				break;
			case '?':
				fprintf(stderr, "Unrecognized option: -%c\n", optopt);
			default:
				printf("Error while parsing arguments\n");
				return EXIT_FAILURE;
			}
		}
		else
		{
			printf("Key(s) was(where) not passed\n");
			break;	
		}	
	}

	return EXIT_SUCCESS;
}
