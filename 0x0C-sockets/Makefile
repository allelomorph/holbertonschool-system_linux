# -*- MakeFile -*-
# 0x0C. Sockets - project 441


CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

4-5-6-7-API_server.o: 4-5-6-7-API_server.c
	$(CC) -c $(CFLAGS) 4-5-6-7-API_server.c

8-9-10-11-API_server.o: 8-9-10-11-API_server.c
	$(CC) -c $(CFLAGS) 8-9-10-11-API_server.c

4-5-6-7-setSignalHandler.o: 4-5-6-7-setSignalHandler.c
	$(CC) -c $(CFLAGS) 4-5-6-7-setSignalHandler.c

8-9-10-11-setSignalHandler.o: 8-9-10-11-setSignalHandler.c
	$(CC) -c $(CFLAGS) 8-9-10-11-setSignalHandler.c

tokenizeBySubstr.o: tokenizeBySubstr.c
	$(CC) -c $(CFLAGS) tokenizeBySubstr.c

parseRequest.o: parseRequest.c
	$(CC) -c $(CFLAGS) parseRequest.c

4-5-6-7-valid_fields.o: 4-5-6-7-valid_fields.c
	$(CC) -c $(CFLAGS) 4-5-6-7-valid_fields.c

8-valid_fields.o: 8-valid_fields.c
	$(CC) -c $(CFLAGS) 8-valid_fields.c

9-10-valid_fields.o: 9-10-valid_fields.c
	$(CC) -c $(CFLAGS) 9-10-valid_fields.c

11-valid_fields.o: 11-valid_fields.c
	$(CC) -c $(CFLAGS) 11-valid_fields.c

parseRequestLine.o: parseRequestLine.c
	$(CC) -c $(CFLAGS) parseRequestLine.c

4-5-6-7-HTTP_response.o: 4-5-6-7-HTTP_response.c
	$(CC) -c $(CFLAGS) 4-5-6-7-HTTP_response.c

8-9-10-11-HTTP_response.o: 8-9-10-11-HTTP_response.c
	$(CC) -c $(CFLAGS) 8-9-10-11-HTTP_response.c

cleanup.o: cleanup.c
	$(CC) -c $(CFLAGS) cleanup.c

methods.o: methods.c
	$(CC) -c $(CFLAGS) methods.c

method_utils.o: method_utils.c
	$(CC) -c $(CFLAG_UTILS) method_utils.c

todos.o: todos.c
	$(CC) -c $(CFLAGS) todos.c



# task 4. REST API - The Request
4-printRequest.o: 4-printRequest.c
	$(CC) -c $(CFLAGS) 4-printRequest.c

todo_api_0: API_server.h \
	4-5-6-7-API_server.o 4-5-6-7-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 4-5-6-7-valid_fields.o parseRequestLine.o \
	4-5-6-7-HTTP_response.o cleanup.o 4-printRequest.o
	$(CC) $(CFLAGS) API_server.h \
	4-5-6-7-API_server.o 4-5-6-7-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 4-5-6-7-valid_fields.o parseRequestLine.o \
	4-5-6-7-HTTP_response.o cleanup.o 4-printRequest.o \
	-o todo_api_0


# task 5. REST API - Queries
5-printRequest.o: 5-printRequest.c
	$(CC) -c $(CFLAGS) 5-printRequest.c

todo_api_1: API_server.h \
	4-5-6-7-API_server.o 4-5-6-7-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 4-5-6-7-valid_fields.o parseRequestLine.o \
	4-5-6-7-HTTP_response.o cleanup.o 5-printRequest.o
	$(CC) $(CFLAGS) API_server.h \
	4-5-6-7-API_server.o 4-5-6-7-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 4-5-6-7-valid_fields.o parseRequestLine.o \
	4-5-6-7-HTTP_response.o cleanup.o 5-printRequest.o \
	-o todo_api_1


# task 6. REST API - Headers
6-printRequest.o: 6-printRequest.c
	$(CC) -c $(CFLAGS) 6-printRequest.c

todo_api_2: API_server.h \
	4-5-6-7-API_server.o 4-5-6-7-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 4-5-6-7-valid_fields.o parseRequestLine.o \
	4-5-6-7-HTTP_response.o cleanup.o 6-printRequest.o
	$(CC) $(CFLAGS) API_server.h \
	4-5-6-7-API_server.o 4-5-6-7-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 4-5-6-7-valid_fields.o parseRequestLine.o \
	4-5-6-7-HTTP_response.o cleanup.o 6-printRequest.o \
	-o todo_api_2


# task 7. REST API - Body parameters
7-printRequest.o: 7-printRequest.c
	$(CC) -c $(CFLAGS) 7-printRequest.c

todo_api_3: API_server.h \
	4-5-6-7-API_server.o 4-5-6-7-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 4-5-6-7-valid_fields.o parseRequestLine.o \
	4-5-6-7-HTTP_response.o cleanup.o 7-printRequest.o
	$(CC) $(CFLAGS) API_server.h \
	4-5-6-7-API_server.o 4-5-6-7-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 4-5-6-7-valid_fields.o parseRequestLine.o \
	4-5-6-7-HTTP_response.o cleanup.o 7-printRequest.o \
	-o todo_api_3


# task 8. REST API - Create TODO
todo_api_4: API_server.h \
	8-9-10-11-API_server.o 8-9-10-11-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 8-valid_fields.o parseRequestLine.o \
	8-9-10-11-HTTP_response.o cleanup.o methods.o todos.o
	$(CC) $(CFLAGS) API_server.h \
	8-9-10-11-API_server.o 8-9-10-11-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 8-valid_fields.o parseRequestLine.o \
	8-9-10-11-HTTP_response.o cleanup.o methods.o method_utils.o todos.o \
	-o todo_api_4


# task 9. REST API - Retrieve all TODOs
todo_api_5: API_server.h \
	8-9-10-11-API_server.o 8-9-10-11-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 9-10-valid_fields.o parseRequestLine.o \
	8-9-10-11-HTTP_response.o cleanup.o methods.o todos.o
	$(CC) $(CFLAGS) API_server.h \
	8-9-10-11-API_server.o 8-9-10-11-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 9-10-valid_fields.o parseRequestLine.o \
	8-9-10-11-HTTP_response.o cleanup.o methods.o method_utils.o todos.o \
	-o todo_api_5


# task 10. REST API - Retrieve a single TODO
todo_api_6: API_server.h \
	8-9-10-11-API_server.o 8-9-10-11-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 9-10-valid_fields.o parseRequestLine.o \
	8-9-10-11-HTTP_response.o cleanup.o methods.o todos.o
	$(CC) $(CFLAGS) API_server.h \
	8-9-10-11-API_server.o 8-9-10-11-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 9-10-valid_fields.o parseRequestLine.o \
	8-9-10-11-HTTP_response.o cleanup.o methods.o method_utils.o todos.o \
	-o todo_api_6


# task 11. REST API - Delete a TODO
todo_api_7: API_server.h \
	8-9-10-11-API_server.o 8-9-10-11-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 11-valid_fields.o parseRequestLine.o \
	8-9-10-11-HTTP_response.o cleanup.o methods.o method_utils.o todos.o
	$(CC) $(CFLAGS) API_server.h \
	8-9-10-11-API_server.o 8-9-10-11-setSignalHandler.o tokenizeBySubstr.o \
	parseRequest.o 11-valid_fields.o parseRequestLine.o \
	8-9-10-11-HTTP_response.o cleanup.o methods.o method_utils.o todos.o \
	-o todo_api_7

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
