#include "holberton.h"

/* printf putchar */
#include <stdio.h>


/**
 * printProgHeaders - formatted printing of program headers (segments) stored
 * in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int printProgHeaders(re_state *state)
{
	uint64_t i;
	char *ph_fmt = NULL;
	Elf64_Phdr *segment = NULL;

	if (state->f_header.e_phnum == 0)
	{
		printf("\nThere are no program headers in this file.\n");
	        return (0);
	}

	printf ("\nElf file type is %s\n", getELFType(state->f_header.e_type));
	printf ("Entry point 0x%lx\n", state->f_header.e_entry);

	if (state->f_header.e_phnum == 1)
	{
		printf("There is %u program header, starting at offset %lu\n",
		       state->f_header.e_phnum, state->f_header.e_phoff);
	}
	else
	{
		printf("There are %u program headers, starting at offset %lu\n",
		       state->f_header.e_phnum, state->f_header.e_phoff);
	}

	printf("\nProgram Header%s:\n", state->f_header.e_phnum > 1 ? "s" : "");

	if (state->ELF_32bit)
	{
		printf("  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align\n");
		ph_fmt = "  %-14s 0x%06x 0x%08x 0x%08x 0x%05x 0x%05x %c%c%c %#lx\n";
	}
	else
	{
		printf("  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align\n");
		ph_fmt = "  %-14s 0x%06lx 0x%016lx 0x%016lx 0x%06lx 0x%06lx %c%c%c %#lx\n";
	}

	for (i = 0; i < state->f_header.e_phnum; i++)
	{
		segment = state->p_headers + i;
		printf(ph_fmt,
		       getSegType(segment->p_type),
		       segment->p_offset,
		       segment->p_vaddr,
		       segment->p_paddr,
		       segment->p_filesz,
		       segment->p_memsz,
		       (segment->p_flags & PF_R) ? 'R' : ' ',
		       (segment->p_flags & PF_W) ? 'W' : ' ',
		       (segment->p_flags & PF_X) ? 'E' : ' ',
		       segment->p_align);

		if (segment->p_type == PT_INTERP)
		{
			printf("      [Requesting program interpreter: %s]\n",
			       state->prog_interp);
		}
	}

	if (state->s_headers != NULL
	    && state->sh_strtab != NULL)
	{
		printf("\n Section to Segment mapping:\n");
		printf("  Segment Sections...\n");

		for (i = 0; i < state->f_header.e_phnum; i++)
		{
			unsigned int j;
			Elf64_Shdr *section = NULL;
			uint64_t seg_end;

			printf ("   %2.2lu     ", i);

			segment = state->p_headers + i;
		        seg_end = segment->p_offset + segment->p_filesz;

			for (j = 1; j < state->f_header.e_shnum; j++)
			{
				section = state->s_headers + j;

				/* ignoring .tbss, .bss only NOBITS handled */
			        if (section->sh_type == SHT_NOBITS)
				{
					if (segment->p_type == PT_LOAD &&
					    (segment + 1)->p_type != PT_LOAD)
						printf("%s ", state->sh_strtab + section->sh_name);
				}
				else
				{
				        if (!(segment->p_type == PT_DYNAMIC &&
					      state->f_header.e_ident[EI_OSABI] == ELFOSABI_SOLARIS) &&
					    (section->sh_offset >= segment->p_offset &&
					     section->sh_offset < seg_end))
						printf("%s ", state->sh_strtab + section->sh_name);
				}
			}
			putchar('\n');
		}
	}

	return (0);
}
