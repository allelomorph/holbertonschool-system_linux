#ifndef HOBJDUMP_H
#define HOBJDUMP_H

/* elf.h includes stdint.h */
#include <elf.h>
#include <errno.h>
#include <stdbool.h>
/* FILE stream typedef */
#include <stdio.h>

/**
 * struct objdump_state_s - ELF information needed globally by most subroutines
 * @exec_name: name of hreadelf executable, for error return
 * @f_name: ELF name
 * @f_stream: ELF file stream
 * @f_size: file size, signed due to being derived from off_t
 * @big_endian: endianness of ELF, false by default
 * @ELF_32bit: ELF is for 32 bit architecture, false by default
 * @f_header: file header read from ELF; 64 bit by default
 * @s_headers: section headers read from ELF, 64 bit by default
 * @sh_strtab: section header string table as a single buffer
 *
 * Description: used to hold file data that needs to be globally visible to
 * various functions to ensure consistent error messages and format output.
 * (Note: if ELF is 32 bit, stucts read from file are cast into 64 bit vers.)
 */
typedef struct objdump_state_s
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
} objdump_state;

/* bfd-style file flags */
#define HAS_RELOC                   0x1 /* BFD contains relocation entries. */
#define EXEC_P                      0x2 /* BFD is directly executable.  */
#define HAS_SYMS                   0x10 /* BFD has symbols. */
#define DYNAMIC                    0x40 /* BFD is a dynamic object. */
#define D_PAGED                   0x100 /* BFD is dynamically paged. */

/* 1-hobjdump.c */
/* int main(int argc, char **argv) */

/* hobjdump_main_help.c */
int openELF(objdump_state *state);
void errorMsg(char *format, char *err_str, objdump_state *state);
void initState(objdump_state *state);
void closeState(objdump_state *state);

/* hobjdump_getFileHeader.c */
int getFileHeader(objdump_state *state);
int get32bitFileHeader(objdump_state *state);
int get64bitFileHeader(objdump_state *state);
void bswapElf64_Ehdr(Elf64_Ehdr *ehdr64);
void bswapElf32_Ehdr(Elf32_Ehdr *ehdr32);

/* hobjdump_getSecHeaders.c */
int getSecHeaders(objdump_state *state);
int get64bitSecHeaders(objdump_state *state);
int get32bitSecHeaders(objdump_state *state);
void bswapElf64_Shdr(Elf64_Shdr *shdr64);
void bswapElf32_Shdr(Elf32_Shdr *shdr32);

/* hobjdump_getSecHeadStrTab.c */
int getSecHeadStrTab(objdump_state *state);

/* hobjdump_printFileInfo.c */
char *fileMachString(objdump_state *state);
char *fileArchString(objdump_state *state);
uint32_t setFileFlags(objdump_state *state);
void printFileFlags(uint32_t flags);
int printFileInfo(objdump_state *state);

/* hobjdump_printSections.c */
int printSections(objdump_state *state);
void printSecBuffer(unsigned char *buff, Elf64_Shdr *section);
unsigned char *getSecBuffer(objdump_state *state, Elf64_Shdr *section);

#endif /* HOBJDUMP_H */
