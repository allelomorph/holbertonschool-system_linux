#include "API_server.h"

/* strncmp strlen */
#include <string.h>


/**
 * isMethodImplemented - validates Method token of a HTTP Request-line
 *
 * @Method: name of HTTP method in request
 * Return: 1 if valid method on this server, 0 if not
 */
int isMethodImplemented(const char *Method)
{
	size_t i;
	char *valid_methods[] = {
		/* "OPTIONS", */
		"GET",
		"HEAD",
		"POST",
		/* "PUT", */
		"DELETE",
		/* "TRACE", */
		/* "CONNECT", */
		NULL
	};

	if (!Method)
		return (0);

	for (i = 0; valid_methods[i]; i++)
	{
		if (strncmp(valid_methods[i],
			    Method,
			    strlen(valid_methods[i]) + 1) == 0)
			return (1);
	}

	return (0);
}


/**
 * isPathValid - validates Request-URI token of a HTTP Request-line
 *
 * @Request_URI: URI in request
 * Return: 1 if valid path on this server, 0 if not
 */
int isPathValid(const char *Request_URI)
{
	size_t i;
	char *valid_paths[] = {
		"/todos", /* tasks 8-11 API use */
		NULL
	};

	if (!Request_URI)
		return (0);

	for (i = 0; valid_paths[i]; i++)
	{
		if (strncmp(valid_paths[i],
			    Request_URI,
			    strlen(valid_paths[i]) + 1) == 0)
			return (1);
	}

	return (0);
}
