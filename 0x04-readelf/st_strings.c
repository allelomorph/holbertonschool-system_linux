#include "holberton.h"


const char *getSymVisibility(unsigned int visibility)
{
	switch (visibility)
	{
	case STV_DEFAULT:return "DEFAULT";
	case STV_INTERNAL:return "INTERNAL";
	case STV_HIDDEN:return "HIDDEN";
	case STV_PROTECTED: return "PROTECTED";
	default:
/*
		errMsg("Unrecognized visibility value: %u\n", visibility);
*/
		return "<unknown>";
	}
}

/*
       st_other  This member defines the symbol visibility.

                 STV_DEFAULT     Default symbol visibility rules.
                 STV_INTERNAL    Processor-specific hidden class.
                 STV_HIDDEN      Symbol is unavailable in other modules.
                 STV_PROTECTED   Not preemptible, not exported.

                 There are macros for extracting the visibility type:

                 ELF32_ST_VISIBILITY(other) or ELF64_ST_VISIBILITY(other)
*/

const char *getSymIdxType(re_state *state, unsigned int type)
{
        static char buff[32];

	switch (type)
	{
	case SHN_UNDEF:return "UND";
	case SHN_ABS:return "ABS";
	case SHN_COMMON:return "COM";
	default:
		/* SHN_IA_64_ANSI_COMMON SHN_X86_64_LCOMMON SHN_MIPS_SCOMMON SHN_MIPS_SUNDEFINED not in elf.h */
	        if (type >= SHN_LOPROC && type <= SHN_HIPROC)
			sprintf(buff, "PRC[0x%04x]", type & 0xffff);
		else if (type >= SHN_LOOS && type <= SHN_HIOS)
			sprintf(buff, "OS [0x%04x]", type & 0xffff);
		else if (type >= SHN_LORESERVE)
			sprintf(buff, "RSV[0x%04x]", type & 0xffff);
		else if (state->f_header.e_shnum != 0
			 && type >= state->f_header.e_shnum)
			sprintf(buff, "bad section index[%3u]", type);
		else
			sprintf(buff, "%3u", type);
		return buff;
	}
}


const char *getSymType(unsigned int type)
{
	static char buff[64];

	switch (type)
	{
	case STT_NOTYPE:return "NOTYPE";
	case STT_OBJECT:return "OBJECT";
	case STT_FUNC:return "FUNC";
	case STT_SECTION:return "SECTION";
	case STT_FILE:return "FILE";
	case STT_COMMON:return "COMMON";
	case STT_TLS:return "TLS";
/* STT_RELC STT_SRELC not in elf.h */
	default:
		if (type >= STT_LOPROC && type <= STT_HIPROC)
			/* STT_ARM_TFUNC STT_REGISTER STT_PARISC_MILLI not in elf.h */
			snprintf (buff, sizeof(buff), "<processor specific>: %u", type);
		/* STT_LOOS STT_HIOS not in elf.h */
		else
			snprintf (buff, sizeof(buff), "<unknown>: %u", type);
		return buff;
	}
}

const char *getSymBinding(re_state *state, unsigned int binding)
{
	static char buff[64];

	switch (binding)
	{
	case STB_LOCAL:return "LOCAL";
	case STB_GLOBAL:return "GLOBAL";
	case STB_WEAK:return "WEAK";
	default:
		if (binding >= STB_LOPROC && binding <= STB_HIPROC)
			snprintf (buff, sizeof(buff),
				  "<processor specific>: %u", binding);
		else if (binding >= STB_LOOS && binding <= STB_HIOS)
		{
			if (binding == STB_GNU_UNIQUE
			    && state->f_header.e_ident[EI_OSABI] == ELFOSABI_GNU)
				return "UNIQUE";
			snprintf (buff, sizeof(buff),
				  "<OS specific>: %u", binding);
		}
		else
			snprintf (buff, sizeof (buff),
				  "<unknown>: %u", binding);
		return buff;
	}
}

/*
st_info   This member specifies the symbol's type and binding attributes:

        STT_NOTYPE  The symbol's type is not defined.

	STT_OBJECT  The symbol is associated with a data object.

	STT_FUNC    The symbol is associated with a function or other executable code.

	STT_SECTION The  symbol  is associated with a section.  Symbol table entries of this type exist pri
	marily for relocation and normally have STB_LOCAL bindings.

	STT_FILE    By convention, the symbol's name gives the name of the source file associated  with  the
	object file.  A file symbol has STB_LOCAL bindings, its section index is SHN_ABS, and it
	precedes the other STB_LOCAL symbols of the file, if it is present.

	STT_LOPROC  This value up to and including STT_HIPROC is reserved for processor-specific semantics.

	STT_HIPROC  This value down to and including STT_LOPROC is reserved  for  processor-specific  seman
	tics.

	STB_LOCAL   Local  symbols  are  not  visible  outside  the object file containing their definition.
	Local symbols of the same name may exist in multiple files without interfering with each
	other.

	STB_GLOBAL  Global symbols are visible to all object files being combined.  One file's definition of
	a global symbol will satisfy another file's undefined reference to the same symbol.

	STB_WEAK    Weak symbols resemble global symbols, but their definitions have lower precedence.

	STB_LOPROC  This value up to and including STB_HIPROC is reserved for processor-specific semantics.

	STB_HIPROC  This value down to and including STB_LOPROC is reserved  for  processor-specific  seman
	tics.

	STB_HIPROC  This value down to and including STB_LOPROC is reserved  for  processor-specific  seman
	tics.

	There are macros for packing and unpacking the binding and type fields:

	ELF32_ST_BIND(info) or ELF64_ST_BIND(info) extract a binding from an st_info value.

	ELF32_ST_TYPE(info) or ELF64_ST_TYPE(info)
	   extract a type from an st_info value.

	ELF32_ST_INFO(bind, type) or ELF64_ST_INFO(bind, type)
	    convert a binding and a type into an st_info value.
*/
