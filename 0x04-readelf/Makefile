# -*- MakeFile -*-xo
# 0x04. C - ELF: readelf - project 364; one rule per exercise

CC = gcc
# consider using -fno-builtin
CFLAGS = -Wall -Werror -Wextra -pedantic


# shared among all builds
main_help.o: main_help.c
	$(CC) -c $(CFLAGS) main_help.c

getFileHeader.o: getFileHeader.c
	$(CC) -c $(CFLAGS) getFileHeader.c

# shared among tasks 1-3
getSecHeadStrTab.o: getSecHeadStrTab.c
	$(CC) -c $(CFLAGS) getSecHeadStrTab.c


# task 0
printFileHeader.o: printFileHeader.c
	$(CC) -c $(CFLAGS) printFileHeader.c

fh_strings.o: fh_strings.c
	$(CC) -c $(CFLAGS) fh_strings.c

0-hreadelf.o: 0-hreadelf.c
	$(CC) -c $(CFLAGS) 0-hreadelf.c

0-hreadelf: holberton.h 0-hreadelf.o main_help.o \
	getFileHeader.o fh_strings.o printFileHeader.o
	$(CC) $(CFLAGS) 0-hreadelf.o main_help.o \
	getFileHeader.o fh_strings.o printFileHeader.o \
	-o 0-hreadelf


# task 1
getSecHeaders.o: getSecHeaders.c
	$(CC) -c $(CFLAGS) getSecHeaders.c

printSecHeaders.o: printSecHeaders.c
	$(CC) -c $(CFLAGS) printSecHeaders.c

sh_strings.o: sh_strings.c
	$(CC) -c $(CFLAGS) sh_strings.c

1-hreadelf.o: 1-hreadelf.c
	$(CC) -c $(CFLAGS) 1-hreadelf.c

1-hreadelf: holberton.h 1-hreadelf.o main_help.o getFileHeader.o \
	getSecHeaders.o getSecHeadStrTab.o printSecHeaders.o sh_strings.o
	$(CC) $(CFLAGS) 1-hreadelf.o main_help.o getFileHeader.o \
	getSecHeaders.o getSecHeadStrTab.o printSecHeaders.o sh_strings.o \
	-o 1-hreadelf


# task 2
getProgHeaders.o: getProgHeaders.c
	$(CC) -c $(CFLAGS) getProgHeaders.c

printProgHeaders.o: printProgHeaders.c
	$(CC) -c $(CFLAGS) printProgHeaders.c

ph_strings.o: ph_strings.c
	$(CC) -c $(CFLAGS) ph_strings.c

2-hreadelf.o: 2-hreadelf.c
	$(CC) -c $(CFLAGS) 2-hreadelf.c

2-hreadelf: holberton.h 2-hreadelf.o main_help.o \
	getFileHeader.o fh_strings.o \
	getSecHeaders.o getSecHeadStrTab.o \
	getProgHeaders.o printProgHeaders.o ph_strings.o
	$(CC) $(CFLAGS) 2-hreadelf.o main_help.o \
	getFileHeader.o fh_strings.o \
	getSecHeaders.o getSecHeadStrTab.o \
	getProgHeaders.o printProgHeaders.o ph_strings.o \
	-o 2-hreadelf


# task 100
getSymTables.o: getSymTables.c
	$(CC) -c $(CFLAGS) getSymTables.c

get_sym_vers.o: get_sym_vers.c
	$(CC) -c $(CFLAGS) get_sym_vers.c

printSymTables.o: printSymTables.c
	$(CC) -c $(CFLAGS) printSymTables.c

st_strings.o: st_strings.c
	$(CC) -c $(CFLAGS) st_strings.c

100-hreadelf.o: 100-hreadelf.c
	$(CC) -c $(CFLAGS) 100-hreadelf.c

100-hreadelf: holberton.h 100-hreadelf.o main_help.o getFileHeader.o \
	getSecHeaders.o getSecHeadStrTab.o \
	getSymTables.o get_sym_vers.o printSymTables.o st_strings.o
	$(CC) $(CFLAGS) 100-hreadelf.o main_help.o getFileHeader.o \
	getSecHeaders.o getSecHeadStrTab.o \
	getSymTables.o get_sym_vers.o printSymTables.o st_strings.o \
	-o 100-hreadelf


.PHONY: all clean_all oclean_all fclean_all re_all

all: 0-hreadelf 1-hreadelf 2-hreadelf 100-hreadelf

clean_all:
	$(RM) *~ *-hreadelf

oclean_all:
	$(RM) *.o

fclean_all: clean_all oclean_all

re_all: oclean_all all
