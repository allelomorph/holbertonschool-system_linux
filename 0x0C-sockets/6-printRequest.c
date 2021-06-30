#include "API_server.h"

/* printf */
#include <stdio.h>


/**
 * printRequest - prints headers of a vaildated HTTP request
 *
 * @request: pointer to struct populated with processed request contents
 */
void printRequest(HTTP_request_t *request)
{
	HTTP_header_t *hdr;

	if (!request)
		return;

	for (hdr = request->headers; hdr; hdr = hdr->next)
	{
		printf("Header: \"%s\" -> \"%s\"\n",
		       hdr->name, hdr->value);
	}
}
