#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <poll.h>

#include "vector.h"

#define BUFFER_SIZE 4
#define FILE_TIMEOUT_MS ((int)5000)

bool wait_for_input(int file_des, int timeout_ms)
{
	struct pollfd poll_file_des = {
		.fd = file_des,
		.events = POLLIN,
		.revents = 0,
	};
	return poll(&poll_file_des, 1, timeout_ms) == 1;
}

bool fill_table(vector_t* table, int file_des)
{
	int old_flags = fcntl(file_des, F_GETFL);
	// Set file_des in nonblock mode
	//
	fcntl(file_des, F_SETFL, old_flags | O_NONBLOCK);

	char buffer[BUFFER_SIZE + 1] = {0};
	off_t current_pos = 0;
	ssize_t read_count = 0;
	errno = 0;

	while((read_count = read(file_des, buffer, BUFFER_SIZE)) != 0)
	{
		// check errno for EINTR, and other valid errors
		if (read_count == -1)
		{
			// If read(...) set errno in EINTR, trying to read data from file again
			// in loop.
			//
			if (errno == EINTR)
			{
				errno = 0;
				continue;
			}
			// If read(...) set errno in EAGAIN, trying to read data from file
			// once again with timeout for TIMEOUT ms.
			//
			else if (errno == EAGAIN)
			{
				if (!wait_for_input(file_des, FILE_TIMEOUT_MS))
				{
					fcntl(file_des, F_SETFL, old_flags);
					perror("Timeout error, could not open file: ");
					return true;
				}
				errno = 0;
			}
			// If read(...) have other errors reading from file considered
			// unseccessful and function returns true;
			//
			else
			{
				perror("fill_table error, cannot read from file: ");
				fcntl(file_des, F_SETFL, old_flags);
				return true;
			}
		}
		buffer[read_count] = '\0'; // making terminated string
		char* n_pos = buffer;

		while ((n_pos = strchr(n_pos, '\n')) != NULL)
		{
			if (vector_push_back(table, current_pos + (n_pos - buffer)))
			{
				perror("fill_table error, cannot to add element to array: ");
				fcntl(file_des, F_SETFL, old_flags);
				return true;
			}
			++n_pos;
		}
		current_pos += read_count;
	}
	//Return old mode for file_des
	//
	fcntl(file_des, F_SETFL, old_flags);
	return false;
}

void print_line(int file_des, vector_t* table, size_t line_number)
{
	off_t line_pos = line_number == 0
			? 0
			: vector_get(table, line_number - 1) + 1;

	size_t line_length = vector_get(table, line_number) - line_pos + 1;
	char* line = calloc(line_length + 1, sizeof(char));

	if (line == NULL)
	{
		perror("Print line error: ");
		return;
	}
	lseek(file_des, line_pos, SEEK_SET);
	read(file_des, line, line_length);
	printf("%s", line);
	free(line);
}

bool line_manage(int file_des, vector_t* table, size_t line_num)
{
	if (line_num == 0)
		return true;

	else if (line_num <= table->size)
	{
		printf("%lu|", line_num);
		print_line(file_des, table, line_num - 1);
	}
	else
		printf("Incorrect line number. Try again:\n");

	return false;
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
	vector_t table;
	size_t line_num = 0;

	if (vector_init(&table) || fill_table(&table, file_des))
	{
		perror("Error, cannot create table\n");
		vector_destroy(&table);
		close(file_des);
		return EXIT_FAILURE;
	}
	printf("Input line number in file.\nInput 0 to exit:\n");

	for(;;)
	{
		scan_luint(&line_num);

		if (line_manage(file_des, &table, line_num))
			break;
	}
	close (file_des);
	vector_destroy(&table);
	return EXIT_SUCCESS;
}
