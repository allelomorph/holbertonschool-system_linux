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
	HTTP_header_t *temp;

	if (!request)
		return;

	while (request->headers)
	{
		temp = request->headers;
		request->headers = request->headers->next;
		free(temp);
	}

	free(request);
}


/**
 * freeTodos - frees global SLL of todos
 */
void freeTodos(void)
{
	todo_t *temp;

	while (todos)
	{
		temp = todos;
		todos = todos->next;
		free(temp->title);
		free(temp->description);
		free(temp);
	}
}
