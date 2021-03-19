#include "hnm.h"
/* fseek fread */
#include <stdio.h>
/* malloc free */
#include <stdlib.h>


/**
 * getSymTables - selects function to get symbol tables based on 32 or
 * 64 configuration in ELF
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int getSymTables(nm_state *state)
{
	if (state->ELF_32bit)
		return (get32bitSymTables(state));
	else
		return (get64bitSymTables(state));
}


/* malloc fseek fread */
/**
 * get64bitSymTables - reads ELF and stores symbol tables in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int get64bitSymTables(nm_state *state)
{
	Elf64_Sym *sym_tab = NULL;
	Elf64_Shdr *section = NULL;
	unsigned int i, j, num_sym;

	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		section = state->s_headers + i;
		if (section->sh_type == SHT_SYMTAB)
		{
			state->symtab_sh = section;

			num_sym = section->sh_size / section->sh_entsize;
			sym_tab = malloc(sizeof(Elf64_Sym) * num_sym);
			if (!sym_tab)
				return (1);

			if (fseek(state->f_stream, section->sh_offset,
				  SEEK_SET) == -1)
				return (1);

			if (fread(sym_tab, sizeof(Elf64_Sym), num_sym,
				  state->f_stream) != num_sym)
				return (1);

			if (state->big_endian)
			{
				for (j = 0; j < num_sym; j++)
					bswapElf64_Sym(sym_tab + j);
			}

			/* state->symtab_st 64 bit by default */
			state->symtab_st = sym_tab;
		}
	}

	return (0);
}


/* malloc fseek fread free */
/**
 * get32bitSymTables - reads ELF and stores symbol tables in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int get32bitSymTables(nm_state *state)
{
	Elf32_Sym *sym_tab32 = NULL;
	Elf64_Sym *sym_tab64 = NULL;
	Elf64_Shdr *section = NULL;
	unsigned int i, j, num_sym;

	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		section = state->s_headers + i;
		if (section->sh_type == SHT_SYMTAB)
		{
			state->symtab_sh = section;

			num_sym = section->sh_size / section->sh_entsize;
			sym_tab32 = malloc(sizeof(Elf32_Sym) * num_sym);
			sym_tab64 = malloc(sizeof(Elf64_Sym) * num_sym);
			if (!sym_tab32 || !sym_tab64)
				return (1);

			if (fseek(state->f_stream, section->sh_offset,
				  SEEK_SET) == -1)
				return (1);
			if (fread(sym_tab32, sizeof(Elf32_Sym), num_sym,
				  state->f_stream) != num_sym)
				return (1);

			if (state->big_endian)
			{
				for (j = 0; j < num_sym; j++)
					bswapElf32_Sym(sym_tab32 + j);
			}

			/* state->symtab_st is 64 bit, need to promote values */
			for (j = 0; j < num_sym; j++)
				E32SymToE64Sym(sym_tab32 + j, sym_tab64 + j);
			free(sym_tab32);
			state->symtab_st = sym_tab64;
		}
	}
	return (0);
}


/**
 * bswapElf64_Sym - byte swaps all little endian values in a Elf64_Sym
 * to their big endian versions
 *
 * @sym64: struct to byte swap
 */
void bswapElf64_Sym(Elf64_Sym *sym64)
{
	sym64->st_name   = __builtin_bswap32(sym64->st_name);
	/* st_info is unsigned char - only 1 btye, no swap */
	/* st_other is unsigned char - only 1 btye, no swap */
	sym64->st_shndx  = __builtin_bswap16(sym64->st_shndx);
	sym64->st_value = __builtin_bswap64(sym64->st_value);
	sym64->st_size  = __builtin_bswap64(sym64->st_size);
}


/**
 * bswapElf32_Sym - byte swaps all little endian values in a Elf32_Sym
 * to their big endian versions
 *
 * @sym32: struct to byte swap
 */
void bswapElf32_Sym(Elf32_Sym *sym32)
{
	/* note: Elf32_Sym members in different order than in Elf64_Sym */

	sym32->st_name   = __builtin_bswap32(sym32->st_name);
	sym32->st_value = __builtin_bswap32(sym32->st_value);
	sym32->st_size  = __builtin_bswap32(sym32->st_size);
	/* st_info is unsigned char - only 1 btye, no swap */
	/* st_other is unsigned char - only 1 btye, no swap */
	sym32->st_shndx  = __builtin_bswap16(sym32->st_shndx);

}
