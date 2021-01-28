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
	int exit_code;
	char *f_name;
	FILE *f_stream;
        int f_size; /* signed due to being derived from off_t */
	bool big_endian; /* false by default */
	bool ELF_32bit; /* false by default */
        Elf64_Ehdr f_header; /* 64 by default, 32 bit values cast into 64 */
	Elf64_Shdr *s_headers; /* 64 by default, 32 bit values cast into 64 */
	Elf64_Phdr *p_headers; /* 64 by default, 32 bit values cast into 64 */
} re_state;

/* printELFHeader.c */
int printELFHeader(re_state *state);

/* f_header_strs.c */
const char *getELFType(Elf64_Half e_type);
const char *getOsabiName(unsigned char osabi);
const char *getMachineName (Elf64_Half e_machine);

/* 0-hreadelf.c */
/* int main(int argc, char **argv) */

/* main_help.c */
int openELFFile(re_state *state);
void errorMsg(char *format, char *err_str, re_state *state);
void initState(re_state *state);
void closeState(re_state *state);

/* getFileHeader.c */
int getFileHeader(re_state *state);
void bswapElf64_Ehdr(Elf64_Ehdr *ehdr64);
void bswapElf32_Ehdr(Elf32_Ehdr *ehdr32);

#endif /* HOLBERTON_H */
