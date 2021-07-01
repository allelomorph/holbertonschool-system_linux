#include "API_server.h"
/* perror */
#include <stdio.h>
/* close */
#include <unistd.h>
/* exit EXIT_FAILURE EXIT_SUCCESS */
#include <stdlib.h>
/* sigaction */
#include <signal.h>
/* memset */
#include <string.h>


/**
 * SIGINT_SIGTERM_handler - closes any open socket fds in the event of a
 *   termination or interrupt
 *
 * @signum: code number of incoming signal
 */
void SIGINT_SIGTERM_handler(int signum)
{
	/* signum required by expected handler prototype */
	(void)signum;

	/* tries to complete transmissions first, otherwise use shutdown(2) */
	if (server_fd != -1)
	{
		if (close(server_fd) != 0)
		{
			perror("SIGINT_SIGTERM_handler: close");
			exit(EXIT_FAILURE);
		}
		server_fd = -1;
	}

	if (client_fd != -1)
	{
		if (close(client_fd) != 0)
		{
			perror("SIGINT_SIGTERM_handler: close");
			exit(EXIT_FAILURE);
		}
		client_fd = -1;
	}
}


/**
 * setSigHandler - sets signal handler for SIGINT and SIGTERM
 */
void setSigHandler(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));

	sa.sa_handler = SIGINT_SIGTERM_handler;

	if (sigaction(SIGINT, &sa, NULL) == -1 ||
	    sigaction(SIGTERM, &sa, NULL) == -1)
		errorExit("setSigHandler: sigaction");
}
