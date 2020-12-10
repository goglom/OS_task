#include <fcntl.h>	// fcntl(...)
#include <stdlib.h>	// system(...)
#include <string.h> // memset(...)
#include <stdio.h>	// printf(...)

#define __FILE_NAME "input.txt"

void setup_lock(struct flock* lock, short lock_type)
{
	memset(lock, 0, sizeof(struct flock));
	lock->l_type = lock_type;
	lock->l_start = 0;
	lock->l_whence = SEEK_SET;
	lock->l_len = 0;
}

int main()
{
	int fd = open(__FILE_NAME, O_WRONLY);
	if (fd == -1)
	{
		perror("Error while opening file");
		return EXIT_FAILURE;
	}
	struct flock lock;
	/*
		* FWRLCK - lock file for writing
	*/
	setup_lock(&lock, F_WRLCK);
	/*
		* SETLKW - set lock, if file not accesible waits
	*/
	fcntl(fd, F_SETLKW, &lock);
	printf("File locked\n");

	system("nano " __FILE_NAME);

	lock.l_type = F_UNLCK;
	setup_lock(&lock, F_WRLCK);
	printf("File unlocked\n");

	return EXIT_SUCCESS;
}
