#include "API_server.h"

/* printf */
#include <stdio.h>
/* strtok */
#include <string.h>


/**
 * printRequest - interprets and prints the URL-encoded query in the
 *   Request-URI field of a vaildated HTTP request
 *
 * @request: pointer to struct populated with processed request contents
 */
void printRequest(HTTP_request_t *request)
{
	char *kv_pair1, *kv_pair2;
	char *key1, *key2, *value1, *value2;

	if (!request || !request->Request_URI || !request->URI_query)
		/* response 500? */
		return;

	kv_pair1 = strtok(request->URI_query, "&");
	kv_pair2 = strtok(NULL, "&");
	if (!kv_pair1 || !kv_pair2 || strtok(NULL, "&"))
		/* response 400? */
		return;

	key1 = strtok(kv_pair1, "=");
	value1 = strtok(NULL, "=");
	if (!key1 || !value1 || strtok(NULL, "="))
		/* response 400? */
		return;

	key2 = strtok(kv_pair2, "=");
	value2 = strtok(NULL, "=");
	if (!key2 || !value2 || strtok(NULL, "="))
		/* response 400? */
		return;

	printf("Path: %s\n", request->Request_URI);
	printf("Query: \"%s\" -> \"%s\"\n", key1, value1);
	printf("Query: \"%s\" -> \"%s\"\n", key2, value2);
	/* response 200? */
}
