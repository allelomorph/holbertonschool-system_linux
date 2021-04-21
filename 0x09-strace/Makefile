# -*- MakeFile -*-
# 0x09. C - Strace - project 393


CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic


# task 0. Step #0 - Syscall number
strace_0.o: strace_0.c
	$(CC) -c $(CFLAGS) strace_0.c

strace_0: strace_0.o
	$(CC) $(CFLAGS) strace_0.o -o strace_0


# task 1. Step #1 - Syscall name
strace_1.o: strace_1.c
	$(CC) -c $(CFLAGS) strace_1.c

strace_1: strace_1.o syscalls.h
	$(CC) $(CFLAGS) strace_1.o -o strace_1


# task 2. Step #2 - Return value
strace_2.o: strace_2.c
	$(CC) -c $(CFLAGS) strace_2.c

strace_2: strace_2.o syscalls.h
	$(CC) $(CFLAGS) strace_2.o -o strace_2


# task 3. Step #3 - Raw parameters
strace_3.o: strace_3.c
	$(CC) -c $(CFLAGS) strace_3.c

strace_3: strace_3.o syscalls.h
	$(CC) $(CFLAGS) strace_3.o -o strace_3


# task 4. Step #4 - Strings
#?.o: ?.c
#	$(CC) -c $(CFLAGS) ?.c

#strace_4: ?.o
#	$(CC) $(CFLAGS) ?.o -o strace_4


# task 5. Step #5 - Integers
#?.o: ?.c
#	$(CC) -c $(CFLAGS) ?.c

#strace_5: ?.o
#	$(CC) $(CFLAGS) ?.o -o strace_5


# task 6. Step #6 - Macros
#?.o: ?.c
#	$(CC) -c $(CFLAGS) ?.c

#strace_6: ?.o
#	$(CC) $(CFLAGS) ?.o -o strace_6


# task 7. Step #7 - Buffer
#?.o: ?.c
#	$(CC) -c $(CFLAGS) ?.c

#strace_7: ?.o
#	$(CC) $(CFLAGS) ?.o -o strace_7


# task 8. Step #8 - Structure
#?.o: ?.c
#	$(CC) -c $(CFLAGS) ?.c

#strace_8: ?.o
#	$(CC) $(CFLAGS) ?.o -o strace_8


.PHONY: all clean_all oclean_all fclean_all re_all

all:
	strace_0 strace_1 strace_2 strace_3 strace_4 \
	strace_5 strace_6 strace_7 strace_8

clean_all:
	$(RM) *~ strace_0 strace_1 strace_2 strace_3 strace_4 \
	strace_5 strace_6 strace_7 strace_8

oclean_all:
	$(RM) *.o

fclean_all: clean_all oclean_all

re_all: oclean_all all