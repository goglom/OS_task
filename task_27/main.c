#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define BUFFER_SIZE 100


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

    	if (pclose(input) == -1)
    	{
			perror("pclose () error");
		}
        return EXIT_FAILURE;
    }
    if (pclose(input) == -1)
    {
    	perror("pclose () error");
    	return EXIT_FAILURE;
    }
	printf("Number of blank lines: %u\n", (unsigned)strtol(num_of_lines, NULL, 10));

	return EXIT_SUCCESS;
}
