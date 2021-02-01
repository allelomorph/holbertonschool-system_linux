#include "holberton.h"

/* snprintf */
#include <stdio.h>

/* malloc */
#include <stdlib.h>

/* strcmp */
#include <string.h>


/* snprintf */
/**
 * getSegType - converts segment type code to formatted string
 *
 * @p_type: segment type code from segment header
 * Return: string containing segment type
 */
const char *getSegType(Elf64_Word p_type)
{
	static char buff[32];

	switch (p_type)
	{
	case PT_NULL:return "NULL";
	case PT_LOAD:return "LOAD";
	case PT_DYNAMIC:return "DYNAMIC";
	case PT_INTERP:return "INTERP";
	case PT_NOTE:return "NOTE";
	case PT_SHLIB:return "SHLIB";
	case PT_PHDR:return "PHDR";
	case PT_TLS:return "TLS";
	case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
	case PT_GNU_STACK:return "GNU_STACK";
	case PT_GNU_RELRO:  return "GNU_RELRO";
	default:
		/* ignoring PT_LOPROC to PT_HIPROC and related EM_ codes */
		if ((p_type >= PT_LOOS) && (p_type <= PT_HIOS))
			sprintf (buff, "LOOS+%x", p_type - PT_LOOS);
		else
			snprintf (buff, sizeof(buff), "<unknown>: %x", p_type);
	}

	return buff;
}


/**
 * getProgInterp - reads ELF .interp section to store name of program
 * interpreter in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int getProgInterp(re_state *state)
{
	Elf64_Shdr s_head;
	char *sh_name_str = NULL, *buff = NULL;
	int i;

	if (state->sh_strtab == NULL)
		return (1);

	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		s_head = state->s_headers[i];
		sh_name_str = (state->sh_strtab) + s_head.sh_name;
		if (strcmp(sh_name_str, ".interp") == 0)
		{
			rewind(state->f_stream);
			if (fseek(state->f_stream,
				  s_head.sh_offset, SEEK_SET) == -1)
				return (1);

			buff = malloc(sizeof(char) * (s_head.sh_size));
			if (!buff)
				return (1);

			if (fread(buff, 1, s_head.sh_size,
				  state->f_stream) != s_head.sh_size)
				return (1);

			state->prog_interp = buff;
		}
	}

	return (0);
}

/*
	printf("\n\ttest printing state->sh_strtab\n");
	for (i = 0; i < st_shdr.sh_size; i++)
	{
		printf("%c", state->sh_strtab[i] == '\0' ? '.' : state->sh_strtab[i]);
		if ((i + 1) % 16 == 0)
			putchar('\n');
	}
	printf("\ni at loop exit: %lu\n", i);
*/
