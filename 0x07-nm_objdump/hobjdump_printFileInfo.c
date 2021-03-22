#include "hobjdump.h"
/* printf */
#include <stdio.h>


/**
 * fileMachString - returns string corresponding to ELF machine type
 * note: only 2 EM_* values handled in sample set of inputs
 *
 * @state: struct containing file data and info for error printing
 * Return: machine type string, or NULL on failure
 */
char *fileMachString(objdump_state *state)
{
	char *ELF_mach = NULL;

	if (!state)
		return (NULL);

	switch (state->f_header.e_machine)
	{
	case EM_386:
		ELF_mach = "i386";
		break;
	case EM_X86_64:
		ELF_mach = "x86-64";
		break;
	default:
		ELF_mach = state->big_endian ? "big" : "little";
		break;
	}

	return (ELF_mach);
}


/**
 * fileArchString - returns string corresponding to ELF architecture type
 * note: only 2 EM_* values handled in sample set of inputs
 *
 * @state: struct containing file data and info for error printing
 * Return: architecture type string, or NULL on failure
 */
char *fileArchString(objdump_state *state)
{
	if (!state)
		return (NULL);

	switch (state->f_header.e_machine)
	{
	case EM_386:
		return ("i386");
	case EM_X86_64:
		return ("i386:x86-64");
	default:
		return ("UNKNOWN!");
	}
}


/**
 * setFileFlags - returns set of bfd-style ELF flags
 *
 * @state: struct containing file data and info for error printing
 * Return: flag set bitwise OR'd into one uint32_t, or 0 on failure or no flags
 */
uint32_t setFileFlags(objdump_state *state)
{
	uint32_t i, flags = 0;
	Elf64_Shdr *section = NULL;

	if (!state || !state->s_headers)
		return (0);

	switch (state->f_header.e_type)
	{
	case ET_REL:
		flags |= HAS_RELOC; /* ELF contains relocation entries */
		break;
	case ET_EXEC:
		flags |= EXEC_P;  /* ELF is directly executable */
		flags |= D_PAGED; /* ELF is dynamically paged */
		break;
	case ET_DYN:
		flags |= DYNAMIC; /* ELF is directly executable */
		flags |= D_PAGED;
		break;
	default:
		flags |= D_PAGED;
		break;
	}

	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		section = state->s_headers + i;
		if (section &&
		    (section->sh_type == SHT_DYNSYM ||
		     section->sh_type == SHT_SYMTAB))
		{
			flags |= HAS_SYMS; /* ELF has symbols */
			break;
		}
	}

	return (flags);
}


/* printf */
/**
 * printFileFlags - formatted printing of bfd-style ELF flags
 *
 * @flags: flag set bitwise OR'd into one uint32_t
 */
void printFileFlags(uint32_t flags)
{
	uint32_t flag_set[] = { HAS_RELOC, EXEC_P, HAS_SYMS, DYNAMIC, D_PAGED };
	char *flag_names[] = { "HAS_RELOC", "EXEC_P", "HAS_SYMS",
		"DYNAMIC", "D_PAGED" };
	int i, flag_ct = 5;

	for (i = 0; i < flag_ct; i++)
	{
		if (flags & flag_set[i])
		{
			flags &= ~flag_set[i];
			printf("%s%s", flag_names[i], flags ? ", " : "\n");
		}
	}
}


/* printf */
/**
 * printFileInfo - `objdump -f` formatted printing of ELF file information
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int printFileInfo(objdump_state *state)
{
	uint32_t flags = setFileFlags(state);

	if (!state)
		return (1);

	printf("\n%s:     file format %s-%s\n",
	       state->f_name,
	       state->ELF_32bit ? "elf32" : "elf64",
	       fileMachString(state));

	printf("architecture: %s, flags 0x%08x:\n",
	       fileArchString(state), flags);

	printFileFlags(flags);

	if (state->ELF_32bit)
		printf("start address 0x%08lx\n\n",
		       state->f_header.e_entry);
	else
		printf("start address 0x%016lx\n\n",
		       state->f_header.e_entry);

	return (0);
}
