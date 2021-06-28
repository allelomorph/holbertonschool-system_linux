# -*- MakeFile -*-
# 0x0C. Sockets - project 441


CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic


# task 4. REST API - The Request
API_server.o: API_server.c
	$(CC) -c $(CFLAGS) API_server.c

parse_request.o: parse_request.c
	$(CC) -c $(CFLAGS) parse_request.c

todo_api_0: API_server.h API_server.o parse_request.o
	$(CC) $(CFLAGS) API_server.h API_server.o parse_request.o -o todo_api_0


# task 5. REST API - Queries
#: .c
#	$(CC) -c $(CFLAGS) .c

todo_api_1: .h .o
	$(CC) $(CFLAGS) .h .o -o todo_api_1


# task 6. REST API - Headers
#: .c
#	$(CC) -c $(CFLAGS) .c

todo_api_2: .h .o
	$(CC) $(CFLAGS) .h .o -o todo_api_2


# task 7. REST API - Body parameters
#: .c
#	$(CC) -c $(CFLAGS) .c

todo_api_3: .h .o
	$(CC) $(CFLAGS) .h .o -o todo_api_3


# task 8. REST API - Create TODO
#: .c
#	$(CC) -c $(CFLAGS) .c

todo_api_4: .h .o
	$(CC) $(CFLAGS) .h .o -o todo_api_4


# task 9. REST API - Retrieve all TODOs
#: .c
#	$(CC) -c $(CFLAGS) .c

todo_api_5: .h .o
	$(CC) $(CFLAGS) .h .o -o todo_api_5


# task 10. REST API - Retrieve a single TODO
#: .c
#	$(CC) -c $(CFLAGS) .c

todo_api_6: .h .o
	$(CC) $(CFLAGS) .h .o -o todo_api_6


# task 11. REST API - Delete a TODO
#: .c
#	$(CC) -c $(CFLAGS) .c

todo_api_7: .h .o
	$(CC) $(CFLAGS) .h .o -o todo_api_7


.PHONY: all clean_all oclean_all fclean_all re_all

all:
	todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 \
	todo_api_5 todo_api_6 todo_api_7

clean_all:
	$(RM) *~ todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 \
	todo_api_5 todo_api_6 todo_api_7

oclean_all:
	$(RM) *.o

fclean_all: clean_all oclean_all

re_all: oclean_all all
