# -*- MakeFile -*-
# 0x09. C - Strace - project 393


CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic


# task 0. Step #0 - Syscall number
0_strace.o: 0_strace.c
	$(CC) -c $(CFLAGS) 0_strace.c

strace_0: syscalls.h 0_strace.o
	$(CC) $(CFLAGS) syscalls.h 0_strace.o -o strace_0


# task 1. Step #1 - Syscall name
1_strace.o: 1_strace.c
	$(CC) -c $(CFLAGS) 1_strace.c

strace_1: syscalls.h 1_strace.o
	$(CC) $(CFLAGS) syscalls.h 1_strace.o -o strace_1


# task 2. Step #2 - Return value
2_strace.o: 2_strace.c
	$(CC) -c $(CFLAGS) 2_strace.c

strace_2: syscalls.h 2_strace.o
	$(CC) $(CFLAGS) syscalls.h 2_strace.o -o strace_2


# task 3. Step #3 - Raw parameters
3_strace.o: 3_strace.c
	$(CC) -c $(CFLAGS) 3_strace.c

strace_3: syscalls.h 3_strace.o
	$(CC) $(CFLAGS) syscalls.h 3_strace.o -o strace_3


# task 4. Step #4 - Strings
4_strace.o: 4_strace.c
	$(CC) -c $(CFLAGS) 4_strace.c

4_printParams.o: 4_printParams.c
	$(CC) -c $(CFLAGS) 4_printParams.c

strace_4: syscalls.h hstrace.h \
	4_strace.o 4_printParams.o 6_printStrParam.o
	$(CC) $(CFLAGS) syscalls.h hstrace.h 4_printParams.o \
	4_strace.o 6_printStrParam.o -o strace_5


# task 5. Step #5 - Integers
5_printParams.o: 5_printParams.c
	$(CC) -c $(CFLAGS) 5_printParams.c

strace_5: syscalls.h hstrace.h \
	6_strace.o 5_printParams.o 6_printStrParam.o
	$(CC) $(CFLAGS) syscalls.h hstrace.h 5_printParams.o \
	6_strace.o 6_printStrParam.o -o strace_5


# task 6. Step #6 - Macros
6_strace.o: 6_strace.c
	$(CC) -c $(CFLAGS) 6_strace.c

6_printParams.o: 6_printParams.c
	$(CC) -c $(CFLAGS) 6_printParams.c

6_printStrParam.o: 6_printStrParam.c
	$(CC) -c $(CFLAGS) 6_printStrParam.c

strace_6: syscalls.h hstrace.h \
	6_strace.o 6_printParams.o 6_printStrParam.o \
	8_printFlagsInt.o
	$(CC) $(CFLAGS) syscalls.h hstrace.h 6_strace.o 6_printParams.o \
	8_printFlagsInt.o 6_printStrParam.o -o strace_6


# task 7. Step #7 - Buffer
7_strace.o: 7_strace.c
	$(CC) -c $(CFLAGS) 7_strace.c

7_printParams.o: 7_printParams.c
	$(CC) -c $(CFLAGS) 7_printParams.c

strace_7: syscalls.h hstrace.h \
	7_strace.o 7_printParams.o \
	8_printFlagsInt.o 8_printStrParam.o
	$(CC) $(CFLAGS) syscalls.h hstrace.h 7_strace.o 7_printParams.o \
	8_printFlagsInt.o 8_printStrParam.o -o strace_7


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
	$(CC) $(CFLAGS) syscalls.h hstrace.h 8_strace.o 8_printParams.o \
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
