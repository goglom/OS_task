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
		if (pclose(input) == -1)
		{
			perror("Error: pclose()");
		}
		return EXIT_FAILURE;
	}
	int status = pclose(input);
    
	if (status == -1)
    {
    	perror("pclose () error");
    	return EXIT_FAILURE;
    }
    else if (WIFEXITED(status))
    {
	int exit_stat = WEXITSTATUS(status);
	if (exit_stat ! = 0)
	{
    		printf("Child exit with non null exit code: %d\n", WEXITSTATUS(status));
		return EXIT_FAILURE;
	}
    }
    else if (WIFSIGNALED(status))
    {
		printf("Child closed by signal: %d\n", WTERMSIG(status));
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
