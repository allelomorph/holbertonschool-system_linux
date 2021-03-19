#include "hnm.h"


/**
 * E32SymToE64Sym - promotes a Elf32_Sym struct to Elf64_Sym
 *
 * @Sym32: pointer to struct to promote to 64-bit
 * @Sym64: pointer to destination struct
 */
void E32SymToE64Sym(Elf32_Sym *Sym32, Elf64_Sym *Sym64)
{
	if (!Sym32 || !Sym64)
		return;

	Sym64->st_name  = (Elf64_Word)Sym32->st_name;
	/* 64 and 32 both unsigned char */
	Sym64->st_info  = Sym32->st_info;
	/* 64 and 32 both unsigned char */
	Sym64->st_other = Sym32->st_other;
	Sym64->st_shndx = (Elf64_Section)Sym32->st_shndx;
	Sym64->st_value = (Elf64_Addr)Sym32->st_value;
	Sym64->st_size  = (Elf64_Xword)Sym32->st_size;
}
