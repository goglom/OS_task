#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

extern char *tzname[];

void main()
{
	if (putenv("TZ=America/Los_Angeles") != 0)
	{
		printf("Error, couldn't set California time zone\n");
		exit(1);
	}

	time_t now;
	time( &now );

	printf("%s", ctime( &now ) );

     	exit(0);
}
