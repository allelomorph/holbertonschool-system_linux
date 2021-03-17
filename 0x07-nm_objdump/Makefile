# -*- MakeFile -*-
# 0x07. C - ELF: nm/objdump - project 369

CC = gcc
# consider using -fno-builtin
CFLAGS = -Wall -Werror -Wextra -pedantic


# task 0. nm
0-hnm.o: 0-hnm.c
	$(CC) -c $(CFLAGS) 0-hnm.c

main_help.o: main_help.c
	$(CC) -c $(CFLAGS) main_help.c

getFileHeader.o: getFileHeader.c
	$(CC) -c $(CFLAGS) getFileHeader.c

getSecHeaders.o: getSecHeaders.c
	$(CC) -c $(CFLAGS) getSecHeaders.c

getSecHeadStrTab.o: getSecHeadStrTab.c
	$(CC) -c $(CFLAGS) getSecHeadStrTab.c

getSymTables.o: getSymTables.c
	$(CC) -c $(CFLAGS) getSymTables.c

printSymTables.o: printSymTables.c
	$(CC) -c $(CFLAGS) printSymTables.c

hnm: hnm.h 0-hnm.o main_help.o getFileHeader.o \
	getSecHeaders.o getSecHeadStrTab.o \
	getSymTables.o printSymTables.o
	$(CC) $(CFLAGS) 0-hnm.o main_help.o getFileHeader.o \
	getSecHeaders.o getSecHeadStrTab.o \
	getSymTables.o printSymTables.o \
	-o hnm


# task 1. objdump -sf


.PHONY: all clean_all oclean_all fclean_all re_all

all: hnm hobjdump

clean_all:
	$(RM) *~ hnm hobjdump

oclean_all:
	$(RM) *.o

fclean_all: clean_all oclean_all

re_all: oclean_all all
