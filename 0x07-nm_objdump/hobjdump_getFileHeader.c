#include "hobjdump.h"
/* fread */
#include <stdio.h>


/* fread */
/**
 * getFileHeader - reads ELF and stores file header in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int getFileHeader(objdump_state *state)
{
	/* read full identity array */
	if (fread(state->f_header.e_ident, EI_NIDENT, 1, state->f_stream) != 1)
		return (1);

	/* Determine how to read the rest of the header.  */
	if (state->f_header.e_ident[EI_DATA] == ELFDATA2MSB)
		state->big_endian = true;

	/* For now we only support 32 bit and 64 bit ELF files.  */
	if (state->f_header.e_ident[EI_CLASS] != ELFCLASS64)
		state->ELF_32bit = true;

	/* Read in the rest of the header. */
	if (state->ELF_32bit)
		return (get32bitFileHeader(state));

	return (get64bitFileHeader(state));
}


/* fread */
/**
 * get64bitFileHeader - reads ELF and stores file header in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int get64bitFileHeader(objdump_state *state)
{
	Elf64_Ehdr ehdr64;

	if (fread(&(ehdr64.e_type), (sizeof(ehdr64) - EI_NIDENT),
		  1, state->f_stream) != 1)
		return (1);

	if (state->big_endian)
		bswapElf64_Ehdr(&ehdr64);

	/* state->f_header 64 bit by default */
	state->f_header.e_type      = ehdr64.e_type;
	state->f_header.e_machine   = ehdr64.e_machine;
	state->f_header.e_version   = ehdr64.e_version;
	state->f_header.e_entry     = ehdr64.e_entry;
	state->f_header.e_phoff     = ehdr64.e_phoff;
	state->f_header.e_shoff     = ehdr64.e_shoff;
	state->f_header.e_flags     = ehdr64.e_flags;
	state->f_header.e_ehsize    = ehdr64.e_ehsize;
	state->f_header.e_phentsize = ehdr64.e_phentsize;
	state->f_header.e_phnum     = ehdr64.e_phnum;
	state->f_header.e_shentsize = ehdr64.e_shentsize;
	state->f_header.e_shnum     = ehdr64.e_shnum;
	state->f_header.e_shstrndx  = ehdr64.e_shstrndx;

	return (0);
}


/* fread */
/**
 * get32bitFileHeader - reads ELF and stores file headers in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int get32bitFileHeader(objdump_state *state)
{
	Elf32_Ehdr ehdr32;

	if (fread(&(ehdr32.e_type), (sizeof(ehdr32) - EI_NIDENT),
		  1, state->f_stream) != 1)
		return (1);

	if (state->big_endian)
		bswapElf32_Ehdr(&ehdr32);

	/* state->f_header 64 bit by default */
	state->f_header.e_type      = (Elf64_Half)ehdr32.e_type;
	state->f_header.e_machine   = (Elf64_Half)ehdr32.e_machine;
	state->f_header.e_version   = (Elf64_Word)ehdr32.e_version;
	state->f_header.e_entry     = (Elf64_Addr)ehdr32.e_entry;
	state->f_header.e_phoff     = (Elf64_Off)ehdr32.e_phoff;
	state->f_header.e_shoff     = (Elf64_Off)ehdr32.e_shoff;
	state->f_header.e_flags     = (Elf64_Word)ehdr32.e_flags;
	state->f_header.e_ehsize    = (Elf64_Half)ehdr32.e_ehsize;
	state->f_header.e_phentsize = (Elf64_Half)ehdr32.e_phentsize;
	state->f_header.e_phnum     = (Elf64_Half)ehdr32.e_phnum;
	state->f_header.e_shentsize = (Elf64_Half)ehdr32.e_shentsize;
	state->f_header.e_shnum     = (Elf64_Half)ehdr32.e_shnum;
	state->f_header.e_shstrndx  = (Elf64_Half)ehdr32.e_shstrndx;

	return (0);
}


/**
 * bswapElf64_Ehdr - byte swaps all little endian values in a Elf64_Ehdr
 * to their big endian versions
 *
 * @ehdr64: struct to byte swap
 */
void bswapElf64_Ehdr(Elf64_Ehdr *ehdr64)
{
	ehdr64->e_type      = __builtin_bswap16(ehdr64->e_type);
	ehdr64->e_machine   = __builtin_bswap16(ehdr64->e_machine);
	ehdr64->e_version   = __builtin_bswap32(ehdr64->e_version);
	ehdr64->e_entry     = __builtin_bswap64(ehdr64->e_entry);
	ehdr64->e_phoff     = __builtin_bswap64(ehdr64->e_phoff);
	ehdr64->e_shoff     = __builtin_bswap64(ehdr64->e_shoff);
	ehdr64->e_flags     = __builtin_bswap32(ehdr64->e_flags);
	ehdr64->e_ehsize    = __builtin_bswap16(ehdr64->e_ehsize);
	ehdr64->e_phentsize = __builtin_bswap16(ehdr64->e_phentsize);
	ehdr64->e_phnum     = __builtin_bswap16(ehdr64->e_phnum);
	ehdr64->e_shentsize = __builtin_bswap16(ehdr64->e_shentsize);
	ehdr64->e_shnum     = __builtin_bswap16(ehdr64->e_shnum);
	ehdr64->e_shstrndx  = __builtin_bswap16(ehdr64->e_shstrndx);
}



/**
 * bswapElf32_Ehdr - byte swaps all little endian values in a Elf32_Ehdr
 * to their big endian versions
 *
 * @ehdr32: struct to byte swap
 */
void bswapElf32_Ehdr(Elf32_Ehdr *ehdr32)
{
	ehdr32->e_type      = __builtin_bswap16(ehdr32->e_type);
	ehdr32->e_machine   = __builtin_bswap16(ehdr32->e_machine);
	ehdr32->e_version   = __builtin_bswap32(ehdr32->e_version);
	ehdr32->e_entry     = __builtin_bswap32(ehdr32->e_entry);
	ehdr32->e_phoff     = __builtin_bswap32(ehdr32->e_phoff);
	ehdr32->e_shoff     = __builtin_bswap32(ehdr32->e_shoff);
	ehdr32->e_flags     = __builtin_bswap32(ehdr32->e_flags);
	ehdr32->e_ehsize    = __builtin_bswap16(ehdr32->e_ehsize);
	ehdr32->e_phentsize = __builtin_bswap16(ehdr32->e_phentsize);
	ehdr32->e_phnum     = __builtin_bswap16(ehdr32->e_phnum);
	ehdr32->e_shentsize = __builtin_bswap16(ehdr32->e_shentsize);
	ehdr32->e_shnum     = __builtin_bswap16(ehdr32->e_shnum);
	ehdr32->e_shstrndx  = __builtin_bswap16(ehdr32->e_shstrndx);
}
