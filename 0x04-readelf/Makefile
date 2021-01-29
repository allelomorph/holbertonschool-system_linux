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


# task 0
printELFHeader.o: printELFHeader.c
	$(CC) -c $(CFLAGS) printELFHeader.c

f_header_strs.o: f_header_strs.c
	$(CC) -c $(CFLAGS) f_header_strs.c

0-hreadelf.o: 0-hreadelf.c
	$(CC) -c $(CFLAGS) 0-hreadelf.c

0-hreadelf: holberton.h 0-hreadelf.o f_header_strs.o \
	printELFHeader.o getFileHeader.o main_help.o
	$(CC) $(CFLAGS) 0-hreadelf.o f_header_strs.o printELFHeader.o \
	getFileHeader.o main_help.o -o 0-hreadelf


# task 1
getSecHeaders.o: getSecHeaders.c
	$(CC) -c $(CFLAGS) getSecHeaders.c

printSecHeaders.o: printSecHeaders.c
	$(CC) -c $(CFLAGS) printSecHeaders.c

sh_strings.o: sh_strings.c
	$(CC) -c $(CFLAGS) sh_strings.c

1-hreadelf.o: 1-hreadelf.c
	$(CC) -c $(CFLAGS) 1-hreadelf.c

1-hreadelf: holberton.h 1-hreadelf.o getFileHeader.o main_help.o \
	getSecHeaders.o printSecHeaders.o sh_strings.o
	$(CC) $(CFLAGS) 1-hreadelf.o getFileHeader.o main_help.o \
	getSecHeaders.o printSecHeaders.o sh_strings.o -o 1-hreadelf


# task 2
2-hreadelf.o: 2-hreadelf.c
	$(CC) -c $(CFLAGS) 2-hreadelf.c
2-hreadelf: holberton.h 2-hreadelf.o getFileHeader.o main_help.o
	$(CC) $(CFLAGS) 2-hreadelf.o getFileHeader.o main_help.o -o 2-hreadelf

# task 100
100-hreadelf.o: 100-hreadelf.c
	$(CC) -c $(CFLAGS) 100-hreadelf.c
100-hreadelf: holberton.h 100-hreadelf.o getFileHeader.o main_help.o
	$(CC) $(CFLAGS) 100-hreadelf.o getFileHeader.o main_help.o -o 100-hreadelf


.PHONY: all clean_all oclean_all fclean_all re_all

all: 0-hreadelf 1-hreadelf 2-hreadelf 100-hreadelf

clean_all:
	$(RM) *~ *-hreadelf

oclean_all:
	$(RM) *-hreadelf.o

fclean_all: clean_all oclean_all

re_all: oclean_all all
