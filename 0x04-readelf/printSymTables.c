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

		if (sym_shdr->sh_type != SHT_SYMTAB
		    && sym_shdr->sh_type != SHT_DYNSYM)
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
