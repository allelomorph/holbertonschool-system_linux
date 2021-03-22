#include "hobjdump.h"
/* fopen fread rewind fprintf fclose */
#include <stdio.h>
/* memcmp strerror memset */
#include <string.h>
/* stat S_ISREG */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/* free */
#include <stdlib.h>


/* stat strerror S_ISREG fopen fread memcmp rewind */
/**
 * openELF - attempts to open an ELF for parsing and printing
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int openELF(objdump_state *state)
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
	/* nm skips files of size 0 with an exit code of 1 */
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
	    memcmp(ELFMAG, magic, SELFMAG) != 0)
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
void errorMsg(char *format, char *err_str, objdump_state *state)
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
void initState(objdump_state *state)
{
	state->exec_name = NULL;
	state->f_stream = NULL;
	state->f_size = 0;
	state->big_endian = false;
	state->ELF_32bit = false;
	memset(&(state->f_header), 0, sizeof(Elf64_Ehdr));
	state->s_headers = NULL;
	state->sh_strtab = NULL;
}


/* fclose free */
/**
 * closeState - closes file stream and frees all memory in state
 *
 * @state: struct containing file data and info for error printing
 */
void closeState(objdump_state *state)
{
	if (state->f_stream != NULL)
		fclose(state->f_stream);

	if (state->s_headers != NULL)
		free(state->s_headers);

	if (state->sh_strtab != NULL)
		free(state->sh_strtab);
}
