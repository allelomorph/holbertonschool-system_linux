#include "holberton.h"

/* fprintf rewind fclose fopen */
#include <stdio.h>

/* memset */
#include <string.h>

/* stat S_ISREG */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* free */
#include <stdlib.h>


/* stat S_ISREG fopen rewind */
/* omitting "Out of memory allocating file data structure" error */
/* skipping archive opening and errors */
int openELFFile(re_state *state)
{
	struct stat statbuf;
	char magic[(EI_NIDENT / 2)];

	if (stat(state->f_name, &statbuf) < 0)
	{
		if (errno == ENOENT)
			errorMsg("'%s': No such file\n", NULL, state);
		else
			errorMsg("Could not locate '%s'.  System error " \
				    "message: %s\n", strerror(errno), state);
		return (1);
	}

	if (!S_ISREG(statbuf.st_mode))
	{
		errorMsg("'%s' is not an ordinary file\n", NULL, state);
		return (1);
	}

	state->f_stream = fopen(state->f_name, "rb");
	if (state->f_stream == NULL)
	{
		errorMsg("Input file '%s' is not readable.\n",
			      NULL, state);
		return (1);
	}

	/* initial check of ELF magic is only the first 8 bytes */
	if (fread(magic, (EI_NIDENT / 2), 1, state->f_stream) != 1)
	{
		errorMsg("%s: Failed to read file's magic number\n",
			    NULL, state);
		return (1);
	}
	state->f_size = statbuf.st_size;
	rewind(state->f_stream);
	return (0);
}


/* fprintf */
void errorMsg(char *format, char *err_str, re_state *state)
{
/*
	fprintf(stderr, "%s: Error: ", state->exec_name);
*/
	fprintf(stderr, "readelf: Error: ");
	if (err_str == NULL)
		fprintf(stderr, format, state->f_name);
	else
		fprintf(stderr, format, state->f_name, err_str);
}


/* memset */
void initState(re_state *state)
{
	state->exec_name = NULL;
	state->exit_code = 0;
	state->f_name = NULL;
	state->f_stream = NULL;
	state->f_size = 0;
	state->big_endian = false;
	state->ELF_32bit = false;
	memset(&(state->f_header), 0, sizeof(Elf64_Ehdr));
        state->s_headers = NULL;
        state->p_headers = NULL;
}


/* fclose free */
void closeState(re_state *state)
{
	if (state->f_stream != NULL)
	{
		fclose(state->f_stream);
		state->f_stream = NULL;
	}

	if (state->s_headers != NULL)
	{
		free(state->s_headers);
		state->s_headers = NULL;
	}

	if (state->p_headers != NULL)
	{
		free(state->p_headers);
		state->p_headers = NULL;
	}
}
