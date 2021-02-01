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
/**
 * openELF - attempts to open an ELF for printing
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int openELF(re_state *state)
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
/**
 * errorMsg - formats error printing
 *
 * @format: error format string
 * @err_str: optional second string containing errno string
 * @state: struct containing file data and info for error printing
 */
void errorMsg(char *format, char *err_str, re_state *state)
{
	fprintf(stderr, "%s: Error: ", state->exec_name);
	if (err_str == NULL)
		fprintf(stderr, format, state->f_name);
	else
		fprintf(stderr, format, state->f_name, err_str);
}


/* memset */
/**
 * initState - initializes state struct at start
 *
 * @state: struct containing file data and info for error printing
 */
void initState(re_state *state)
{
	state->exec_name = NULL;
	state->f_name = NULL;
	state->f_stream = NULL;
	state->f_size = 0;
	state->big_endian = false;
	state->ELF_32bit = false;
	memset(&(state->f_header), 0, sizeof(Elf64_Ehdr));
        state->s_headers = NULL;
	state->sh_strtab = NULL;
	state->prog_interp = NULL;
        state->p_headers = NULL;
	state->dyn_sym = NULL;
	state->sym_tab = NULL;
}


/* fclose free */
/**
 * closeState - closes file stream and frees all memory in state
 *
 * @state: struct containing file data and info for error printing
 */
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

	if (state->sh_strtab != NULL)
	{
		free(state->sh_strtab);
		state->sh_strtab = NULL;
	}

	if (state->prog_interp != NULL)
	{
		free(state->prog_interp);
		state->prog_interp = NULL;
	}

	if (state->p_headers != NULL)
	{
		free(state->p_headers);
		state->p_headers = NULL;
	}

	if (state->dyn_sym != NULL)
	{
		free(state->dyn_sym);
		state->dyn_sym = NULL;
	}

	if (state->sym_tab != NULL)
	{
		free(state->sym_tab);
		state->sym_tab = NULL;
	}

}
