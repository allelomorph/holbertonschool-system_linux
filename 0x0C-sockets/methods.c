#include "API_server.h"

/* malloc free strtoul */
#include <stdlib.h>
/* strncmp strlen strtok */
#include <string.h>
/* sprintf */
#include <stdio.h>


int addResponseHeader(HTTP_header_t **head, const char *name,
		      const char *description)
{
	HTTP_header_t *new = NULL, *temp;

	if (!head || !name || !description)
		return (1);

	new = malloc(sizeof(HTTP_header_t));
	if (!new)
		return (1);

	new->name = strdup(name);
	new->value = strdup(description);
	new->type = getHeaderType(new->name);
	new->next = NULL;

	if (!(*head))
		*head = new;
	else
	{
		for (temp = *head; temp && temp->next; temp = temp->next)
		{}

		temp->next = new;
	}

	return (0);
}


void freeResponseHeaders(HTTP_header_t **head)
{
	HTTP_header_t *temp, *curr;

	if (!head)
		return;

	for (temp = *head; temp;)
	{
		curr = temp;
		temp = temp->next;

		free(curr->name);
		free(curr->value);
		free(curr);
		curr = NULL;
	}
}





HTTP_header_t *getHeaderByName(const char *name, HTTP_header_t *headers)
{
	HTTP_header_t *temp;

	if (!name || !headers)
		return (NULL);

	for (temp = headers; temp; temp = temp->next)
	{
		if (strncmp(name, temp->name, strlen(temp->name) + 1) == 0)
			return (temp);
	}

	return (NULL);
}




int todoValuesFromQuery(char *query, char **title, char **description)
{
	char *kv_pair1, *kv_pair2, *key, *value;

	if (!query || !title || !description)
	{
		HTTP_response(500, NULL, NULL);
		return (1);
	}

	kv_pair1 = strtok(query, "&");
	kv_pair2 = strtok(NULL, "&");

	key = strtok(kv_pair1, "=");
	value = strtok(NULL, "=");
	if (!kv_pair1 || !key || !value ||
	    strncmp("title", key, strlen("title") + 1) != 0)
	{
		HTTP_response(422, NULL, NULL);
		return (1);
	}
	*title = value;

	key = strtok(kv_pair2, "=");
	value = strtok(NULL, "=");
	if (!kv_pair2 || !key || !value ||
	    strncmp("description", key, strlen("description") + 1) != 0)
	{
		HTTP_response(422, NULL, NULL);
		return (1);
	}
	*description = value;

	return (0);
}




int IDFromQuery(char *query, size_t *id)
{
	char *key, *value;
	int i, kv_pair_ct = 1;

	if (!query || !id)
		return (1);

	for (i = 0; query[i]; i++)
		if (query[i] == '&')
			kv_pair_ct++;

	/* expecting just '?id={}' */
	if (kv_pair_ct > 1)
		return (1);

	key = strtok(query, "=");
	value = strtok(NULL, "=");
	if (!key || !value ||
	    strncmp("id", key, strlen("id") + 1) != 0)
		return (1);

	*id = (size_t)strtoul(value, NULL, 10);
	return (0);
}

/*
  -check for header Content-Length
  ----if no, respond 411

  - check query in body
  title -> String
  description: -> String
  (no %hex or + decoding)

  bad query syntax: 422 Unprocessable Entity -> Missing title or description

  on success respond 201 Created
  ----response has header Content-Type: application/json
  ----Response body must include the json representation of the created todo
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

	if (addResponseHeader(&resp_hdrs, "Content-Type",
			      "application/json") != 0 ||
	    addResponseHeader(&resp_hdrs, "Content-Length",
			      JSON_len_buf) != 0)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}

	HTTP_response(201, resp_hdrs, msg_body);
	freeResponseHeaders(&resp_hdrs);
	free(msg_body);
}

/*
no URI query?
Retrieves the list of all the todos
Response:
    200 OK -> Todos retrieved
        Response must include the header Content-Type: application/json
        Response must include the json representation of the list of todos in its body
        NOTE: If there is no todo in the list, your server must return an empty list

URI qeury of ?id={}
Response:
    200 OK -> Todo retrieved
        Response must include the header Content-Type: application/json
        Response must include the json representation of the list of todos in its body
    404 Not Found -> No todo with this id


 */

void methodGET(HTTP_request_t *request, int GET_body_flag)
{
	size_t id, JSON_len = 0;
	todo_t *todo;
	HTTP_header_t *resp_hdrs = NULL;
	char *msg_body = NULL;
	char JSON_len_buf[20] = {0};

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
	if (addResponseHeader(&resp_hdrs, "Content-Type",
			      "application/json") != 0 ||
	    addResponseHeader(&resp_hdrs, "Content-Length",
			      JSON_len_buf) != 0)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}

	if (GET_body_flag == INCL_MSG_BODY)
		HTTP_response(200, resp_hdrs, msg_body);
	else
		HTTP_response(200, resp_hdrs, NULL);
	free(resp_hdrs);
	free(msg_body);
}


/*
Required queries:

    id -> Positive integer

Required headers: None
Required body parameters: None
Deletes a todo, identified by its id
Response:

    204 No Content -> Todo deleted
    404 Not Found -> No todo with this id

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
