#include "holberton.h"

/* snprintf */
#include <stdio.h>

/* malloc */
#include <stdlib.h>


const char *getSecType(Elf64_Word sh_type)
{
	static char buff[32];

	switch (sh_type)
	{
	case SHT_NULL:return "NULL";
	case SHT_PROGBITS:return "PROGBITS";
	case SHT_SYMTAB:return "SYMTAB";
	case SHT_STRTAB:return "STRTAB";
	case SHT_RELA:return "RELA";
	case SHT_HASH:return "HASH";
	case SHT_DYNAMIC:return "DYNAMIC";
	case SHT_NOTE:return "NOTE";
	case SHT_NOBITS:return "NOBITS";
	case SHT_REL:return "REL";
	case SHT_SHLIB:return "SHLIB";
	case SHT_DYNSYM:return "DYNSYM";
	case SHT_INIT_ARRAY:return "INIT_ARRAY";
	case SHT_FINI_ARRAY:return "FINI_ARRAY";
	case SHT_PREINIT_ARRAY:return "PREINIT_ARRAY";
	case SHT_GNU_HASH:return "GNU_HASH";
	case SHT_GROUP:return "GROUP";
	case SHT_SYMTAB_SHNDX:return "SYMTAB SECTION INDICES";
	case SHT_GNU_verdef:return "VERDEF";
	case SHT_GNU_verneed:return "VERNEED";
	case SHT_GNU_versym:return "VERSYM";
	case 0x6ffffff0:return "VERSYM";
	case 0x6ffffffc:return "VERDEF";
	case 0x7ffffffd:return "AUXILIARY";
	case 0x7fffffff:return "FILTER";
	case SHT_GNU_LIBLIST:return "GNU_LIBLIST";
	default:
		/* This message is probably going to be displayed in a 15
		   character wide field, so put the hex value first.  */
		snprintf(buff, sizeof(buff), "%08x: <unknown>", sh_type);
		return buff;
	}
}


/* trimmmed to remove the following flags not found in elf.h: */
/* SHF_COMPRESSED SHF_X86_64_LARGE SHF_ARM_PURECODE SHF_PPC_VLE SHF_GNU_* */
const char *getSecFlags(Elf64_Xword sh_flags)
{
	static char buff[64];
	char *p = buff;
        Elf64_Xword flag = 0;

	while (sh_flags)
	{
		flag = sh_flags & -sh_flags;
		sh_flags &= ~flag;

		/* unknown flags are skipped without printing */
		switch (flag)
		{
		case SHF_WRITE:*p = 'W'; break;
		case SHF_ALLOC:*p = 'A'; break;
		case SHF_EXECINSTR:*p = 'X'; break;
		case SHF_MERGE:*p = 'M'; break;
		case SHF_STRINGS:*p = 'S'; break;
		case SHF_INFO_LINK:*p = 'I'; break;
		case SHF_LINK_ORDER:*p = 'L'; break;
		case SHF_OS_NONCONFORMING:*p = 'O'; break;
		case SHF_GROUP:*p = 'G'; break;
		case SHF_TLS:*p = 'T'; break;
		case SHF_EXCLUDE:*p = 'E'; break;
		default:
			if (flag & SHF_MASKOS)
			{
				*p = 'o';
				sh_flags &= ~SHF_MASKOS;
			}
			else if (flag & SHF_MASKPROC)
			{
				*p = 'p';
				sh_flags &= ~SHF_MASKPROC;
			}
			else
				*p = 'x';
			break;
		}
		p++;
	}
	*p = '\0';

	return buff;
}


int getSHStrTab(re_state *state)
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
