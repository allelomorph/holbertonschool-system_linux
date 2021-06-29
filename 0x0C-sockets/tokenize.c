#include "API_server.h"

/* strlen strncmp */
#include <string.h>
/* fprintf */
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
char *strtokSubstr(char *str, const char *delim)
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


/**
 * countTokensBySubstr - counts potential tokens in a string by a 1+ char
 *   delimiter, as would be produced by looping strtokSubstr until NULL output
 *
 * @str: string to subdivide by delimiter
 * @delim: substring by which to subdivide str
 * Return: amount of tokens that could be derived with strtokSubstr
 */
size_t countTokensBySubstr(const char *str, const char *delim)
{
	size_t tokens, delim_len, i;

	if (!str || !str[0])
		return (0);
	if (!delim)
		return (1);

	delim_len = strlen(delim);

	for (i = 0, tokens = 1; str[i];)
	{
		if (strncmp(str + i, delim, delim_len) == 0)
		{
			tokens++;
			i += delim_len;
		}
		else
			i++;
	}

	return (tokens);
}


/**
 * tokenizeBySubstr - divides a string into tokens based on strtokSubstr and
 *   its use of 1+ char delimiters
 *
 * @str: string to subdivide by delimiter
 * @delim: substring by which to subdivide str
 * @token_ct: count of tokens produced, modified by reference
 * Return: array of tokens derived with strtokSubstr, or NULL on failure
 */
char **tokenizeBySubstr(char *str, const char *delim, size_t *token_ct)
{
	char **tokens = NULL;
	size_t i;

	*token_ct = countTokensBySubstr(str, delim);

	tokens = malloc(sizeof(char *) * *token_ct);
	if (!tokens)
		/* response 500? */
		return (NULL);

	tokens[0] = strtokSubstr(str, delim);
	if (!tokens[0])
	{
		/* response 500? */
		free(tokens);
		return (NULL);
	}

	for (i = 1; i < *token_ct; i++)
	{
		tokens[i] = strtokSubstr(NULL, delim);
		if (!tokens[i])
		{
			/* response 500? */
			free(tokens);
			return (NULL);
		}
	}

	return (tokens);
}
