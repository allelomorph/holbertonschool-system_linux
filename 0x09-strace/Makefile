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
strace_4: syscalls.h hstrace.h \
	7_strace.o 7_printParams.o 7_printFlagsInt.o \
	7_printStrParam.o
	$(CC) $(CFLAGS) syscalls.h 7_strace.o 7_printParams.o \
	7_printFlagsInt.o 7_printStrParam.o -o strace_4


# task 5. Step #5 - Integers
strace_5: syscalls.h hstrace.h \
	7_strace.o 7_printParams.o 7_printFlagsInt.o \
	7_printStrParam.o
	$(CC) $(CFLAGS) syscalls.h 7_strace.o 7_printParams.o \
	7_printFlagsInt.o 7_printStrParam.o -o strace_5


# task 6. Step #6 - Macros
strace_6: syscalls.h hstrace.h \
	7_strace.o 7_printParams.o 7_printFlagsInt.o \
	7_printStrParam.o
	$(CC) $(CFLAGS) syscalls.h 7_strace.o 7_printParams.o \
	7_printFlagsInt.o 7_printStrParam.o -o strace_6


# task 7. Step #7 - Buffer
7_strace.o: 7_strace.c
	$(CC) -c $(CFLAGS) 7_strace.c

7_printParams.o: 7_printParams.c
	$(CC) -c $(CFLAGS) 7_printParams.c

7_printFlagsInt.o: 7_printFlagsInt.c
	$(CC) -c $(CFLAGS) 7_printFlagsInt.c

7_printStrParam.o: 7_printStrParam.c
	$(CC) -c $(CFLAGS) 7_printStrParam.c

strace_7: syscalls.h hstrace.h \
	7_strace.o 7_printParams.o 7_printFlagsInt.o \
	7_printStrParam.o
	$(CC) $(CFLAGS) syscalls.h 7_strace.o 7_printParams.o \
	7_printFlagsInt.o 7_printStrParam.o -o strace_7


# task 8. Step #8 - Structure
8_strace.o: 8_strace.c
	$(CC) -c $(CFLAGS) 8_strace.c

8_printParams.o: 8_printParams.c
	$(CC) -c $(CFLAGS) 8_printParams.c

8_printFlagsInt.o: 8_printFlagsInt.c
	$(CC) -c $(CFLAGS) 8_printFlagsInt.c

8_printStrParam.o: 8_printStrParam.c
	$(CC) -c $(CFLAGS) 8_printStrParam.c

8_printStatParam.o: 8_printStatParam.c
	$(CC) -c $(CFLAGS) 8_printStatParam.c

strace_8: syscalls.h hstrace.h \
	8_strace.o 8_printParams.o 8_printFlagsInt.o \
	8_printStrParam.o 8_printStatParam.o
	$(CC) $(CFLAGS) syscalls.h 8_strace.o 8_printParams.o \
	8_printFlagsInt.o 8_printStrParam.o 8_printStatParam.o -o strace_8


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
