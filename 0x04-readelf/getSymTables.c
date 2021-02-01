#include "holberton.h"

/* fseek fread */
#include <stdio.h>

/* malloc free */
#include <stdlib.h>


int getSymTables(re_state *state)
{
	if (state->ELF_32bit)
		return (get32bitSymTables(state));
	else
		return (get64bitSymTables(state));
}


/* malloc lseek fread */
int get64bitSymTables(re_state *state)
{
        Elf64_Sym *sym_tab = NULL;
	Elf64_Shdr section;
	unsigned int i, j, num_sym;

	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		section = state->s_headers[i];
		if (section.sh_type == SHT_DYNSYM ||
		    section.sh_type == SHT_SYMTAB)
		{
			num_sym = section.sh_size / section.sh_entsize;
			sym_tab = malloc(sizeof(Elf64_Sym) * num_sym);
			if (!sym_tab)
				return (1);

			if (fseek(state->f_stream, section.sh_offset, SEEK_SET) == -1)
				return (1);

			if (fread(sym_tab, sizeof(Elf64_Sym), num_sym,
				  state->f_stream) != num_sym)
				return (1);

			if (state->big_endian)
			{
				for (j = 0; j < num_sym; j++)
					bswapElf64_Sym(sym_tab + j);
			}

			/* state->dyn_sym and ->sym_tab 64 bit by default */
			if (section.sh_type == SHT_DYNSYM)
				state->dyn_sym = sym_tab;
			else
				state->sym_tab = sym_tab;
		}
	}

	return (0);
}

/* malloc lseek fread free */
int get32bitSymTables(re_state *state)
{
        Elf32_Sym *sym_tab32 = NULL, *curr32 = NULL;
        Elf64_Sym *sym_tab64 = NULL, *curr64 = NULL;
	Elf64_Shdr section;
	unsigned int i, j, num_sym;

	for (i = 0; i < state->f_header.e_shnum; i++)
	{
		section = state->s_headers[i];
		if (section.sh_type == SHT_DYNSYM ||
		    section.sh_type == SHT_SYMTAB)
		{
			num_sym = section.sh_size / section.sh_entsize;

			sym_tab32 = malloc(sizeof(Elf32_Sym) * num_sym);
			sym_tab64 = malloc(sizeof(Elf64_Sym) * num_sym);
			if (!sym_tab32 || !sym_tab64)
				return (1);

			if (fseek(state->f_stream, section.sh_offset, SEEK_SET) == -1)
				return (1);

			if (fread(sym_tab32, sizeof(Elf32_Sym), num_sym,
				  state->f_stream) != num_sym)
				return (1);

			if (state->big_endian)
			{
				for (j = 0; j < num_sym; j++)
					bswapElf32_Sym(sym_tab32 + j);
			}

			/* state->dyn_sym and ->sym_tab 64 bit by default, need to promote values */
			for (j = 0; j < num_sym; j++)
			{
				curr32 = sym_tab32 + j;
				curr64 = sym_tab64 + j;
				curr64->st_name  = (Elf64_Word)curr32->st_name;
				/* 64 and 32 both unsigned char */
				curr64->st_info  = curr32->st_info;
				/* 64 and 32 both unsigned char */
				curr64->st_other = curr32->st_other;
				curr64->st_shndx = (Elf64_Section)curr32->st_shndx;
				curr64->st_value = (Elf64_Addr)curr32->st_value;
				curr64->st_size  = (Elf64_Xword)curr32->st_size;
			}

			free(sym_tab32);
			sym_tab32 = NULL;
			if (section.sh_type == SHT_DYNSYM)
				state->dyn_sym = sym_tab64;
			else
				state->sym_tab = sym_tab64;
		}
	}

	return (0);
}

void bswapElf64_Sym(Elf64_Sym *sym64)
{
	sym64->st_name   = __builtin_bswap32(sym64->st_name);
	/* st_info is unsigned char - only 1 btye, no swap */
	/* st_other is unsigned char - only 1 btye, no swap */
	sym64->st_shndx  = __builtin_bswap16(sym64->st_shndx);
	sym64->st_value = __builtin_bswap64(sym64->st_value);
	sym64->st_size  = __builtin_bswap64(sym64->st_size);
}

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
