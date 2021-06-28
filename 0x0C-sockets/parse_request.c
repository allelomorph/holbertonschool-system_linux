#include "API_server.h"

/* strlen strncmp strdup */
#include <string.h>
/* fprintf printf */
#include <stdio.h>
/* malloc free */
#include <stdlib.h>

/* from shell_v2/ */
/**
 * strtokSubstr - largely similar to library function strtok, returns pointers
 * substrings found by replacing delimiters found with null bytes.
 * Two main differences from strtok:
 * 1 - mulitchar delim arg treated as if entire string is one delim
 * 2 - when delimter found at beginning of string, end of string, or two delims
 * are adjacent in the string, returns "" token instead of skipping.
 *
 * @str: string to subdivide by delimiter; if NULL continue parsing same str
 * @delim: substring by which to subdivide str
 * Return: pointer to next token found , or NULL on failure or when no more
 * tokens are available
 */
char *strtokSubstr(char *str, char *delim)
{
	static char *nextToken, *parseStr;
	size_t i, delimLen, parseStrLen;

	if (!delim || !delim[0])
	{
		fprintf(stderr, "strtokSubstr: missing delimiter\n");
		return (NULL);
	}
	/* str != NULL starts parsing of new string */
	/* testing (str[0] != '\0') prevents returning 1 token for empty str */
	if (str && str[0])
	{
		parseStr = str;
		nextToken = NULL;
	}
	else
	{
		if (!nextToken) /* previous save point already at final \0 */
			return (NULL);
		/* still parsing previous string `str` */
		parseStr = nextToken;
	}
	delimLen = strlen(delim);
	parseStrLen = strlen(parseStr);
	for (i = 0; parseStr[i]; i++)
	{
		if (parseStr[i] == delim[0])
		{
			if (parseStrLen >= i + delimLen &&
			    (strncmp(parseStr + i, delim, delimLen) == 0))
			{
				nextToken = (parseStr + i + delimLen);
				parseStr[i] = '\0';
				break;
			}
		}
	}
	if (nextToken == parseStr) /* no more tokens, final valid return */
		nextToken = NULL;
	return (parseStr);
}


HTTP_request_t *parseHTTPRequest(char *recv_str)
{
	char **message_lines;
	size_t message_line_ct, i;

	if (!recv_str)
		/* response? */
		return (NULL);

	/* ignore leading CRLFs */
	while (*recv_str == '\r' && *(recv_str + 1) == '\n')
		recv_str += 2;

	for (i = 0, message_line_ct = 1; recv_str[i]; i++)
	{
		if (recv_str[i] == '\r' && recv_str[i + 1] == '\n')
		{
			message_line_ct++;
		        i += 2;
		}
	}

	message_lines = malloc(sizeof(char *) * message_line_ct);
	if (!message_lines)
		/* response? */
		return (NULL);

	message_lines[0] = strtokSubstr(recv_str, "\r\n");
	for (i = 1; i < message_line_ct; i++)
		message_lines[i] = strtokSubstr(NULL, "\r\n");

	for (i = 0; i < message_line_ct; i++)
		printf("'%s'\n", message_lines[i]);

	free(message_lines);

	return (NULL);
}
