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
		"/test", /* project tasks 4-7 */
		"/holberton", /* project tasks 4-7 */
		"/todo", /* tasks 8-11 API use */
		"/", /* checker script */
		"/holberton_school", /* checker script */
		"/holberton/school", /* checker script */
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


/**
 * isVersionSupported - validates HTTP_Version token of a HTTP Request-line
 *
 * @HTTP_Version: protocol version in request
 * Return: 1 if version supported by this server, 0 if not
 */
int isVersionSupported(const char *HTTP_Version)
{
	size_t i;
	char *supported_vers[] = {
		"HTTP/1.1",
		NULL
	};

	if (!HTTP_Version)
		return (0);

	for (i = 0; supported_vers[i]; i++)
	{
		if (strncmp(supported_vers[i],
			    HTTP_Version,
			    strlen(supported_vers[i]) + 1) == 0)
			return (1);
	}

	return (0);
}


/**
 * parseRequestLine - validates and parses a HTTP Request-line to populate a
 *   HTTP_Request struct
 *
 * @recv_str: buffer of raw request received from client
 * @message_lines: array of pointers to recv_str after being tokenized by CRLF
 * @request: struct to populate with processed values, modified by reference
 * Return: 1 on failure, 0 on success
 */
int parseRequestLine(char *recv_str, char **message_lines,
		     HTTP_request_t *request)
{
	if (!request || !message_lines || !recv_str)
	{
		HTTP_response(500, NULL, NULL);
		return (1);
	}

	request->Method = strtok(message_lines[0], " ");
	request->Request_URI = strtok(NULL, " ");
	request->HTTP_Version = strtok(NULL, " ");
	/* validate expected HTTP/1.1 syntax of 3 SP-delimited tokens */
	if (!request->Method || !strlen(request->Method) ||
	    !request->Request_URI || !strlen(request->Request_URI) ||
	    !request->HTTP_Version || !strlen(request->HTTP_Version))
	{
		HTTP_response(400, NULL, NULL);
		return (1);
	}
	/* RFC 2616 recommends URIs <= 255 chars for legacy compliance */
	if (strlen(request->Request_URI) > 255)
	{
		HTTP_response(414, NULL, NULL);
		return (1);
	}

	request->Request_URI = strtok(request->Request_URI, "?");
	request->URI_query = strtok(NULL, "?");

	if (!isMethodImplemented(request->Method) ||
	    !isPathValid(request->Request_URI))
	{
		HTTP_response(404, NULL, NULL);
		return (1);
	}
	if (!isVersionSupported(request->HTTP_Version))
	{
		HTTP_response(505, NULL, NULL);
		return (1);
	}

	return (0);
}
