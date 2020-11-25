#include <poll.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	struct pollfd stdin_fdes = {
		.fd = 0, //stdin file descriptor
		.events = POLLIN,
		.revents = 0
	};

	int timeout = 5000; // value in ms
	int poll_res = poll(&stdin_fdes, 1, timeout);

	if (poll_res == 1)
	{
		int a = 0;
		scanf("%d", &a);
		printf("%d, Your input: %d\n", stdin_fdes.revents, a);
	}
	else if (poll_res == 0)
	{
		printf("Input timeout\n");
	}
	else
	{
		perror("Error while poll stdin: ");
	}
	return 0;
}
