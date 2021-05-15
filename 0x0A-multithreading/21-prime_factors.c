#include "multithreading.h"
/* strtoul free */
#include <stdlib.h>


/**
 * addFactorToList - adds a factor to the factorization sequence for a number
 *   contained in a DLL
 *
 * @factor_list: DLL containing factorization sequence of a number
 * @n: factor value to add to list
 * Return: pointer to node added to list, or NULL on failure
 */
node_t *addFactorToList(list_t *factor_list, unsigned long n)
{
	unsigned long *factor = NULL;

	if (!factor_list)
		return (NULL);

	factor = malloc(sizeof(unsigned long));
	if (!factor)
	{
		list_destroy(factor_list, free);
		free(factor_list);
		return (NULL);
	}
	*factor = n;

	/* no failure provisions built into list_add (list.c) */
	return (list_add(factor_list, factor));
}


/**
 * prime_factors - factorizes a number into a list of prime factors
 *
 * @s: string representation of the number to factorize; this number will be
 *   positive and fit into an unsigned long
 * Return: list_t wrapper for a DLL containing the factorization sequence of a
 *   number, or NULL on failure
 */
list_t *prime_factors(char const *s)
{
	unsigned long n, i;
	list_t *factor_list = NULL;

	if (!s)
		return (NULL);

	factor_list = malloc(sizeof(list_t));
	if (!factor_list)
		return (NULL);
	list_init(factor_list);

	n = strtoul(s, NULL, 10); /* returns 0 for non-numeral strings */

	while (n % 2 == 0)
	{
		if (!addFactorToList(factor_list, 2))
			return (NULL);
		n /= 2;
	}

	for (i = 3; i * i <= n; i += 2)
	{
		while (n % i == 0)
		{
			if (!addFactorToList(factor_list, i))
				return (NULL);
			n /= i;
		}
	}

	if (n > 2)
	{
		if (!addFactorToList(factor_list, n))
			return (NULL);
	}

	return (factor_list);
}
