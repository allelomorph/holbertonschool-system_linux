#include "API_server.h"

/* strlen */
#include <string.h>
/* printf */
#include <stdio.h>
/* free */
#include <stdlib.h>


/**
 * freeRequest - frees a parsed request
 *
 * @request: pointer to struct containing parsed data from client HTTP request
 */
void freeRequest(HTTP_request_t *request)
{
	HTTP_header_t *temp, *curr;

	if (!request || !request->headers)
		return;

	for (temp = request->headers; temp;)
	{
		curr = temp;
		temp = temp->next;
		free(curr);
	}

	free(request);
}
