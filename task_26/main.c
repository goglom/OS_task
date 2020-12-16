#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define BUFFER_SIZE 100

void make_upper(char* buff, size_t n)
{
	for(size_t i = 0; i < n; ++i)
		buff[i] = toupper(buff[i]);
}

int main()
{
    FILE* input = popen("echo < input.txt", "r");
    if (f == NULL) 
	{
        perror("popen() error");
        return EXIT_FAILURE;
    }

    char buf[BUFFER_SIZE];
    size_t read_count = fread(buf, sizeof(char), BUFFER_SIZE, input);

    if (ferror(f) == -1) 
	{
        perror("fread() error");
        pclose(f);
        return EXIT_FAILURE;
    }
    if (pclose(f) == -1) 
	{	
        perror("pclose() error");
        return EXIT_FAILURE;
    }
    make_upper(buf, read_count);
    fwrite(buf, read_count, 1, stdout);
	
    if (ferror(f) == -1) 
	{
        perror("fwrite() error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
