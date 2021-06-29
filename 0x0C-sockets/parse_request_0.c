#include "API_server.h"

/* strlen */
#include <string.h>
/* printf */
#include <stdio.h>
/* free */
#include <stdlib.h>


/**
 * parseHTTPRequest - parses raw HTTP request to populate request data struct
 *
 * @recv_str: raw request string recieved from client
 * Return: pointer to populated HTTP_request_s, or NULL on failure
 */
HTTP_request_t *parseHTTPRequest(char *recv_str)
{
	char **message_lines;
	size_t message_line_ct;
	HTTP_request_t *request = NULL;

	request = malloc(sizeof(HTTP_request_t));
	if (!request || !recv_str)
	{
		HTTP_response(500, NULL, NULL);
		return (NULL);
	}
	while (*recv_str == '\r' && *(recv_str + 1) == '\n')
		recv_str += 2; /* ignore leading CRLFs */
	message_lines = tokenizeBySubstr(recv_str, "\r\n", &message_line_ct);
	if (!message_lines)
	{
		HTTP_response(500, NULL, NULL);
		return (NULL);
	}
	if (message_line_ct < 2 || !strlen(message_lines[0]) ||
	    strlen(message_lines[message_line_ct - 2]) ||
	    countTokensBySubstr(message_lines[0], " ") != 3)
	{
		HTTP_response(400, NULL, NULL);
		free(message_lines);
		return (NULL);
	}
	request->Method = strtokSubstr(message_lines[0], " ");
	request->Request_URI = strtokSubstr(NULL, " ");
	request->HTTP_Version = strtokSubstr(NULL, " ");
	if (!request->Method || !strlen(request->Method) ||
	    !request->Request_URI || !strlen(request->Request_URI) ||
	    !request->HTTP_Version || !strlen(request->HTTP_Version))
	{
		HTTP_response(400, NULL, NULL);
		free(message_lines);
		return (NULL);
	}
	free(message_lines);
	return (request);
}
