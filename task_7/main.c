#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <stdlib.h>
#include "vector.h"

#define TIMEOUT ((int)5000)
#define FILE_TIMEOUT_MS ((int)5000)

int wait_for_input(int file_des, int timeout_ms)
{
	struct pollfd poll_file_des = {
		.fd = file_des,
		.events = POLLIN,
		.revents = 0
	};
	return poll(&poll_file_des, 1, timeout_ms);
}

void* map_file_to_buffer(int file_des, size_t* buffer_size)
{
	struct stat file_stat = {0};
	void* file_buffer = NULL;

	if (fstat(file_des, &file_stat) != 0)
	{
		perror("Error while filling the table: ");
		return NULL;
	}
	*buffer_size = file_stat.st_size;
	file_buffer = mmap((caddr_t) 0, *buffer_size, PROT_READ, MAP_PRIVATE, file_des, 0);

	if (file_buffer == MAP_FAILED)
	{
		perror("Error while mapping file: ");
	}
	return file_buffer;
}

int fill_table(vector_t* table, char const* file_buffer)
{
	char const* n_pos = file_buffer;

	while ((n_pos = strchr(n_pos, '\n')) != NULL)
	{
		if (vector_push_back(table, n_pos - file_buffer))
		{
			perror("fill_table error, cannot to add element to array: ");
			return -1;
		}
		++n_pos;
	}
	return 0;
}

void print_line(vector_t* table, char const* file_buffer, size_t line_number)
{
    size_t line_pos = line_number == 0
            ? 0
            : vector_get(table, line_number - 1) + 1;

    size_t line_length = vector_get(table, line_number) - line_pos + 1;
	file_buffer += line_pos;

	for (size_t i =0; i < line_length; ++i)
	{
		fputc(file_buffer[i], stdout);
	}
}

int line_manage(vector_t* table, char const* file_buffer, size_t line_num)
{
	if (line_num == 0)
	{
		return -1;
	}
	else if (line_num <= table->size)
	{
		printf("%lu|", line_num);
		print_line(table, file_buffer, line_num - 1);
	}
	else
	{
		printf("Incorrect line number. Try again:\n");
	}
	return 0;
}

void scan_luint(size_t* value)
{
	while(scanf("%lu", value) == 0)
	{
		printf("Incorrect input. Try to input integer value:\n");
		fflush(stdin);
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Invalid argument\nUsage: prog [filename]\n");
		return EXIT_FAILURE;
	}
	int file_des = open(argv[1], O_RDONLY);

	if (file_des == -1)
	{
		perror("Cannot open input file\n");
		return EXIT_FAILURE;
	}
	size_t line_num = 0;
	vector_t table;
	size_t buffer_size = 0;
	char* file_buffer = map_file_to_buffer(file_des, &buffer_size);

	if (file_buffer == NULL)
	{
		return EXIT_FAILURE;
	}
	close(file_des);

	if (vector_init(&table) || fill_table(&table, file_buffer))
	{
		perror("Error, cannot create table\n");
		vector_destroy(&table);
		return EXIT_FAILURE;
	}
	printf(
			"Input line number in file.\n"
			"After %d seconds program will print all file and close.\n"
			"Input 0 to exit:\n",
			TIMEOUT / 1000
	);
	while(1)
	{
		int result =  wait_for_input(0 /*stdin*/, TIMEOUT);
		if (result == 1)
		{
			scan_luint(&line_num);
			if (line_manage(&table, file_buffer, line_num))
				break;
		}
		else if (result == 0)
		{
			printf("Input timeout\n");
			for (size_t i = 1; i <= table.size; ++i)
			{
				line_manage(&table, file_buffer, i);
			}
			break;
		}
		else
		{
			perror("Error while wait for input from stdin: ");
			break;
		}
	}
	munmap(file_buffer, buffer_size);
	vector_destroy(&table);
	return EXIT_SUCCESS;
}
