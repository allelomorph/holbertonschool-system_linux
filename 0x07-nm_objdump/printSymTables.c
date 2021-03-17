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
int printSymTables(re_state *state)
{
	Elf64_Shdr *sym_shdr = state->s_headers, *st_shdr = NULL;
	Elf64_Sym *symbol = NULL;
        char *fmt = NULL, *empty_fmt = NULL, *strtab = NULL;
	unsigned int i;

	for (i = 0; i < state->f_header.e_shnum && sym_shdr &&
		    sym_shdr->sh_type != SHT_SYMTAB; i++)
		sym_shdr++;

	if (i == state->f_header.e_shnum || !sym_shdr ||
	    (sym_shdr && sym_shdr->sh_entsize == 0))
	{
		errorMsg("%s: no symbols\n", NULL, state);
		return (0);
	}

	fmt = state->ELF_32bit ? "%08x %c %s\n" : "%016x %c %s\n";
	empty_fmt = state->ELF_32bit ? "         %c %s\n" :
				       "                 %c %s\n";

	/* get symbol name string table - always next section ? */
	st_shdr = sym_shdr + 1;

	if (fseek(state->f_stream, st_shdr->sh_offset, SEEK_SET) == -1)
		return (1);

	strtab = malloc(sizeof(char) * (st_shdr->sh_size));
	if (!strtab)
		return (1);

	if (fread(strtab, sizeof(char), st_shdr->sh_size,
		  state->f_stream) != st_shdr->sh_size)
		return (1);

	for (i = 1; i < sym_shdr->sh_size / sym_shdr->sh_entsize; i++)
	{
		symbol = state->sym_tab + i;

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

char getSymNMType(re_state *state, Elf64_Sym *symbol)
{
	Elf64_Shdr *section = NULL;

	/* A: symbol is absolute (Ndx of ABS); 'a' not used */
	if (symbol->st_shndx == SHN_ABS)
		return ('A');
	/* C: symbol is common (uninitialized data) (Ndx of COM); 'c' not used */
	if (symbol->st_shndx == SHN_COMMON)
		return ('C');

	if (symbol->st_shndx == SHN_UNDEF)
	{
		if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK)
		{
			/* vV: symbol is a weak object (Bind:WEAK readelf-type:OBJECT); */
			/* (uppercase indicates that a default value has been specified (Ndx not UND?) */
			if (ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT)
				return ('v');

			/* wW: weak symbol that has not been specifically tagged as a weak object symbol (Bind:WEAK readelf-type != OBJECT); */
			/* (w == Ndx UND? or readelf-type of NOTYPE?) */
			return ('w');
		}
		/* U: symbol is undefined (Ndx of UND, Bind != WEAK); */
		return ('U');
	}

	if (ELF64_ST_BIND(symbol->st_info) == STB_WEAK)
	{
		/* vV: symbol is a weak object (Bind:WEAK readelf-type:OBJECT); */
		/* (uppercase indicates that a default value has been specified (Ndx not UND?) */
		if (ELF64_ST_TYPE(symbol->st_info) == STT_OBJECT)
			return ('V');
		/* wW: weak symbol that has not been specifically tagged as a weak object symbol (Bind:WEAK readelf-type != OBJECT); */
			/* (w == Ndx UND? or readelf-type of NOTYPE?) */
			/* (W == Ndx not UND? or readelf-type of not NOTYPE?) */
		return ('W');
	}

	section = state->s_headers + symbol->st_shndx;
	/* bB: symbol is in the uninitialized data section (known as BSS) flags WA type NOBITS */
		/* b == Bind:LOCAL B == Bind:GLOBAL */
	if ((section->sh_flags & SHF_WRITE) == SHF_WRITE &&
	    (section->sh_flags & SHF_ALLOC) == SHF_ALLOC &&
	    section->sh_type == SHT_NOBITS)
	    	return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL ? 'b' : 'B');
	/* dD: symbol is in the initialized data section; flag WA and type PROGBITS or DYNAMIC */
		/* d ==Bind:LOCAL D == Bind:GLOBAL */
	if ((section->sh_flags & SHF_WRITE) == SHF_WRITE &&
	    (section->sh_flags & SHF_ALLOC) == SHF_ALLOC &&
	    (section->sh_type == SHT_PROGBITS || section->sh_type == SHT_DYNAMIC))
	    	return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL ? 'd' : 'D');
	/* rR: symbol is in a read only data section; flags no X, no W type PROGBITS */
		/* r == Bind:LOCAL R == Bind:GLOBAL */
	if ((section->sh_flags & SHF_WRITE) != SHF_WRITE &&
	    (section->sh_flags & SHF_EXECINSTR) != SHF_EXECINSTR &&
	    section->sh_type == SHT_PROGBITS)
	    	return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL ? 'r' : 'R');
	/* tT: symbol in text (code) section; flag X and type PROGBITS or type INIT_ARRAY or type FINI_ARRAY */
		/* t ==Bind:LOCAL T == Bind:GLOBAL */
	if (section->sh_type == SHT_INIT_ARRAY ||
	    section->sh_type == SHT_FINI_ARRAY ||
	    ((section->sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR &&
	     section->sh_type == SHT_PROGBITS))
	    	return (ELF32_ST_BIND(symbol->st_info) == STB_LOCAL ? 't' : 'T');

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
