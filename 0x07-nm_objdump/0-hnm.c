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
	char **args = NULL;
	re_state state;
	int i, ac, retval = 0;

	if (argc > 1)
	{
		ac = argc - 1;
		args = argv + 1;
	}
	else
	{
		ac = 1;
		args = default_args;
	}

	initState(&state);
	state.exec_name = argv[0];
	for (i = 0; i < ac; i++)
	{
		state.f_name = args[i];
		retval = openELF(&state);
		if (retval == 0)
		{
			retval = getFileHeader(&state);
			if (retval == 0)
				retval = (getSecHeaders(&state) ||
						  getSecHeadStrTab(&state));
			else
				errorMsg("%s: Failed to read file header\n",
						 NULL, &state);
			if (retval == 0)
				retval = getSymTables(&state);
			if (retval == 0)
				retval = printSymTables(&state);
		}
	}
	closeState(&state);
	return (retval);
}
