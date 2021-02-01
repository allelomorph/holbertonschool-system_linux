#include "holberton.h"

/* printf putchar */
#include <stdio.h>

/* malloc free */
#include <stdlib.h>


int printSymTables(re_state *state)
{
	Elf64_Shdr *sym_shdr = NULL, *st_shdr = NULL;
	unsigned int i, j, num_syms, num_vers;
	Elf64_Sym *s_tab = NULL, *symbol = NULL;
        char *st_fmt = NULL, *strtab = NULL, *ver_str = NULL;
	Elf64_Versym *sym_vers = NULL;
	Elf64_Vernaux *vernauxs = NULL;

	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		sym_shdr = state->s_headers + i;

		if (/*sym_shdr->sh_type != SHT_SYMTAB
		      &&*/ sym_shdr->sh_type != SHT_DYNSYM)
			continue;

		if (sym_shdr->sh_entsize == 0)
		{
			printf ("\nSymbol table '%s' has a sh_entsize of zero!\n",
			        state->sh_strtab + sym_shdr->sh_name);
			continue;
		}

		num_syms = sym_shdr->sh_size / sym_shdr->sh_entsize;
		printf("\nSymbol table '%s' contains %u %s:\n",
		       state->sh_strtab + sym_shdr->sh_name,
		       num_syms, num_syms > 1 ? "entries" : "entry");

		if (state->ELF_32bit)
		{
			printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
			st_fmt = "%6u: %08lx %5lu %-7s %-6s %-7s %4s %s%s\n";
    		}
		else
		{
			printf("   Num:    Value          Size Type    Bind   Vis      Ndx Name\n");
			st_fmt = "%6u: %016lx %5lu %-7s %-6s %-7s %4s %s%s\n";
		}

		if (sym_shdr->sh_type == SHT_DYNSYM)
		{
			s_tab = state->dyn_sym;
			/* get array of Versym nums */
			sym_vers = getVersyms(state, sym_shdr + 2, num_syms);
			/* get flattened array of vernauxs (Vernaux) */
			vernauxs = getVernauxFlatArr(state, sym_shdr + 3, &num_vers);
		}
		else if (sym_shdr->sh_type == SHT_SYMTAB)
			s_tab = state->sym_tab;
		else
			return (1);

		/* get symbol name string table - always next section ? */
		st_shdr = sym_shdr + 1;

		if (fseek(state->f_stream, st_shdr->sh_offset, SEEK_SET) == -1)
			return (1);

		strtab = malloc(sizeof(char) * (st_shdr->sh_size));
		if (!strtab)
			return (1);

		if (fread(strtab, sizeof(char), st_shdr->sh_size, state->f_stream) != st_shdr->sh_size)
			return (1);

		for (j = 0; j < num_syms; j++)
		{
			symbol = s_tab + j;

			if (sym_shdr->sh_type == SHT_DYNSYM &&
			    (symbol->st_value == 0 &&
			     ELF64_ST_TYPE(symbol->st_info) != STT_NOTYPE))
				ver_str = getSymVerStr(strtab, sym_vers, j, vernauxs, num_vers);
			else
				ver_str = "";

			printf(st_fmt, j,
			       symbol->st_value,
			       symbol->st_size,
			       getSymType(ELF64_ST_TYPE(symbol->st_info)),
			       getSymBinding(state, ELF64_ST_BIND(symbol->st_info)),
			       getSymVisibility(ELF64_ST_VISIBILITY(symbol->st_other)),
			       getSymIdxType(state, symbol->st_shndx),
			       strtab + symbol->st_name,
			       ver_str);
		}

		free(strtab);
		strtab = NULL;

		if(sym_vers)
		{
			free(sym_vers);
			sym_vers = NULL;
		}

		if(vernauxs)
		{
			free(vernauxs);
		        vernauxs = NULL;
		}
	}

	return (0);
}

/*
void printSecToSegMap(re_state *state)
*/

char *getSymVerStr(char *strtab, Elf64_Versym *sym_vers, unsigned int sym_idx, Elf64_Vernaux *versions, unsigned int num_vers)
{
	static char buff[128];
	char *ver_str = NULL;
	Elf64_Vernaux *version = NULL;
	unsigned int i;

	for (i = 0; i < num_vers; i++)
	{
		version = versions + i;
		if (version->vna_other == sym_vers[sym_idx])
		{
			ver_str = strtab + version->vna_name;
			break;
		}
	}

	snprintf(buff, sizeof(buff), "@%s (%u)", ver_str, version->vna_other);
	return buff;
}

/* get array of Versym nums */
Elf64_Versym *getVersyms(re_state *state, Elf64_Shdr *versym_shdr, unsigned int num_syms)
{
	Elf64_Versym *sym_vers = NULL;
	unsigned int i;

	if (versym_shdr->sh_type != SHT_GNU_versym)
		return (NULL);

	if (fseek(state->f_stream, versym_shdr->sh_offset, SEEK_SET) == -1)
		return (NULL);

	/* Elf32_Versym and Elf64_Versym both uint16_t */
	sym_vers = malloc(sizeof(Elf64_Versym) * num_syms);
	if (!sym_vers)
		return (NULL);

	if (fread(sym_vers, sizeof(Elf64_Versym), num_syms, state->f_stream) != num_syms)
		return (NULL);

	if (state->big_endian)
		for (i = 0; i < num_syms; i++)
			sym_vers[i] = __builtin_bswap16(sym_vers[i]);

	return (sym_vers);
}

/* get flattened array of versions (Vernaux) */
Elf64_Vernaux *getVernauxFlatArr(re_state *state, Elf64_Shdr *verneed_shdr, unsigned int *num_vers)
{
	unsigned int vernaux_ct = 0, next_vn, next_vna, i = 0, j;
	Elf64_Verneed *verneed;
	Elf64_Vernaux *vernauxs;

	if (verneed_shdr->sh_type != SHT_GNU_verneed)
		return (NULL);

	/* sizeof(Elf32_Verneed) == sizeof(Elf64_Verneed) */
	verneed = malloc(sizeof(Elf64_Verneed));
	if (!verneed)
		return (NULL);

	/* count Vernaux structs */
	next_vn = verneed_shdr->sh_offset;
	do
	{
		if (fseek(state->f_stream, next_vn, SEEK_SET) == -1)
			return (NULL);

		if (fread(verneed, sizeof(Elf64_Verneed), 1, state->f_stream) != 1)
			return (NULL);

		if (state->big_endian)
			bswapElf64_Verneed(verneed);

		vernaux_ct += verneed->vn_cnt;
		next_vn += verneed->vn_next;
	} while (verneed->vn_next != 0);

	/* sizeof(Elf32_Vernaux) == sizeof(Elf64_Vernaux) */
	vernauxs = malloc(sizeof(Elf64_Vernaux) * vernaux_ct);
	if (!vernauxs)
		return (NULL);
        *num_vers = vernaux_ct;

	/* flatten mixed Verneed/Vernaux section into Vernaux array */
	next_vn = verneed_shdr->sh_offset;
	j = 0;
	do
	{
		if (fseek(state->f_stream, next_vn, SEEK_SET) == -1)
			return (NULL);

		if (fread(verneed, sizeof(Elf64_Verneed), 1, state->f_stream) != 1)
			return (NULL);

		if (state->big_endian)
			bswapElf64_Verneed(verneed);

		next_vna = next_vn + sizeof(Elf64_Verneed);
		for (; i < verneed->vn_cnt && j < vernaux_ct; i++)
		{
			if (fseek(state->f_stream, next_vna, SEEK_SET) == -1)
				return (NULL);

			if (fread(vernauxs + j, sizeof(Elf64_Vernaux),
				  verneed->vn_cnt, state->f_stream) != verneed->vn_cnt)
				return (NULL);

			j += verneed->vn_cnt;
		}

		next_vn += verneed->vn_next;
	} while (verneed->vn_next != 0);

	if (state->big_endian)
	{
		for (i = 0; i < vernaux_ct; i++)
			bswapElf64_Vernaux(vernauxs + i);
	}

	free(verneed);
	return (vernauxs);
}


void bswapElf64_Verneed(Elf64_Verneed *verneed64)
{
	verneed64->vn_version = __builtin_bswap16(verneed64->vn_version);
	verneed64->vn_cnt     = __builtin_bswap16(verneed64->vn_cnt);
	verneed64->vn_file    = __builtin_bswap32(verneed64->vn_file);
	verneed64->vn_aux     = __builtin_bswap32(verneed64->vn_aux);
	verneed64->vn_next    = __builtin_bswap32(verneed64->vn_next);
}


void bswapElf64_Vernaux(Elf64_Vernaux *vernaux64)
{
	vernaux64->vna_hash  = __builtin_bswap32(vernaux64->vna_hash);
	vernaux64->vna_flags = __builtin_bswap16(vernaux64->vna_flags);
	vernaux64->vna_other = __builtin_bswap16(vernaux64->vna_other);
	vernaux64->vna_name  = __builtin_bswap32(vernaux64->vna_name);
	vernaux64->vna_next  = __builtin_bswap32(vernaux64->vna_next);
}
