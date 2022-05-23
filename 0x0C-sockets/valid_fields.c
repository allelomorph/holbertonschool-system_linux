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
#if SRC_VERSION != 8
		"GET",
		"HEAD",
#endif
		"POST",
		/* "PUT", */
#if SRC_VERSION == 11
		"DELETE",
#endif
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
#if SRC_VERSION >= 8
	size_t i;
	char *valid_paths[] = {
		"/todos", /* tasks 8-11 API use */
		NULL
	};
#endif

	if (!Request_URI)
		return (0);

#if SRC_VERSION <= 7
	if (strlen(Request_URI) &&
	    Request_URI[0] == '/')
		return (1);
#else
	for (i = 0; valid_paths[i]; i++)
	{
		if (strncmp(valid_paths[i],
			    Request_URI,
			    strlen(valid_paths[i]) + 1) == 0)
			return (1);
	}
#endif
	return (0);
}
