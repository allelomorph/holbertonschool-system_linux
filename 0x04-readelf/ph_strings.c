#include "holberton.h"

/* snprintf */
#include <stdio.h>

/* malloc */
#include <stdlib.h>

/* strcmp */
#include <string.h>


/* snprintf */
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
/* not in elf.h
	case PT_GNU_PROPERTY: return "GNU_PROPERTY";
	case PT_OPENBSD_RANDOMIZE: return "OPENBSD_RANDOMIZE";
	case PT_OPENBSD_WXNEEDED: return "OPENBSD_WXNEEDED";
	case PT_OPENBSD_BOOTDATA: return "OPENBSD_BOOTDATA";
*/
	default:
/*
		if ((p_type >= PT_LOPROC) && (p_type <= PT_HIPROC))
		{
		* use helper functions to find special seg types for the following machine codes:
		EM_AARCH64 EM_ARM EM_MIPS EM_MIPS_RS3_LE EM_PARISC EM_IA_64 EM_TI_C6000 EM_S390 EM_S390_OLD *
*/
/*
		if ((p_type >= PT_LOOS) && (p_type <= PT_HIOS))
		{
			const char * result = NULL;

			switch (filedata->file_header.e_ident[EI_OSABI])
			{
			case ELFOSABI_GNU:
			case ELFOSABI_FREEBSD:
				if (p_type >= PT_GNU_MBIND_LO && p_type <= PT_GNU_MBIND_HI)
				{
					sprintf (buff, "GNU_MBIND+%#lx", p_type - PT_GNU_MBIND_LO);
					result = buff;
				}
				break;
			case ELFOSABI_HPUX:
				result = get_hpux_segment_type (p_type,
								filedata->file_header.e_machine);
				break;
			case ELFOSABI_SOLARIS:
				result = get_solaris_segment_type (p_type);
				break;
			default:
				break;
			}

			if (result != NULL)
				return result;

			sprintf (buff, "LOOS+%#lx", p_type - PT_LOOS);
		}
		else
*/
		snprintf (buff, sizeof(buff), "<unknown>: %x", p_type);
	}

	return buff;
}


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

#ifdef ZZZ
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
case PT_GNU_PROPERTY: return "GNU_PROPERTY";

case PT_OPENBSD_RANDOMIZE: return "OPENBSD_RANDOMIZE";
case PT_OPENBSD_WXNEEDED: return "OPENBSD_WXNEEDED";
case PT_OPENBSD_BOOTDATA: return "OPENBSD_BOOTDATA";

default:
	if ((p_type >= PT_LOPROC) && (p_type <= PT_HIPROC))
	{
		const char * result;

		switch (filedata->file_header.e_machine)
		{
		case EM_AARCH64:
			result = get_aarch64_segment_type (p_type);
			break;
		case EM_ARM:
			result = get_arm_segment_type (p_type);
			break;
		case EM_MIPS:
		case EM_MIPS_RS3_LE:
			result = get_mips_segment_type (p_type);
			break;
		case EM_PARISC:
			result = get_parisc_segment_type (p_type);
			break;
		case EM_IA_64:
			result = get_ia64_segment_type (p_type);
			break;
		case EM_TI_C6000:
			result = get_tic6x_segment_type (p_type);
			break;
		case EM_S390:
		case EM_S390_OLD:
			result = get_s390_segment_type (p_type);
			break;
		default:
			result = NULL;
			break;
		}

		if (result != NULL)
			return result;

		sprintf (buff, "LOPROC+%#lx", p_type - PT_LOPROC);
	}
	else if ((p_type >= PT_LOOS) && (p_type <= PT_HIOS))
	{
		const char * result = NULL;

		switch (filedata->file_header.e_ident[EI_OSABI])
		{
		case ELFOSABI_GNU:
		case ELFOSABI_FREEBSD:
			if (p_type >= PT_GNU_MBIND_LO && p_type <= PT_GNU_MBIND_HI)
			{
				sprintf (buff, "GNU_MBIND+%#lx", p_type - PT_GNU_MBIND_LO);
				result = buff;
			}
			break;
		case ELFOSABI_HPUX:
			result = get_hpux_segment_type (p_type,
							filedata->file_header.e_machine);
			break;
		case ELFOSABI_SOLARIS:
			result = get_solaris_segment_type (p_type);
			break;
		default:
			break;
		}
		if (result != NULL)
			return result;

		sprintf (buff, "LOOS+%#lx", p_type - PT_LOOS);
	}
	else
		snprintf (buff, sizeof (buff), _("<unknown>: %lx"), p_type);

	return buff;
}
#endif
