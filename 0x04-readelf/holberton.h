#ifndef HOLBERTON_H
#define HOLBERTON_H

/* elf.h includes stdint.h */
#include <elf.h>
#include <errno.h>
#include <stdbool.h>
/* FILE stream type def */
#include <stdio.h>

typedef struct readelf_state {
        char *exec_name;
	char *f_name;
	FILE *f_stream;
        int f_size; /* signed due to being derived from off_t */
	bool big_endian; /* false by default */
	bool ELF_32bit; /* false by default */
        Elf64_Ehdr f_header; /* 64 by default, 32 bit values cast into 64 */
	Elf64_Shdr *s_headers; /* 64 by default, 32 bit values cast into 64 */
        char *sh_strtab; /* full section header string table as one buffer */
	char *prog_interp; /* contents of .interp section, if present */
	Elf64_Phdr *p_headers; /* 64 by default, 32 bit values cast into 64 */
	Elf64_Sym *dyn_sym; /* from .dynsym section, if present */
	Elf64_Sym *sym_tab; /* from .symtab section, if present */
} re_state;


/* files shared by all versions: */
/* main_help.c */
int openELFFile(re_state *state);
void errorMsg(char *format, char *err_str, re_state *state);
void initState(re_state *state);
void closeState(re_state *state);

/* getFileHeader.c */
int getFileHeader(re_state *state);
void bswapElf64_Ehdr(Elf64_Ehdr *ehdr64);
void bswapElf32_Ehdr(Elf32_Ehdr *ehdr32);


/* 0-hreadelf: */
/* printELFHeader.c */
int printELFHeader(re_state *state);

/* fh_strings.c */
const char *getELFType(Elf64_Half e_type);
const char *getOsabiName(unsigned char osabi);
const char *getMachineName (Elf64_Half e_machine);

/* 0-hreadelf.c */
/* int main(int argc, char **argv) */


/* 1-hreadelf and 2-hreadelf: */
/* getSecHeaders.c */
int getSecHeaders(re_state *state);
int get64bitSecHeaders(re_state *state);
int get32bitSecHeaders(re_state *state);
void bswapElf64_Shdr(Elf64_Shdr *shdr64);
void bswapElf32_Shdr(Elf32_Shdr *shdr32);


/* 1-hreadelf: */
/* printSecHeaders.c */
int printSecHeaders(re_state *state);

/* sh_strings.c */
const char *getSecType(Elf64_Word sh_type);
const char *getSecFlags(Elf64_Xword sh_flags);
int getSHStrTab(re_state *state);

/* 1-hreadelf.c */
/* int main(int argc, char **argv) */


/* 2-hreadelf: */
/* getProgHeaders.c */
int getProgHeaders(re_state *state);
int get64bitProgHeaders(re_state *state);
int get32bitProgHeaders(re_state *state);
void bswapElf64_Phdr(Elf64_Phdr *phdr64);
void bswapElf32_Phdr(Elf32_Phdr *phdr32);

/* ph_strings.c */
const char *getSegType(Elf64_Word p_type);
int getProgInterp(re_state *state);

/* printProgHeaders.c */
int printProgHeaders(re_state *state);

/* 2-hreadelf.c */
/* int main(int argc, char **argv) */


/* 100-hreadelf: */
/* getSymTables.c */
int getSymTables(re_state *state);
int get64bitSymTables(re_state *state);
int get32bitSymTables(re_state *state);
void bswapElf64_Sym(Elf64_Sym *sym64);
void bswapElf32_Sym(Elf32_Sym *sym32);

/* st_strings.c */
const char *getSymVisibility(unsigned int visibility);
const char *getSymIdxType(re_state *state, unsigned int type);
const char *getSymType(unsigned int type);
const char *getSymBinding(re_state *state, unsigned int binding);

/* printSymTables.c */
int printSymTables(re_state *state);

char *getSymVerStr(char *strtab, Elf64_Versym *sym_vers, unsigned int sym_idx, Elf64_Vernaux *versions, unsigned int num_vers);
Elf64_Versym *getVersyms(re_state *state, Elf64_Shdr *versym_shdr, unsigned int num_syms);
Elf64_Vernaux *getVernauxFlatArr(re_state *state, Elf64_Shdr *verneed_shdr, unsigned int *num_vers);
void bswapElf64_Verneed(Elf64_Verneed *verneed64);
void bswapElf64_Vernaux(Elf64_Vernaux *vernaux64);


/* 100-hreadelf.c */
/* int main(int argc, char **argv) */


#endif /* HOLBERTON_H */
