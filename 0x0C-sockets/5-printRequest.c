#include "API_server.h"

/* printf */
#include <stdio.h>
/* strtok */
#include <string.h>
/* malloc free */
#include <stdlib.h>


/**
 * printRequest - interprets and prints the URL-encoded query in the
 *   Request-URI field of a vaildated HTTP request
 *
 * @request: pointer to struct populated with processed request contents
 */
void printRequest(HTTP_request_t *request)
{
	char **kv_pairs, *key, *value;
	int i, kv_pair_ct = 1;

	if (!request || !request->Request_URI || !request->URI_query)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}

	for (i = 0; request->URI_query[i]; i++)
		if (request->URI_query[i] == '&')
			kv_pair_ct++;

	kv_pairs = malloc(sizeof(char *) * kv_pair_ct);
	if (!kv_pairs)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}
	kv_pairs[0] = strtok(request->URI_query, "&");
	for (i = 1; i < kv_pair_ct; i++)
		kv_pairs[i] = strtok(NULL, "&");

	printf("Path: %s\n", request->Request_URI);

	for (i = 0; i < kv_pair_ct; i++)
	{
		key = strtok(kv_pairs[i], "=");
		value = strtok(NULL, "=");
		if (!kv_pairs[i] || !key || !value)
		{
			free(kv_pairs);
			HTTP_response(400, NULL, NULL);
			return;
		}
		printf("Query: \"%s\" -> \"%s\"\n", key, value);
	}

	free(kv_pairs);
	HTTP_response(200, NULL, NULL);
}
