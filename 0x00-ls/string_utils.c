#include "hls.h"


/**
 * stringExactMatch - goes beyond strcmp to ensure that the two strings match to
 * the null bit
 * @s1: first string to compare
 * @s2: second string to compare
 * Return: boolean representing exact match
 */
bool stringExactMatch(const char *s1, char *s2)
{
	while (s1 && s2)
	{
		if (*s1 != *s2)
			return (false);
		else if (*s1 == '\0' && *s2 == '\0')
			return (true);
		s1++;
		s2++;
	}
	return (false);
}


/**
 * _strcopy - copies a string
 * @string: original string
 * Return: copied string, or NULL on failure
 */
char *_strcopy(char *string)
{
	int i, len = 0;
	char *copy = NULL;

	if (!string)
		return (NULL);

	/* measure length */
	for (i = 0; string[i]; i++)
		len++;

	/* new array */
	copy = malloc(sizeof(char) * (len + 1));
	if (!copy)
	{
		fprintf(stderr, "_strcopy: Can't malloc\n");
		return (NULL);
	}

	/* copy */
	for (i = 0; i <= len; i++)
		copy[i] = string[i];

	return (copy);
}


/**
 * _strlen - counts char bytes in a string, not inlcuding null byte
 *
 * @s: string to be measured
 * Return: length of string
 */
unsigned int _strlen(const char *s)
{
        unsigned int len;

        if (!s)
                return (0);

        for (len = 0; *s; s++)
                len++;

        return (len);
}
