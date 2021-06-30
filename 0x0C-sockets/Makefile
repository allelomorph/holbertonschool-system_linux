# -*- MakeFile -*-
# 0x0C. Sockets - project 441


CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

4-5-6-7-API_server.o: 4-5-6-7-API_server.c
	$(CC) -c $(CFLAGS) 4-5-6-7-API_server.c

setSignalHandler.o: setSignalHandler.c
	$(CC) -c $(CFLAGS) setSignalHandler.c

tokenizeBySubstr.o: tokenizeBySubstr.c
	$(CC) -c $(CFLAGS) tokenizeBySubstr.c

parseRequest.o: parseRequest.c
	$(CC) -c $(CFLAGS) parseRequest.c

parseRequestLine.o: parseRequestLine.c
	$(CC) -c $(CFLAGS) parseRequestLine.c

HTTP_response.o: HTTP_response.c
	$(CC) -c $(CFLAGS) HTTP_response.c


# task 4. REST API - The Request
4-printRequest.o: 4-printRequest.c
	$(CC) -c $(CFLAGS) 4-printRequest.c

todo_api_0: API_server.h \
	4-5-6-7-API_server.o setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o parseRequestLine.o HTTP_response.o \
	4-printRequest.o
	$(CC) $(CFLAGS) API_server.h \
	4-5-6-7-API_server.o setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o parseRequestLine.o HTTP_response.o \
	4-printRequest.o \
	-o todo_api_0


# task 5. REST API - Queries
5-printRequest.o: 5-printRequest.c
	$(CC) -c $(CFLAGS) 5-printRequest.c

todo_api_1: API_server.h \
	4-5-6-7-API_server.o setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o parseRequestLine.o HTTP_response.o \
	5-printRequest.o
	$(CC) $(CFLAGS) API_server.h \
	4-5-6-7-API_server.o setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o parseRequestLine.o HTTP_response.o \
	5-printRequest.o \
	-o todo_api_1


# task 6. REST API - Headers
6-printRequest.o: 6-printRequest.c
	$(CC) -c $(CFLAGS) 6-printRequest.c

todo_api_2: API_server.h \
	4-5-6-7-API_server.o setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o parseRequestLine.o HTTP_response.o \
	6-printRequest.o
	$(CC) $(CFLAGS) API_server.h \
	4-5-6-7-API_server.o setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o parseRequestLine.o HTTP_response.o \
	6-printRequest.o \
	-o todo_api_2


# task 7. REST API - Body parameters
7-printRequest.o: 7-printRequest.c
	$(CC) -c $(CFLAGS) 7-printRequest.c

todo_api_3: API_server.h \
	4-5-6-7-API_server.o setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o parseRequestLine.o HTTP_response.o \
	7-printRequest.o
	$(CC) $(CFLAGS) API_server.h \
	4-5-6-7-API_server.o setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o parseRequestLine.o HTTP_response.o \
	7-printRequest.o \
	-o todo_api_3


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
