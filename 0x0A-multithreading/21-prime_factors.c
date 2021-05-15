#include "multithreading.h"
/* strtoul free */
#include <stdlib.h>

#include <stdio.h>

#ifdef ZZZ
/* strlen */
#include <string.h>
/* isdigit */
#include <ctype.h>


/*
void freeUL(void *node_data)
{
	free((unsigned long *)node_data);
}
*/
#endif

/**
 * prime_factors - factorizes a number into a list of prime factors
 *
 * @s: string representation of the number to factorize; this number will be
 *   positive and fit into an unsigned long
 * Return: ?? TBD (list_t defined in list.h)
 */
list_t *prime_factors(char const *s)
{
	unsigned long n, i, j, steps = 0, *factor = NULL;
	list_t *factor_list = NULL;
	int is_prime;

	if (!s)
		return (NULL);
	factor_list = malloc(sizeof(list_t));
	if (!factor_list)
		return (NULL);
	list_init(factor_list);

	n = strtoul(s, NULL, 10); /* returns 0 for non-numeral strings */
	for (i = 2; i <= n; i++, steps++)
	{
		if (n % i == 0)
		{
			is_prime = 1;
			for (j = 2; j <= i/2; j++, steps++)
			{
				if (i % j == 0)
				{
				        is_prime = 0;
					break;
				}
			}
			if (is_prime)
			{
				factor = malloc(sizeof(unsigned long));
				if (!factor)
				{
					list_destroy(factor_list, free);
					return (NULL);
				}
				*factor = i;
				list_add(factor_list, factor);
				n /= i;
			}
		}
	}

	printf("\tfactored in %lu steps\n", steps);

	return (factor_list);
}
