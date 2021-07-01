#include "API_server.h"

/* malloc free strtoul */
#include <stdlib.h>
/* strdup strncmp strlen strtok */
#include <string.h>


/**
 * addResponseHeader - TBD
 *
 * @head: TBD
 * @name: TBD
 * @description: TBD
 * Return: TBD
 */
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


/**
 * freeResponseHeaders - TBD
 *
 * @head: TBD
 */
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


/**
 * getHeaderByName - TBD
 *
 * @name: TBD
 * @headers: TBD
 * Return: TBD
 */
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


/**
 * todoValuesFromQuery - TBD
 *
 * @query: TBD
 * @title: TBD
 * @description: TBD
 * Return: TBD
 */
int todoValuesFromQuery(char *query, char **title, char **description)
{
	char *kv_pair1, *kv_pair2, *key1, *value1 = NULL,
		*key2, *value2 = NULL;

	if (!query || !title || !description)
	{
		HTTP_response(500, NULL, NULL);
		return (1);
	}

	kv_pair1 = strtok(query, "&");
	kv_pair2 = strtok(NULL, "&");

	key1 = strtok(kv_pair1, "=");
	value1 = strtok(NULL, "=");

	key2 = strtok(kv_pair2, "=");
	value2 = strtok(NULL, "=");

	if (strncmp("title", key1, strlen("title") + 1) == 0)
		*title = value1;
	else if (strncmp("description", key1, strlen("description") + 1) == 0)
		*description = value1;
	else
	{
		HTTP_response(422, NULL, NULL);
		return (1);
	}

	if (strncmp("title", key2, strlen("title") + 1) == 0)
		*title = value2;
	else if (strncmp("description", key2, strlen("description") + 1) == 0)
		*description = value2;
	else
	{
		HTTP_response(422, NULL, NULL);
		return (1);
	}

	return (0);
}


/**
 * IDFromQuery - TBD
 *
 * @query: TBD
 * @id: TBD
 * Return: TBD
 */
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
