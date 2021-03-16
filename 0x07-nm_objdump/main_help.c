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
/**
 * openELF - attempts to open an ELF for parsing and printing
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int openELF(re_state *state)
{
	struct stat statbuf;
	char magic[(EI_NIDENT / 2)];

	errno = 0;
	if (stat(state->f_name, &statbuf) < 0)
	{
		if (errno == ENOENT)
			errorMsg("'%s': No such file\n", NULL, state);
		else
			errorMsg("%s: %s\n", strerror(errno), state);
		return (1);
	}

	if (!S_ISREG(statbuf.st_mode))
	{
		errorMsg("Warning: '%s' is not an ordinary file\n",
			 NULL, state);
		return (1);
	}

	/* nm skips files with size 0 but still sets exit code of 1 */
	if (statbuf.st_size == 0)
		return (1);
	state->f_size = statbuf.st_size;

	state->f_stream = fopen(state->f_name, "rb");
	if (state->f_stream == NULL)
	{
		errorMsg("%s: %s\n", strerror(errno), state);
		return (1);
	}

	/* initial check of ELF magic in first 8 bytes */
	if (fread(magic, (EI_NIDENT / 2), 1, state->f_stream) != 1 ||
	    magic[EI_MAG0] != ELFMAG0 || magic[EI_MAG1] != ELFMAG1 ||
	    magic[EI_MAG2] != ELFMAG2 || magic[EI_MAG3] != ELFMAG3)
	{
		errorMsg("%s: File format not recognized\n",
			 NULL, state);
		return (1);
	}
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
	fprintf(stderr, "%s: ", state->exec_name);
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
		fclose(state->f_stream);

	if (state->s_headers != NULL)
		free(state->s_headers);

	if (state->sh_strtab != NULL)
		free(state->sh_strtab);

	if (state->prog_interp != NULL)
		free(state->prog_interp);

	if (state->p_headers != NULL)
		free(state->p_headers);

	if (state->dyn_sym != NULL)
		free(state->dyn_sym);

	if (state->sym_tab != NULL)
		free(state->sym_tab);
}
