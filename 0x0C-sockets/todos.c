#include "API_server.h"

/*
malloc
strdup
free spritnf
*/

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
		return (0);
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
			return (0);
		}
	}

	return (1);
}



size_t JSONSerializeAllTodos(char *JSON_output)
{
	char JSON_buf[JSON_BUFSZ] = {0};
	char *curr, *todo_JSON =
		"{\"id\":%lu,\"title\":\"%s\",\"description\":\"%s\"}";
	todo_t *temp;
        size_t JSON_len;
	int incr;

	if (!JSON_output)
		reurn (0);

	curr = JSON_buf;
	JSON_len = 0;

        *curr = '[';
        JSON_len++;
	curr++;

	for (temp = todos; temp; temp = temp->next)
	{
		incr = sprintf(curr, temp_JSON, temp->id, temp->title,
			       temp->description);

		JSON_len += incr;
		curr += incr;

		if (temp->next)
		{
		        *curr = ',';
			JSON_len++;
			curr++;
		}
	}

	*curr = ']';
        JSON_len++;
	curr++;

	*JSON_output = strdup(JSON_buf);

	return (JSON_len);
}

size_t JSONSerializeTodo(todo_t *todo, char **JSON_output)
{
	size_t JSON_len;
	char todo_JSON_fmt =
		"{\"id\":%lu,\"title\":\"%s\",\"description\":\"%s\"}";
	char JSON_buf[JSON_BUFSZ] = {0};

	if (!todo || !JSON_output)
		reurn (0);

	JSON_Len = sprintf(todo_JSON_fmt, todo->id, todo->title,
			   todo->description);

	*JSON_output = strdup(JSON_buf);

	return (JSON_len);
}
