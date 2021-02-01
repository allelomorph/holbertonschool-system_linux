#include "holberton.h"

/* fseek fread */
#include <stdio.h>

/* malloc free */
#include <stdlib.h>


/**
 * getProgHeaders - selects function to get program headers (segments) based
 * on 32 or 64 configuration in ELF
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int getProgHeaders(re_state *state)
{
	if (state->f_header.e_phoff && state->f_header.e_phnum)
	{
		if (state->ELF_32bit)
		        return (get32bitProgHeaders(state));
		else
		        return (get64bitProgHeaders(state));
	}

	return (0);
}


/* malloc lseek fread */
/**
 * get64bitProgHeaders - reads ELF and stores program headers (segments)
 * in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int get64bitProgHeaders(re_state *state)
{
	Elf64_Phdr *p_headers = NULL;
	int i;

	p_headers = malloc(sizeof(Elf64_Phdr) * state->f_header.e_phnum);
	if (!p_headers)
		return (1);

	if (fseek(state->f_stream, state->f_header.e_phoff, SEEK_SET) == -1)
		return (1);

	if (fread(p_headers, sizeof(Elf64_Phdr), state->f_header.e_phnum,
		  state->f_stream) != state->f_header.e_phnum)
		return (1);

	if (state->big_endian)
	{
		for (i = 0; i < state->f_header.e_phnum; i++)
			bswapElf64_Phdr(p_headers + i);
	}

	/* state->p_headers 64 bit by default */
	state->p_headers = p_headers;

	return (0);
}

/* malloc lseek fread free */
/**
 * get32bitProgHeaders - reads ELF and stores program headers (segments)
 * in state
 *
 * @state: struct containing file data and info for error printing
 * Return: 1 on failure, 0 on success
 */
int get32bitProgHeaders(re_state *state)
{
	Elf32_Phdr *p_headers32 = NULL, *curr32 = NULL;
	Elf64_Phdr *p_headers64 = NULL, *curr64 = NULL;
	int i;

	p_headers32 = malloc(sizeof(Elf32_Phdr) * state->f_header.e_phnum);
	p_headers64 = malloc(sizeof(Elf64_Phdr) * state->f_header.e_phnum);
	if (!p_headers32 || !p_headers64)
		return (1);

	if (fseek(state->f_stream, state->f_header.e_phoff, SEEK_SET) == -1)
		return (1);

	if (fread(p_headers32, sizeof(Elf32_Phdr), state->f_header.e_phnum,
		  state->f_stream) != state->f_header.e_phnum)
		return (1);

	if (state->big_endian)
	{
		for (i = 0; i < state->f_header.e_phnum; i++)
			bswapElf32_Phdr(p_headers32 + i);
	}

	/* state->p_headers 64 bit by default, need to promote values */
	for (i = 0; i < state->f_header.e_phnum; i++)
	{
		curr32 = p_headers32 + i;
		curr64 = p_headers64 + i;
		curr64->p_type   = (Elf64_Word)curr32->p_type;
		curr64->p_flags  = (Elf64_Word)curr32->p_flags;
		curr64->p_offset = (Elf64_Off)curr32->p_offset;
		curr64->p_vaddr  = (Elf64_Addr)curr32->p_vaddr;
		curr64->p_paddr  = (Elf64_Addr)curr32->p_paddr;
		curr64->p_filesz = (Elf64_Xword)curr32->p_filesz;
		curr64->p_memsz  = (Elf64_Xword)curr32->p_memsz;
		curr64->p_align  = (Elf64_Xword)curr32->p_align;
	}

	free(p_headers32);
	state->p_headers = p_headers64;

	return (0);
}

/**
 * bswapElf64_Phdr - byte swaps all little endian values in a Elf64_Phdr
 * to their big endian versions
 *
 * @phdr64: struct to byte swap
 */
void bswapElf64_Phdr(Elf64_Phdr *phdr64)
{
	phdr64->p_type   = __builtin_bswap32(phdr64->p_type);
	phdr64->p_flags  = __builtin_bswap32(phdr64->p_flags);
	phdr64->p_offset = __builtin_bswap64(phdr64->p_offset);
	phdr64->p_vaddr  = __builtin_bswap64(phdr64->p_vaddr);
	phdr64->p_paddr  = __builtin_bswap64(phdr64->p_paddr);
	phdr64->p_filesz = __builtin_bswap64(phdr64->p_filesz);
	phdr64->p_memsz  = __builtin_bswap64(phdr64->p_memsz);
	phdr64->p_align  = __builtin_bswap64(phdr64->p_align);
}

/**
 * bswapElf32_Phdr - byte swaps all little endian values in a Elf32_Phdr
 * to their big endian versions
 *
 * @phdr32: struct to byte swap
 */
void bswapElf32_Phdr(Elf32_Phdr *phdr32)
{
	phdr32->p_type   = __builtin_bswap32(phdr32->p_type);
	phdr32->p_flags  = __builtin_bswap32(phdr32->p_flags);
	phdr32->p_offset = __builtin_bswap32(phdr32->p_offset);
	phdr32->p_vaddr  = __builtin_bswap32(phdr32->p_vaddr);
	phdr32->p_paddr  = __builtin_bswap32(phdr32->p_paddr);
	phdr32->p_filesz = __builtin_bswap32(phdr32->p_filesz);
	phdr32->p_memsz  = __builtin_bswap32(phdr32->p_memsz);
	phdr32->p_align  = __builtin_bswap32(phdr32->p_align);
}
