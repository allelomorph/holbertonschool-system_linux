#include "holberton.h"


/**
 * getSymVisibility - converts symbol visibility code to formatted string
 *
 * @visibility: visibility type code from symbol table
 * Return: string containing visibility type
 */
const char *getSymVisibility(unsigned int visibility)
{
	switch (visibility)
	{
	case STV_DEFAULT:return "DEFAULT";
	case STV_INTERNAL:return "INTERNAL";
	case STV_HIDDEN:return "HIDDEN";
	case STV_PROTECTED: return "PROTECTED";
	default: return "<unknown>";
	}
}

/**
 * getSymIdxType - converts symbol index code to formatted string
 *
 * @state: struct containing file data and info for error printing
 * @type: index code from symbol table
 * Return: string containing index type
 */
const char *getSymIdxType(re_state *state, unsigned int type)
{
	static char buff[32];

	switch (type)
	{
	case SHN_UNDEF:return "UND";
	case SHN_ABS:return "ABS";
	case SHN_COMMON:return "COM";
	default:
		/* SHN_IA_64_ANSI_COMMON SHN_X86_64_LCOMMON SHN_MIPS_SCOMMON SHN_MIPS_SUNDEFINED not in elf.h */
		if (type >= SHN_LOPROC && type <= SHN_HIPROC)
			sprintf(buff, "PRC[0x%04x]", type & 0xffff);
		else if (type >= SHN_LOOS && type <= SHN_HIOS)
			sprintf(buff, "OS [0x%04x]", type & 0xffff);
		else if (type >= SHN_LORESERVE)
			sprintf(buff, "RSV[0x%04x]", type & 0xffff);
		else if (state->f_header.e_shnum != 0
			 && type >= state->f_header.e_shnum)
			sprintf(buff, "bad section index[%3u]", type);
		else
			sprintf(buff, "%3u", type);
		return (buff);
	}
}


/**
 * getSymType - converts symbol type code to formatted string
 *
 * @type: type code from symbol table
 * Return: string containing symbol type
 */
const char *getSymType(unsigned int type)
{
	static char buff[64];

	switch (type)
	{
	case STT_NOTYPE:return "NOTYPE";
	case STT_OBJECT:return "OBJECT";
	case STT_FUNC:return "FUNC";
	case STT_SECTION:return "SECTION";
	case STT_FILE:return "FILE";
	case STT_COMMON:return "COMMON";
	case STT_TLS:return "TLS";
/* STT_RELC STT_SRELC not in elf.h */
	default:
		if (type >= STT_LOPROC && type <= STT_HIPROC)
			/* STT_ARM_TFUNC STT_REGISTER STT_PARISC_MILLI not in elf.h */
			snprintf(buff, sizeof(buff), "<processor specific>: %u", type);
		/* STT_LOOS STT_HIOS not in elf.h */
		else
			snprintf(buff, sizeof(buff), "<unknown>: %u", type);
		return (buff);
	}
}

/**
 * getSymBinding - converts symbol binding code to formatted string
 *
 * @state: struct containing file data and info for error printing
 * @binding: binding code from symbol table
 * Return: string containing binding type
 */
const char *getSymBinding(re_state *state, unsigned int binding)
{
	static char buff[64];

	switch (binding)
	{
	case STB_LOCAL:return "LOCAL";
	case STB_GLOBAL:return "GLOBAL";
	case STB_WEAK:return "WEAK";
	default:
		if (binding >= STB_LOPROC && binding <= STB_HIPROC)
			snprintf(buff, sizeof(buff),
				 "<processor specific>: %u", binding);
		else if (binding >= STB_LOOS && binding <= STB_HIOS)
		{
			if (binding == STB_GNU_UNIQUE
			    && state->f_header.e_ident[EI_OSABI] == ELFOSABI_GNU)
				return ("UNIQUE");
			snprintf(buff, sizeof(buff),
				 "<OS specific>: %u", binding);
		}
		else
			snprintf(buff, sizeof(buff),
				 "<unknown>: %u", binding);
		return (buff);
	}
}
