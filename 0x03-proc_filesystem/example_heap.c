/* printf fprtinf */
#include <stdio.h>
/* free malloc */
#include <stdlib.h>
/* strcpy strlen */
#include <string.h>
/* sleep getpid */
#include <unistd.h>
/* getpid (pid_t) */
#include <sys/types.h>
/* sigaction */
#include <signal.h>
/* bool */
#include <stdbool.h>


static bool breakLoop;

/**
 * interruptHandler - sets boolean to break out of infinite loop in main
 *
 * @signo: signal number; only SIGINT handled by this function
 */
static void interruptHandler(int signo)
{
	if (signo == SIGINT)
		breakLoop = true;
}


/**
 * main - entry point; takes first arg and allocates space on the heap for
 * that string, then prints string and address until SIGINT
 *
 * @argc: amount of command line args
 * @argv: array of command line args
 * Return: EXIT_SUCCESS (0) on success, EXIT_FAILURE (1) on failure
 */
int main(int argc, char **argv)
{
	char *str;
	int i;
	pid_t pid;
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = interruptHandler;
	sigaction(SIGINT, &act, NULL);

	if (argc < 2)
	{
		fprintf(stderr, "%s usage: %s <string to repeat>\n",
			argv[0], argv[0]);
		return (EXIT_FAILURE);
	}
	else
	{
		str = malloc(strlen(argv[1]) + 1);
		if (!str)
		{
			fprintf(stderr, "%s: malloc failure\n", argv[0]);
			return (EXIT_FAILURE);
		}
		strcpy(str, argv[1]);
	}

	pid = getpid();
	/* intentionally infinite loop to monitor alterations in heap */
	for (i = 1; !breakLoop; i++)
	{
		printf("[%i]\t\"%s\" @ %p\tpid: %i\n", i, str, (void *)str, pid);
		sleep(1);
	}

	free(str);

	return (EXIT_SUCCESS);
}
