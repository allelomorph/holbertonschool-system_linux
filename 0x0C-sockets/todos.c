#include "API_server.h"


/* calling func: 404 on failure, 200 on success */

todo_t *getTodoByID(size_t id)
{
	todo_t *temp;

	for (temp = todos; temp; temp = temp->next)
	{
		if (temp->id == id)
			return (temp);
	}

	return (NULL);
}


/* calling func: 500 on failure, 201 on success */

todo_t *createTodo(const char *title, const char *description)
{
	static size_t id;
	todo_t *new, *temp;

	if (!title || !description)
		return (NULL);

	new = malloc(sizeof(todo_t));
	if (!new)
		return (NULL);

	new->id = id;
	new->title = strdup(title);
	new->description = strdup(description);
	new->next = NULL;

	id++;

	if (!todos)
	{
		todos = new;
		return (new);
	}

	for (temp = todos; temp && temp->next; temp = temp->next)
	{}
	temp->next = new;
	return (new);
}


/* calling func: 404 on failure, 204 on success */
int deleteTodo(size_t id)
{
	todo_t *temp, *prev;

	if (todos && todos->id == id)
	{
		temp = todos;
		todos = todos->next;
		free(temp->title);
		free(temp->description);
		free(temp);
		return (1);
	}

	for (prev = todos, temp = todos->next;
	     temp;
	     prev = temp, temp = temp->next)
	{
		if (temp->id == id)
		{
			prev->next = temp->next;
			free(temp->title);
			free(temp->description);
			free(temp);
			return (1);
		}
	}

	HTTP_response(404, NULL, NULL);
	return;
}


char *JSONSerializeAllTodos(size_t *JSON_len)
{
	char JSON_buf[JSON_BUFSZ] = {0};
	char *curr, *todo_JSON =
		"{\"id\":%lu,\"title\":\"%s\",\"description\":\"%s\"}";
	todo_t *temp;
	int incr;

	if (!JSON_len)
		reurn (NULL);

	curr = JSON_buf;
	*JSON_len = 0;

        *curr = '[';
	(*JSON_len)++;
	curr++;

	for (temp = todos; temp; temp = temp->next)
	{
		incr = sprintf(curr, temp_JSON, temp->id, temp->title,
			       temp->description);

		(*JSON_len) += incr;
		curr += incr;

		if (temp->next)
		{
		        *curr = ',';
			(*JSON_len)++;
			curr++;
		}
	}

	*curr = ']';
	(*JSON_len)++;
	curr++;

	return (strdup(JSON_buf));
}

char *JSONSerializeTodo(todo_t *todo, size_t *JSON_len)
{
	char todo_JSON =
		"{\"id\":%lu,\"title\":\"%s\",\"description\":\"%s\"}";
	char JSON_buf[JSON_BUFSZ] = {0};

	if (!todo || !JSON_len)
		reurn (NULL);

	*JSON_Len = sprintf(todo_JSON, todo->id, todo->title,
			    todo->description);

	return (strdup(JSON_buf));
}
