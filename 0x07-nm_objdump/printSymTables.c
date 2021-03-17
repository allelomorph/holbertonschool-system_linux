#include "hnm.h"

/* printf putchar */
#include <stdio.h>

/* malloc free */
#include <stdlib.h>


/**
 * printSymTables - formatted printing of symbol tables stored in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int printSymTables(nm_state *state)
{
	Elf64_Shdr *symtab_sh = state->symtab_sh, *st_shdr = NULL;
	Elf64_Sym *symbol = NULL;
        char *fmt = NULL, *empty_fmt = NULL, *strtab = NULL;
	unsigned int i;

	if (!symtab_sh || (symtab_sh && symtab_sh->sh_entsize == 0))
	{
		errorMsg("%s: no symbols\n", NULL, state);
		return (0);
	}
	fmt = state->ELF_32bit ? "%08x %c %s\n" : "%016x %c %s\n";
	empty_fmt = state->ELF_32bit ? "         %c %s\n" :
				       "                 %c %s\n";

	/* get symbol name string table - always next section ? */
	st_shdr = symtab_sh + 1;
	if (fseek(state->f_stream, st_shdr->sh_offset, SEEK_SET) == -1)
		return (1);
	strtab = malloc(sizeof(char) * (st_shdr->sh_size));
	if (!strtab)
		return (1);
	if (fread(strtab, sizeof(char), st_shdr->sh_size,
		  state->f_stream) != st_shdr->sh_size)
		return (1);

	for (i = 1; i < symtab_sh->sh_size / symtab_sh->sh_entsize; i++)
	{
		symbol = state->symtab_st + i;
		if (ELF64_ST_TYPE(symbol->st_info) != STT_SECTION &&
		    ELF64_ST_TYPE(symbol->st_info) != STT_FILE)
		{
			if (symbol->st_shndx == SHN_UNDEF)
				printf(empty_fmt,
					getSymNMType(state, symbol),
				       strtab + symbol->st_name);
			else
				printf(fmt, symbol->st_value,
					getSymNMType(state, symbol),
				       strtab + symbol->st_name);
		}
	}

	free(strtab);
	return (0);
}

char getSymNMType(nm_state *state, Elf64_Sym *symbol)
{
	if (!state || !symbol)
		return ('?');

	/* A: symbol is absolute; 'a' not used */
	if (symbol->st_shndx == SHN_ABS)
		return ('A');

	/* C: symbol is common (uninitialized data); 'c' not used */
	if (symbol->st_shndx == SHN_COMMON)
		return ('C');

	if (symbol->st_shndx == SHN_UNDEF)
	{
		if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK)
		{
			/* v: symbol is a weak object */
			if (ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT)
				return ('v');
			/* w: weak symbol not tagged as a weak object */
			return ('w');
		}
		/* U: symbol is undefined */
		return ('U');
	}

	if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK)
	{
		/* V: symbol is a weak object */
		if (ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT)
			return ('V');
		/* W: weak symbol not tagged as a weak object */
		return ('W');
	}

	return (getSymNMTypeBySec(state->s_headers + symbol->st_shndx,
				       symbol));
}


char getSymNMTypeBySec(Elf64_Shdr *section, Elf64_Sym *symbol)
{
	if (!section || !symbol)
		return ('?');

	if ((section->sh_flags & SHF_WRITE) == SHF_WRITE &&
	     section->sh_type == SHT_NOBITS)
	{	/* bB: symbol in uninitialized data section (known as BSS) */
		if (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL)
			return ('b');
		if (ELF32_ST_BIND(symbol->st_info) == STB_GLOBAL)
			return ('B');
	}
    	if ((section->sh_flags & SHF_WRITE) == SHF_WRITE &&
	    (section->sh_type == SHT_PROGBITS ||
	     section->sh_type == SHT_DYNAMIC))
    	{	/* dD: symbol in initialized data section */
		if (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL)
			return ('d');
		if (ELF32_ST_BIND(symbol->st_info) == STB_GLOBAL)
			return ('D');
	}
    	if ((section->sh_flags & SHF_WRITE) != SHF_WRITE &&
	    (section->sh_flags & SHF_EXECINSTR) != SHF_EXECINSTR &&
	    section->sh_type == SHT_PROGBITS)
	{	/* rR: symbol in read only data section */
		if (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL)
			return ('r');
		if (ELF32_ST_BIND(symbol->st_info) == STB_GLOBAL)
			return ('R');
	}
	if (section->sh_type == SHT_INIT_ARRAY ||
    	    section->sh_type == SHT_FINI_ARRAY ||
    	    ((section->sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR &&
	     section->sh_type == SHT_PROGBITS))
	{ 	/* tT: symbol in text (code) section */
		if (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL)
			return ('t');
		if (ELF32_ST_BIND(symbol->st_info) == STB_GLOBAL)
			return ('T');
	}
	return ('?');
}

/*
	switch (symbol->st_shndx)
	{
	case SHN_UNDEF:return "UND";
	case SHN_ABS:return "ABS";
	case SHN_COMMON:return "COM";
	default:
		// SHN_IA_64_ANSI_COMMON SHN_X86_64_LCOMMON SHN_MIPS_SCOMMON SHN_MIPS_SUNDEFINED not in elf.h
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
*/


/*
	switch (ELF64_ST_TYPE(symbol->st_info))
	{
	case STT_NOTYPE:return "NOTYPE";
	case STT_OBJECT:return "OBJECT";
	case STT_FUNC:return "FUNC";
	case STT_SECTION:return "SECTION";
	case STT_FILE:return "FILE";
	case STT_COMMON:return "COMMON";
	case STT_TLS:return "TLS";
	//STT_RELC STT_SRELC not in elf.h
	default:
		if (type >= STT_LOPROC && type <= STT_HIPROC)
			// STT_ARM_TFUNC STT_REGISTER STT_PARISC_MILLI not in elf.h
			snprintf(buff, sizeof(buff), "<processor specific>: %u", type);
		// STT_LOOS STT_HIOS not in elf.h
		else
			snprintf(buff, sizeof(buff), "<unknown>: %u", type);
		return (buff);
	}
*/

/*
	switch (ELF64_ST_BIND(symbol->st_info))
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
*/
