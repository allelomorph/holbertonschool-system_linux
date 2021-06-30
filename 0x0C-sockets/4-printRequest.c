#include "API_server.h"

/* printf */
#include <stdio.h>


/**
 * printRequest - prints Request-Line fields of a vaildated HTTP request
 *
 * @request: pointer to struct populated with processed request contents
 */
void printRequest(HTTP_request_t *request)
{
	if (!request)
		return;

	printf("Method: %s\nPath: %s\nVersion: %s\n",
	       request->Method, request->Request_URI,
	       request->HTTP_Version);

	HTTP_response(200, NULL, NULL);
}
