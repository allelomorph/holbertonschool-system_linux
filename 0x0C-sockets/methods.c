#include "API_server.h"

/* malloc free strtoul */
#include <stdlib.h>
/* strncmp strlen strtok */
#include <string.h>
/* sprintf */
#include <stdio.h>


/**
 * methodPOST - Performs POST from incoming HTTP request, specifically for this
 *   API adding a todo record to the list todos
 *
 * @request: Pointer to struct containing strings parsed from raw HTTP request
 */
void methodPOST(HTTP_request_t *request)
{
	todo_t *todo;
	HTTP_header_t *resp_hdrs = NULL;
	char *title = NULL, *description = NULL, *msg_body = NULL;
	char JSON_len_buf[20] = {0};
	size_t JSON_len;

	if (!request)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}
	if (todoValuesFromQuery(request->message_body, &title,
				&description) != 0)
		return;
	todo = createTodo(title, description);
	if (!todo)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}

	JSON_len = JSONSerializeTodo(todo, &msg_body);
	sprintf(JSON_len_buf, "%lu", JSON_len);

	if (addResponseHeader(&resp_hdrs, "Content-Length",
			      JSON_len_buf) != 0 ||
	    addResponseHeader(&resp_hdrs, "Content-Type",
			      "application/json") != 0)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}

	HTTP_response(201, resp_hdrs, msg_body);
	freeResponseHeaders(&resp_hdrs);
	free(msg_body);
}


/**
 * methodGET - Performs GET or HEAD from incoming HTTP request, specifically
 *   for this API reading a todo record from the list todos, including the
 *   message body for GET and not for HEAD
 *
 * @request: Pointer to struct containing strings parsed from raw HTTP request
 *
 * @GET_body_flag: If 1/INCL_MSG_BODY, treat as GET request; if 0/NO_MSG_BODY,
 *   treat as HEAD
 */
void methodGET(HTTP_request_t *request, int GET_body_flag)
{
	size_t id, JSON_len = 0;
	todo_t *todo;
	HTTP_header_t *resp_hdrs = NULL;
	char *msg_body = NULL, JSON_len_buf[20] = {0};

	if (!request)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}
	if (request->URI_query)
	{
		if (IDFromQuery(request->URI_query, &id) != 0)
		{
			HTTP_response(400, NULL, NULL);
			return;
		}
		todo = getTodoByID(id);
		if (!todo)
		{
			HTTP_response(404, NULL, NULL);
			return;
		}
		JSON_len = JSONSerializeTodo(todo, &msg_body);
	}
	else
		JSON_len = JSONSerializeAllTodos(&msg_body);
	sprintf(JSON_len_buf, "%lu", JSON_len);
	if (addResponseHeader(&resp_hdrs, "Content-Length",
			      JSON_len_buf) != 0 ||
	    addResponseHeader(&resp_hdrs, "Content-Type",
			      "application/json") != 0)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}
	HTTP_response(200, resp_hdrs,
		      (GET_body_flag == INCL_MSG_BODY) ? msg_body : NULL);
	free(resp_hdrs);
	free(msg_body);
}


/**
 * methodDELETE - Performs DELETE from incoming HTTP request, specifically for
 *   this API removing a todo record from the list todos
 *
 * @request: Pointer to struct containing strings parsed from raw HTTP request
 */
void methodDELETE(HTTP_request_t *request)
{
	size_t id;
	char *key, *value;
	int i, kv_pair_ct = 1;

	if (!request || !request->URI_query)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}

	for (i = 0; request->URI_query[i]; i++)
		if (request->URI_query[i] == '&')
			kv_pair_ct++;

	if (kv_pair_ct > 1)
	{
		HTTP_response(400, NULL, NULL);
		return;
	}

	key = strtok(request->URI_query, "=");
	value = strtok(NULL, "=");
	if (!key || !value)
	{
		HTTP_response(422, NULL, NULL);
		return;
	}

	id = (size_t)strtoul(value, NULL, 10);

	if (deleteTodo(id) != 0)
	{
		HTTP_response(404, NULL, NULL);
		return;
	}

	HTTP_response(204, NULL, NULL);
}


/**
 * runMethod - Checks request Method field to run appropriate method helper
 *
 * @request: Pointer to struct containing strings parsed from raw HTTP request
 */
void runMethod(HTTP_request_t *request)
{
	if (!request)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}

	if (strncmp("POST", request->Method, strlen("POST") + 1) == 0)
	{
		methodPOST(request);
		return;
	}

	else if (strncmp("GET", request->Method, strlen("GET") + 1) == 0)
	{
		methodGET(request, INCL_MSG_BODY);
		return;
	}
	else if (strncmp("HEAD", request->Method, strlen("HEAD") + 1) == 0)
	{
		methodGET(request, NO_MSG_BODY);
		return;
	}
	else if (strncmp("DELETE", request->Method, strlen("DELETE") + 1) == 0)
	{
		methodDELETE(request);
		return;
	}

	/* redundant check, methods already filtered by isMethodImplemented() */
	HTTP_response(404, NULL, NULL);
}
