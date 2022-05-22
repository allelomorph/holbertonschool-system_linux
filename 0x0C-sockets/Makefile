# -*- MakeFile -*-
# (411) 0x0C. Sockets


CC     := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic


tokenizeBySubstr.o: tokenizeBySubstr.c
	$(CC) -c $(CFLAGS) tokenizeBySubstr.c

parseRequest.o: parseRequest.c
	$(CC) -c $(CFLAGS) parseRequest.c

parseRequestLine.o: parseRequestLine.c
	$(CC) -c $(CFLAGS) parseRequestLine.c

cleanup.o: cleanup.c
	$(CC) -c $(CFLAGS) cleanup.c

4_11_SHARED_OBJ := \
	tokenizeBySubstr.o parseRequest.o \
	parseRequestLine.o cleanup.o

4-7-API_server.o \
8-11-API_server.o: API_server.c
	$(CC) -c $(CFLAGS) -D SRC_VERSION=$(VERSION) $^ -o $@

4-7-setSignalHandler.o \
8-11-setSignalHandler.o: setSignalHandler.c
	$(CC) -c $(CFLAGS) -D SRC_VERSION=$(VERSION) $^ -o $@

4-7-HTTP_response.o \
8-11-HTTP_response.o: HTTP_response.c
	$(CC) -c $(CFLAGS) -D SRC_VERSION=$(VERSION) $^ -o $@

4_7_SHARED_OBJ := \
	4-7-API_server.o 4-7-setSignalHandler.o 4-7-HTTP_response.o

methods.o: methods.c
	$(CC) -c $(CFLAGS) methods.c

method_utils.o: method_utils.c
	$(CC) -c $(CFLAGS) method_utils.c

todos.o: todos.c
	$(CC) -c $(CFLAGS) todos.c

8_11_SHARED_OBJ := \
	8-11-API_server.o 8-11-setSignalHandler.o 8-11-HTTP_response.o \
	methods.o method_utils.o todos.o

4-7-valid_fields.o \
8-valid_fields.o \
9-10-valid_fields.o \
11-valid_fields.o: valid_fields.c
	$(CC) -c $(CFLAGS) -D SRC_VERSION=$(VERSION) $^ -o $@


# task 4. REST API - The Request
4-printRequest.o: 4-printRequest.c
	$(CC) -c $(CFLAGS) 4-printRequest.c

todo_api_0: VERSION := 4
todo_api_0: API_server.h $(4_11_SHARED_OBJ) $(4_7_SHARED_OBJ) \
	4-7-valid_fields.o 4-printRequest.o
	$(CC) $(CFLAGS) $^ -o $@


# task 5. REST API - Queries
5-printRequest.o: 5-printRequest.c
	$(CC) -c $(CFLAGS) 5-printRequest.c

todo_api_1: VERSION := 5
todo_api_1: API_server.h $(4_11_SHARED_OBJ) $(4_7_SHARED_OBJ) \
	4-7-valid_fields.o 5-printRequest.o
	$(CC) $(CFLAGS) $^ -o $@


# task 6. REST API - Headers
6-printRequest.o: 6-printRequest.c
	$(CC) -c $(CFLAGS) 6-printRequest.c

todo_api_2: VERSION := 6
todo_api_2: API_server.h $(4_11_SHARED_OBJ) $(4_7_SHARED_OBJ) \
	4-7-valid_fields.o 6-printRequest.o
	$(CC) $(CFLAGS) $^ -o $@


# task 7. REST API - Body parameters
7-printRequest.o: 7-printRequest.c
	$(CC) -c $(CFLAGS) 7-printRequest.c

todo_api_3: VERSION := 7
todo_api_3: API_server.h $(4_11_SHARED_OBJ) $(4_7_SHARED_OBJ) \
	4-7-valid_fields.o 7-printRequest.o
	$(CC) $(CFLAGS) $^ -o $@


# task 8. REST API - Create TODO
todo_api_4: VERSION := 8
todo_api_4: API_server.h $(4_11_SHARED_OBJ) $(8_11_SHARED_OBJ) \
	8-valid_fields.o
	$(CC) $(CFLAGS) $^ -o $@


# task 9. REST API - Retrieve all TODOs
todo_api_5: VERSION := 9
todo_api_5: API_server.h $(4_11_SHARED_OBJ) $(8_11_SHARED_OBJ) \
	9-10-valid_fields.o
	$(CC) $(CFLAGS) $^ -o $@


# task 10. REST API - Retrieve a single TODO
todo_api_6: VERSION := 10
todo_api_6: API_server.h $(4_11_SHARED_OBJ) $(8_11_SHARED_OBJ) \
	9-10-valid_fields.o
	$(CC) $(CFLAGS) $^ -o $@


# task 11. REST API - Delete a TODO
todo_api_7: VERSION := 11
todo_api_7: API_server.h $(4_11_SHARED_OBJ) $(8_11_SHARED_OBJ) \
	11-valid_fields.o
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: all clean_all oclean_all fclean_all re_all

all: todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 \
     todo_api_5 todo_api_6 todo_api_7

clean_all:
	$(RM) *~ todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 \
	todo_api_5 todo_api_6 todo_api_7

oclean_all:
	$(RM) *.o

fclean_all: clean_all oclean_all

re_all: oclean_all all
