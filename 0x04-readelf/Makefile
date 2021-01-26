# -*- MakeFile -*-xo
# 0x04. C - ELF: readelf - project 364; one rule per exercise

CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -fno-builtin
#OBJ = $(SRC:.c=.o)

#$(OBJ): $(SRC)
#	$(info $$var is [${SRC}])
#	$(info $$var is [${OBJ}])
#	$(CC) -c $(CFLAGS) $(SRC)

0-hreadelf: SRC := 0-hreadelf.c
0-hreadelf: NAME := 0-hreadelf
0-hreadelf.o: $(SRC)
	$(CC) -c $(CFLAGS) $(SRC)
0-hreadelf: holberton.h 0-hreadelf.o
	$(CC) $(CFLAGS) 0-hreadelf.o -o $(NAME)

1-hreadelf: SRC := 1-hreadelf.c
1-hreadelf: NAME := 1-hreadelf
1-hreadelf.o: $(SRC)
	$(CC) -c $(CFLAGS) $(SRC)
1-hreadelf: holberton.h 1-hreadelf.o
	$(CC) $(CFLAGS) 1-hreadelf.o -o $(NAME)

2-hreadelf: SRC := 2-hreadelf.c
2-hreadelf: NAME := 2-hreadelf
2-hreadelf.o: $(SRC)
	$(CC) -c $(CFLAGS) $(SRC)
2-hreadelf: holberton.h 2-hreadelf.o
	$(CC) $(CFLAGS) 2-hreadelf.o -o $(NAME)

100-hreadelf: SRC := 100-hreadelf.c
100-hreadelf: NAME := 100-hreadelf
100-hreadelf.o: $(SRC)
	$(CC) -c $(CFLAGS) $(SRC)
100-hreadelf: holberton.h 100-hreadelf.o
	$(CC) $(CFLAGS) 100-hreadelf.o -o $(NAME)

.PHONY: all clean_all oclean_all fclean_all re_all

all: 0-hreadelf 1-hreadelf 2-hreadelf 100-hreadelf

clean_all:
	$(RM) *~ *-hreadelf

oclean_all:
	$(RM) *-hreadelf.o

fclean_all: clean_all oclean_all

re_all: oclean_all all
