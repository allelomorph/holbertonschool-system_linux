#include "holberton.h"

/* printf */
#include <stdio.h>


int printSecHeaders(re_state *state)
{
	uint64_t i;
	char *sh_fmt = NULL;
	Elf64_Shdr *curr = NULL;

	/* when to use "\nThere are no sections to group in this file.\n"? */
	switch (state->f_header.e_shnum)
	{
	case 0:
		printf("\nThere are no sections in this file.\n");
	        return (0);
	case 1:
		printf("There is %d section header, starting at offset 0x%lx:\n",
		       state->f_header.e_shnum, state->f_header.e_shoff);
		break;
	default:
		printf("There are %d section headers, starting at offset 0x%lx:\n",
		       state->f_header.e_shnum, state->f_header.e_shoff);
		break;
	}

	printf("\nSection Header%s:\n", state->f_header.e_shnum > 1 ? "s" : "");

	if (state->ELF_32bit)
	{
		printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
		sh_fmt = "  [%2i] %-17s %-15s %08x %06x %06x %02lx %3s %2u %3u %2lu\n";
	}
	else
	{
		printf("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");
		sh_fmt = "  [%2i] %-17s %-15s %016x %06x %06x %02lx %3s %2u %3u %2lu\n";
	}

	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		curr = state->s_headers + i;
		printf(sh_fmt, i,
		       state->sh_strtab + curr->sh_name,
		       getSecType(curr->sh_type),
		       curr->sh_addr,
		       curr->sh_offset,
		       curr->sh_size,
		       curr->sh_entsize,
		       getSecFlags(curr->sh_flags),
		       curr->sh_link,
		       curr->sh_info,
		       curr->sh_addralign);
	}

	printf("Key to Flags:\n" \
	       "  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n" \
	       "  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n" \
	       "  O (extra OS processing required) o (OS specific), p (processor specific)\n");

	return (0);
}
