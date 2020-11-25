#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void print_user_id()
{
	printf(
		"\tUser id: %lu\n"
		"\tEffective user id: %lu\n",
		(unsigned long int)getuid(),
		(unsigned long int)geteuid()
	);
}

void try_open_and_close_file(const char* filename)
{
	FILE* fp = fopen(filename, "r");

	if (fp == NULL)
	{
		perror("\tError couldn't open file: ");
		return;
	}

	printf("\tFile '%s' successful open\n", filename);
	fclose(fp);
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf(
			"Incorect arguments count.\n"
			"Usage: user_id [file]\n"
			"file - opening file\n"
		);
		return EXIT_FAILURE;
	}

	//---------------- First try ---------------------//
	printf("First try: \n");
	print_user_id();
	try_open_and_close_file(argv[1]);
	printf("\n");
	//------------------------------------------------//

	if (seteuid(getuid()) == -1)
	{
		printf("User effective id not changed\n");
		return EXIT_FAILURE;
	}
	printf("Effective user id now is equal to real user id\n\n");

	//----------------- Second try -------------------//
	printf("Second try: \n");
	print_user_id();
	try_open_and_close_file(argv[1]);
	//------------------------------------------------//

	return EXIT_SUCCESS;
}
