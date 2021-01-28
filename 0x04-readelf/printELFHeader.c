#include "holberton.h"

/* memcmp */
#include <string.h>


int printELFHeader(re_state *state)
{
	unsigned int i;

/* this second check of full magic 16 after header load needs to also happen when not printing file header */
	if (memcmp(state->f_header.e_ident, ELFMAG, SELFMAG) != 0)
	{
	        errorMsg("Not an ELF file - it has the wrong magic bytes " \
			    "at the start\n", NULL, state);
		return (1);
	}

	puts("ELF Header:");
	printf("  Magic:   ");
	/* stock readelf prints 1 space before newline */
	for (i = 0; i < EI_NIDENT; i++)
		printf("%2.2x ", state->f_header.e_ident[i]);
	putchar('\n');
	printf("  Class:                             %s\n",
	       state->ELF_32bit ? "ELF32" : "ELF64");
	printf("  Data:                              %s, %s endian\n",
	       "2's complement", state->big_endian ? "big" : "little");
	printf("  Version:                           %u%s\n",
	       state->f_header.e_ident[EI_VERSION],
	       state->f_header.e_ident[EI_VERSION] ==
	       EV_CURRENT ? " (current)" : "");
	printf("  OS/ABI:                            %s\n",
	       getOsabiName(state->f_header.e_ident[EI_OSABI]));
	printf("  ABI Version:                       %u\n",
	       state->f_header.e_ident[EI_ABIVERSION]);
	printf("  Type:                              %s\n",
	       getELFType(state->f_header.e_type));
	printf("  Machine:                           %s\n",
	       getMachineName(state->f_header.e_machine));
	printf("  Version:                           0x%x\n",
	       state->f_header.e_version);
	printf("  Entry point address:               0x%lx\n",
	       state->f_header.e_entry);
	printf("  Start of program headers:          %lu (bytes into file)\n",
	       state->f_header.e_phoff);
	printf("  Start of section headers:          %lu (bytes into file)\n",
	       state->f_header.e_shoff);
	printf("  Flags:                             0x%x\n",
	       state->f_header.e_flags);
	printf("  Size of this header:               %u (bytes)\n",
	       state->f_header.e_ehsize);
	printf("  Size of program headers:           %u (bytes)\n",
	       state->f_header.e_phentsize);
	printf("  Number of program headers:         %u\n",
	       state->f_header.e_phnum);
	printf("  Size of section headers:           %u (bytes)\n",
	       state->f_header.e_shentsize);
	printf("  Number of section headers:         %u\n",
	       state->f_header.e_shnum);
	printf("  Section header string table index: %u\n",
	       state->f_header.e_shstrndx);

	return (0);
}

/*
  Start of program headers:          64 (bytes into file)
  (bytes 32 - 39, read backwards since little endian: 00 00 00 00 00 00 00 40 = 64 (here 8 bytes/64 bits since 64 bit machine, would be 28 - 31, 4 bytes, 32 bits otherwise))

  Start of section headers:          4472 (bytes into file)
  (bytes 40 - 47, read backwards since little endian: 00 00 00 00 00 00 11 78 = 4472 (here 8 bytes/64 bits since 64 bit machine, would be 32 - 35, 4 bytes, 32 bits otherwise))

  Flags:                             0x0
  (bytes 48 - 51?, read backwards since little endian: 00 00 00 00 = 0)
  (bytes 36 - 39 in 32 bit)

  Size of this header:               64 (bytes)
  (bytes 52 - 53?, read backwards since little endian: 00 40 = 64)
  (bytes 40 - 41 in 32 bit)

  Size of program headers:           56 (bytes)
  (bytes 54 - 55?, read backwards since little endian: 00 38 = 56)
  (bytes 42 - 43 in 32 bit)

  Number of program headers:         9
  (bytes 56 - 57?, read backwards since little endian: 00 09 = 9)
  (bytes 44 - 45 in 32 bit)

  Size of section headers:           64 (bytes)
  (bytes 58 - 59?, read backwards since little endian: 00 40 = 64)
  (bytes 46 - 47 in 32 bit)

  Number of section headers:         30
  (bytes 60 - 61?, read backwards since little endian: 00 1e = 30)
  (bytes 48 - 49 in 32 bit)

  Section header string table index: 27
  (bytes 62 - 63?, read backwards since little endian: 00 1b = 27)
  (bytes 50 - 51 in 32 bit)
*/
