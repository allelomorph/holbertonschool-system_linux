#include "holberton.h"

/* rewind fseek fread */
#include <stdio.h>

/* malloc */
#include <stdlib.h>


int getSecHeadStrTab(re_state *state)
{
        Elf64_Shdr st_shdr;
        char *buff = NULL;

	st_shdr = state->s_headers[state->f_header.e_shstrndx];

	rewind(state->f_stream);
        if (fseek(state->f_stream, st_shdr.sh_offset, SEEK_SET) == -1)
		return (1);

        buff = malloc(sizeof(char) * (st_shdr.sh_size));
	if (!buff)
		return (1);

	if (fread(buff, 1, st_shdr.sh_size, state->f_stream) != st_shdr.sh_size)
		return (1);

	state->sh_strtab = buff;

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
