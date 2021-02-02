#include "holberton.h"

/* fprintf */
#include <stdio.h>


/**
 * main - entry point to 100-hreadelf
 *
 * @argc: command line arg count
 * @argv: command line arg array
 * Return: 1 on failure, 0 on success
 */
int main(int argc, char **argv)
{
	re_state state;
	int retval = 0;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return (1);
	}

	initState(&state);
	state.exec_name = argv[0];
	state.f_name = argv[1];

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

	closeState(&state);
	return (retval);
}
