#include "hobjdump.h"

#include <string.h>
/* malloc free */
#include <stdlib.h>

int printSections(objdump_state *state)
{
	Elf64_Shdr *section = NULL;
	bool bss_reached = false;
	int i;
	unsigned char *sec_buf = NULL;

	if (!state || !state->s_headers)
		return (1);

	for (i = 1; i < state->f_header.e_shnum; i++)
	{
		section = state->s_headers + i;
		if (section->sh_type == SHT_NOBITS &&
		    strncmp(state->sh_strtab + section->sh_name, ".bss",
			    5) == 0)
		{
			bss_reached = true;
			continue;
		}
		if (section->sh_size == 0)
			continue;
		if (state->f_header.e_type == ET_REL &&
		    (section->sh_type == SHT_RELA ||
		     section->sh_type == SHT_REL))
			continue;
		if (bss_reached &&
		    (section->sh_type == SHT_SYMTAB ||
		     section->sh_type == SHT_STRTAB))
			continue;

		sec_buf = getSecBuffer(state, section);
		if (!sec_buf)
			return (1);
		printf("Contents of section %s:\n",
		       state->sh_strtab + section->sh_name);
		printSecBuffer(sec_buf, section);
		free(sec_buf);
	}

	return (0);
}

void printSecBuffer(unsigned char *buff, Elf64_Shdr *section)
{
	uint64_t row, col;
	char addr_fmt[10], addr_end[20];
	int addr_width = 0;

	if (!buff || !section || section->sh_size == 0)
		return;

	/* measure printed length of last address in section */
	addr_width = sprintf(addr_end, "%lx",
		section->sh_addr + section->sh_size);
	/* this becomes address field width, minimum 4 chars */
	sprintf(addr_fmt, " %%0%ilx", addr_width > 4 ? addr_width : 4);

	for (row = 0; row < section->sh_size / 16; row++)
	{
		/* row address */
		printf(addr_fmt, section->sh_addr + (row * 16));
		/* hex */
		for (col = 0; col < 16; col++)
		{
			if (col % 4 == 0)
				putchar(' ');
			printf("%02x", buff[(row * 16) + col]);
		}
		putchar(' ');
		putchar(' ');
		/* as chars */
		for (col = 0; col < 16; col++)
		{
			if (buff[(row * 16) + col] < ' ' ||
			    buff[(row * 16) + col] > '~')
				putchar('.');
			else
				putchar(buff[(row * 16) + col]);
		}
		putchar('\n');
	}

	if (section->sh_size % 16)
	{
		/* row address */
		printf(addr_fmt, section->sh_addr + (row * 16));
		/* hex */
		for (col = 0; col < 16; col++)
		{
			if (col % 4 == 0)
				putchar(' ');
			if (col >= section->sh_size % 16)
			{
				putchar(' ');
				putchar(' ');
			}
			else
				printf("%02x", buff[(row * 16) + col]);
		}
		putchar(' ');
		putchar(' ');
		/* as chars */
		for (col = 0; col < 16; col++)
		{
			if (col >= section->sh_size % 16)
				putchar(' ');
			else if (buff[(row * 16) + col] < ' ' ||
			    buff[(row * 16) + col] > '~')
				putchar('.');
			else
				putchar(buff[(row * 16) + col]);
		}
		printf("\n");
	}

}

unsigned char *getSecBuffer(objdump_state *state, Elf64_Shdr *section)
{
	unsigned char *buff = NULL;

	if (!state || !section)
		return (NULL);

	buff = malloc(sizeof(unsigned char) * section->sh_size);
	if (!buff)
		return (NULL);

	if (fseek(state->f_stream, section->sh_offset, SEEK_SET) == -1)
		return (NULL);

	if (fread(buff, sizeof(unsigned char), section->sh_size,
		  state->f_stream) != section->sh_size)
		return (NULL);

	/* need to byteswap anything if just chars? */

	return (buff);
}
