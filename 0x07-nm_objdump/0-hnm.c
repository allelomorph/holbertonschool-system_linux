#include "holberton.h"

/* fprintf */
#include <stdio.h>


/**
 * main - entry point to hnm
 *
 * @argc: command line arg count
 * @argv: command line arg array
 * Return: 1 on failure, 0 on success
 */
int main(int argc, char **argv)
{
	char *default_args[] = {"a.out"};
	char **args = argc > 1 ? argv + 1 : default_args;
	re_state state;
	int i, retval, ac = argc > 1 ? argc - 1 : 1;

	/* nm takes multiple files as args */
	/* any failure for a single file sets exit code of 1 for entire loop */
	for (i = 0, retval = 0; i < ac; i++)
	{
		initState(&state);
		state.exec_name = argv[0];
		state.f_name = args[i];

		if (openELF(&state) == 0)
		{
			if (getFileHeader(&state) == 0)
			{
				if (!(getSecHeaders(&state) == 0 &&
				      getSecHeadStrTab(&state) == 0 &&
				      getSymTables(&state) == 0 &&
				      printSymTables(&state) == 0))
					retval = 1;
			}
			else
			{
				errorMsg("%s: File format not recognized\n",
					 NULL, &state);
				retval = 1;
			}
		}
		else
			retval = 1;

		closeState(&state);
	}

	return (retval);
}
