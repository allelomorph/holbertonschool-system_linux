#include "holberton.h"

/* snprintf fseek fread */
#include <stdio.h>

/* malloc free */
#include <stdlib.h>


/**
 * getSymVerStr - formats a string containing the version of a dynamic symbol
 *
 * @strtab: symbol string table
 * @sym_vers: array of Elf64_Versym in same order as symbol table
 * @sym_idx: index of symbol in table and sym_vers
 * @versions: array of Elf64_Vernaux containing symbol version information
 * @num_vers: amount of possible versions
 * Return: NULL on failure, formatted symbol version string on success
 */
char *getSymVerStr(char *strtab, Elf64_Versym *sym_vers, unsigned int sym_idx,
		   Elf64_Vernaux *versions, unsigned int num_vers)
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
/**
 * printSecHeaders - reads ELF to return an array of version numbers for
 * dynamic symbols
 *
 * @state: struct containing file data and info for error printing
 * @versym_shdr: section header of type SHT_VERSYM
 * @num_syms: total symbols in table
 * Return: NULL on failure, on success an array of Elf64_Versym containing
 * dynamic symbol version numbers
 */
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

/**
 * getVernauxFlatArr - reads ELF to return an array of versions for dynamic
 * symbols. Normally interleaved with verneed structs, the array is "flattened"
 * to only include the vernaux structs
 *
 * @state: struct containing file data and info for error printing
 * @verneed_shdr: section header of type SHT_VERNEED
 * @num_vers: total amount of vernaux found
 * Return: NULL on failure, on success an array of Elf64_Vernaux containing
 * dynamic symbol versions
 */
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


/**
 * bswapElf64_Verneed - byte swaps all little endian values in a Elf64_Verneed
 * to their big endian versions (no 32 bit equivalent as both versions of the
 * struct have the same total size)
 *
 * @verneed64: struct to byte swap
 */
void bswapElf64_Verneed(Elf64_Verneed *verneed64)
{
	verneed64->vn_version = __builtin_bswap16(verneed64->vn_version);
	verneed64->vn_cnt     = __builtin_bswap16(verneed64->vn_cnt);
	verneed64->vn_file    = __builtin_bswap32(verneed64->vn_file);
	verneed64->vn_aux     = __builtin_bswap32(verneed64->vn_aux);
	verneed64->vn_next    = __builtin_bswap32(verneed64->vn_next);
}


/**
 * bswapElf64_Vernaux - byte swaps all little endian values in a Elf64_Vernaux
 * to their big endian versions (no 32 bit equivalent as both versions of the
 * struct have the same total size)
 *
 * @vernaux64: struct to byte swap
 */
void bswapElf64_Vernaux(Elf64_Vernaux *vernaux64)
{
	vernaux64->vna_hash  = __builtin_bswap32(vernaux64->vna_hash);
	vernaux64->vna_flags = __builtin_bswap16(vernaux64->vna_flags);
	vernaux64->vna_other = __builtin_bswap16(vernaux64->vna_other);
	vernaux64->vna_name  = __builtin_bswap32(vernaux64->vna_name);
	vernaux64->vna_next  = __builtin_bswap32(vernaux64->vna_next);
}
