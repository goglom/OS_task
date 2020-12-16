#include <signal.h>
#include <stdio.h>

typedef enum _events
{
	beep,
	quit,
	none,
} events;

events event = none;

void handler(int sig)
{
	if (sig == SIGINT)
	{
		event = beep;
	}
	else if (sig == SIGQUIT)
	{
		event = quit;
	}
}

int main()
{
	if (sigset(SIGINT, handler) == SIG_ERR)
	{
		perror("sigset() - error, couldn't set signal handler to SIGINT");
	}
	if (sigset(SIGQUIT, handler) == SIG_ERR)
	{
		perror("sigset() - error, couldn't set signal handler to SIGQUIT");
	}
	size_t beep_count = 0;

	while(1)
	{
		if (event == beep)
		{
			event = none;
			++beep_count;
			printf("BEEP\n\a");
		}
		else if (event == quit)
		{
			event = none;
			printf("Count of beeps: %d\nexit...\n", beep_count);
			return 0;
		}
	}
	return 0;
}
