#include "hobjdump.h"

#include <string.h>

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

void printFileFlags(uint32_t flags)
{
	uint32_t flag_set[] = {HAS_RELOC, EXEC_P, HAS_SYMS, DYNAMIC, D_PAGED};
	char *flag_names[] = {"HAS_RELOC", "EXEC_P", "HAS_SYMS",
		"DYNAMIC", "D_PAGED"};
	uint32_t i;

	for (i = 0; i < 5; i++)
	{
		if (flags & flag_set[i])
		{
			flags &= ~flag_set[i];
			printf("%s%s", flag_names[i], flags ? ", " : "\n");
		}
	}
}

int printFileInfo(objdump_state *state)
{
	Elf64_Shdr *section = NULL;
	uint32_t flags = setFileFlags(state);
	int i;
	Elf64_Addr start_addr = 0;
	char *ELF_data = NULL;

	if (!state)
		return (1);

	ELF_data = state->ELF_32bit ? "elf32" : "elf64";

	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		section = state->s_headers + i;
		if (strcmp(state->sh_strtab + section->sh_name, ".text") == 0 &&
		    strlen(state->sh_strtab + section->sh_name) == 5)
		{
			start_addr = section->sh_addr;
			break;
		}
	}

	printf("\n%s:     file format %s-%s\n", state->f_name, ELF_data,
	       fileMachString(state));
	printf("architecture: %s, flags 0x%08x:\n",
		fileArchString(state), flags);
	printFileFlags(flags);
	if (state->ELF_32bit)
		printf("start address 0x%08lx\n\n", start_addr);
	else
		printf("start address 0x%016lx\n\n", start_addr);

	return (0);
}
