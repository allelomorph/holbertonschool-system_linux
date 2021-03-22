# -*- MakeFile -*-
# 0x07. C - ELF: nm/objdump - project 369

CC = gcc
# consider using -fno-builtin
CFLAGS = -Wall -Werror -Wextra -pedantic


# task 0. nm -p
hnm.o: hnm.c
	$(CC) -c $(CFLAGS) hnm.c

hnm_main_help.o: hnm_main_help.c
	$(CC) -c $(CFLAGS) hnm_main_help.c

hnm_getFileHeader.o: hnm_getFileHeader.c
	$(CC) -c $(CFLAGS) hnm_getFileHeader.c

hnm_getSecHeaders.o: hnm_getSecHeaders.c
	$(CC) -c $(CFLAGS) hnm_getSecHeaders.c

hnm_getSymTables.o: hnm_getSymTables.c
	$(CC) -c $(CFLAGS) hnm_getSymTables.c

hnm_E32SymToE64Sym.o: hnm_E32SymToE64Sym.c
	$(CC) -c $(CFLAGS) hnm_E32SymToE64Sym.c

hnm_printSymTables.o: hnm_printSymTables.c
	$(CC) -c $(CFLAGS) hnm_printSymTables.c

hnm: hnm.h hnm.o hnm_main_help.o hnm_getFileHeader.o hnm_getSecHeaders.o \
	hnm_getSymTables.o hnm_E32SymToE64Sym.o hnm_printSymTables.o
	$(CC) $(CFLAGS) hnm.o hnm_main_help.o hnm_getFileHeader.o \
	hnm_getSecHeaders.o hnm_getSymTables.o hnm_E32SymToE64Sym.o \
	hnm_printSymTables.o \
	-o hnm


# task 1. objdump -sf
hobjdump.o: hobjdump.c
	$(CC) -c $(CFLAGS) hobjdump.c

hobjdump_main_help.o: hobjdump_main_help.c
	$(CC) -c $(CFLAGS) hobjdump_main_help.c

hobjdump_getFileHeader.o: hobjdump_getFileHeader.c
	$(CC) -c $(CFLAGS) hobjdump_getFileHeader.c

hobjdump_getSecHeaders.o: hobjdump_getSecHeaders.c
	$(CC) -c $(CFLAGS) hobjdump_getSecHeaders.c

hobjdump_getSecHeadStrTab.o: hobjdump_getSecHeadStrTab.c
	$(CC) -c $(CFLAGS) hobjdump_getSecHeadStrTab.c

hobjdump_printFileInfo.o: hobjdump_printFileInfo.c
	$(CC) -c $(CFLAGS) hobjdump_printFileInfo.c

hobjdump_printSections.o: hobjdump_printSections.c
	$(CC) -c $(CFLAGS) hobjdump_printSections.c

hobjdump: hobjdump.h hobjdump.o hobjdump_main_help.o \
	hobjdump_getFileHeader.o hobjdump_getSecHeaders.o \
	hobjdump_getSecHeadStrTab.o \
	hobjdump_printFileInfo.o hobjdump_printSections.o
	$(CC) $(CFLAGS) hobjdump.o hobjdump_main_help.o \
	hobjdump_getFileHeader.o hobjdump_getSecHeaders.o \
	hobjdump_getSecHeadStrTab.o \
	hobjdump_printFileInfo.o hobjdump_printSections.o \
	-o hobjdump


.PHONY: all clean_all oclean_all fclean_all re_all

all: hnm hobjdump

clean_all:
	$(RM) *~ hnm hobjdump

oclean_all:
	$(RM) *.o

fclean_all: clean_all oclean_all

re_all: oclean_all all
