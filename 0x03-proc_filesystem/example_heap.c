/* printf fprtinf */
#include <stdio.h>
/* free malloc */
#include <stdlib.h>
/* strcpy strlen */
#include <string.h>
/* sleep */
#include <unistd.h>
/* sigaction */
#include <signal.h>
#include <stdbool.h>


static bool breakLoop = false;

static void interruptHandler(int signo)
{
	if (signo == SIGINT)
		breakLoop = true;
}

int main(int argc, char **argv)
{
	char *str;
	int i;
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = interruptHandler;
	sigaction(SIGINT, &act, NULL);

	if (argc < 2)
	{
		fprintf(stderr, "%s usage: %s <string to repeat>\n",
			argv[0], argv[0]);
		return (1);
	}
	else
	{
		str = malloc(strlen(argv[1]) + 1);
		if (!str)
		{
			fprintf(stderr, "%s: malloc failure\n", argv[0]);
			return (1);
		}
		strcpy(str, argv[1]);
	}

	/* intentionally infinite loop to monitor alterations in heap */
	for (i = 1; !breakLoop; i++)
	{
		printf("[%2d] \"%s\" @ %p\n", i, str, (void *)str);
		sleep(1);
	}

	free(str);

	return (0);
}
