#ifndef API_SERVER_H
#define API_SERVER_H

/* size_t */
#include <stddef.h>
/* netinet/in.h also includes sys/types.h and sys/socket.h */
/* sys/types.h needed for socket.h portability beyond Linux / POSIX.1-2001 */
/* sockaddr_in */
#include <netinet/in.h>


typedef struct HTTP_header_s {
	char *header;
	char *value;
	struct HTTP_header_s *next;
} HTTP_header_t;

typedef struct HTTP_request_s {
	char *Method;
	char *Request_URI;
	char *HTTP_Version;
	HTTP_header_t *general_headers;
	HTTP_header_t *request_headers;
	HTTP_header_t *entity_headers;
	char *message_body;
} HTTP_request_t;

typedef struct HTTP_response_s {
	char *HTTP_Version;
	char *Status_Code;
	char *Reason_Phrase;
	HTTP_header_t *general_headers;
	HTTP_header_t *response_headers;
	HTTP_header_t *entity_headers;
	char *message_body;
} HTTP_response_t;

/*
typedef struct HTTP_message_s {
	union start_line_s {
		struct Request_line_s {
			char *Method;
			char *Request_URI;
			char *HTTP_Version;
		} Request_line;
		struct Response_line_s {
			char *HTTP_Version;
			char *Status_Code;
			char *Reason_Phrase;
		} Response_line;
	} start_line;
	HTTP_header_t *general_headers;
	HTTP_header_t *request_headers;
	HTTP_header_t *response_headers;
	HTTP_header_t *entity_headers;
	char *message_body;
} HTTP_message_t;
*/

typedef struct todo_s {
	size_t id;
	char *title;
	char *description;
	struct todo_s *next;
} todo_t;


/* API_server.c */
void errorExit(int sock_fd, char *error_msg);
int newTCPIPv4Socket(uint16_t port, struct sockaddr_in *addr);
int API_server(void);
/* int main(void) */

/* parse_request.c */
char *strtokSubstr(char *str, char *delim);
HTTP_request_t *parseHTTPRequest(char *recv_str);


#endif /* API_SERVER_H */
