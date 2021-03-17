#ifndef HNM_H
#define HNM_H

/* elf.h includes stdint.h */
#include <elf.h>
#include <errno.h>
#include <stdbool.h>

/* FILE stream type def */
#include <stdio.h>

/**
 * struct nm_state - ELF information needed globally by most subroutines
 * @exec_name: name of hreadelf executable, for error return
 * @f_name: ELF name
 * @f_stream: ELF file stream
 * @f_size: file size, signed due to being derived from off_t
 * @big_endian: endianness of ELF, false by default
 * @ELF_32bit: ELF is for 32 bit architecture, false by default
 * @f_header: file header read from ELF; 64 bit by default
 * @s_headers: section headers read from ELF, 64 bit by default
 * @sh_strtab: full section header string table as one buffer
 * @p_headers: program headers (segments) read from ELF, 64 bit by default
 * @symtab_sh: section header from .symtab section, if present
 * @symtab_st: symbol table from .symtab section, if present
 *
 * Description: used to hold file data that needs to be globally visible to
 * various functions to ensure consistent error messages and format output.
 * (Note: if ELF is 32 bit, stucts read from file are cast into 64 bit vers.)
 */
typedef struct nm_state_s
{
	char *exec_name;
	char *f_name;
	FILE *f_stream;
	int f_size;
	bool big_endian;
	bool ELF_32bit;
	Elf64_Ehdr f_header;
	Elf64_Shdr *s_headers;
	char *sh_strtab;
	Elf64_Phdr *p_headers;
	Elf64_Shdr *symtab_sh;
 	Elf64_Sym *symtab_st;
} nm_state;


/* 100-hreadelf.c */
/* int main(int argc, char **argv) */

/* main_help.c */
int openELF(nm_state *state);
void errorMsg(char *format, char *err_str, nm_state *state);
void initState(nm_state *state);
void closeState(nm_state *state);

/* getFileHeader.c */
int getFileHeader(nm_state *state);
int get32bitFileHeader(nm_state *state);
int get64bitFileHeader(nm_state *state);
void bswapElf64_Ehdr(Elf64_Ehdr *ehdr64);
void bswapElf32_Ehdr(Elf32_Ehdr *ehdr32);

/* getSecHeaders.c */
int getSecHeaders(nm_state *state);
int get64bitSecHeaders(nm_state *state);
int get32bitSecHeaders(nm_state *state);
void bswapElf64_Shdr(Elf64_Shdr *shdr64);
void bswapElf32_Shdr(Elf32_Shdr *shdr32);

/* getSecHeadStrTab.c */
int getSecHeadStrTab(nm_state *state);

/* getSymTables.c */
int getSymTables(nm_state *state);
int get64bitSymTables(nm_state *state);
int get32bitSymTables(nm_state *state);
void bswapElf64_Sym(Elf64_Sym *sym64);
void bswapElf32_Sym(Elf32_Sym *sym32);

/* st_strings.c */
const char *getSymVisibility(unsigned int visibility);
const char *getSymIdxType(nm_state *state, unsigned int type);
const char *getSymType(unsigned int type);
const char *getSymBinding(nm_state *state, unsigned int binding);

/* get_sym_vers.c */
char *getSymVerStr(char *strtab, Elf64_Versym *sym_vers, unsigned int sym_idx,
				   Elf64_Vernaux *versions, unsigned int num_vers);
Elf64_Versym *getVersyms(nm_state *state, Elf64_Shdr *versym_shdr,
						 unsigned int num_syms);
Elf64_Vernaux *getVernauxFlatArr(nm_state *state, Elf64_Shdr *verneed_shdr,
								 unsigned int *num_vers);
void bswapElf64_Verneed(Elf64_Verneed *verneed64);
void bswapElf64_Vernaux(Elf64_Vernaux *vernaux64);

/* printSymTables.c */
int printSymTables(nm_state *state);
char getSymNMType(nm_state *state, Elf64_Sym *symbol);
char getSymNMTypeBySec(Elf64_Shdr *section, Elf64_Sym *symbol);

#endif /* HNM_H */
