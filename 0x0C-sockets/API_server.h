#ifndef API_SERVER_H
#define API_SERVER_H

#include <errno.h>
/* size_t */
#include <stddef.h>
/* netinet/in.h also includes sys/types.h and sys/socket.h */
/* sys/types.h needed for socket.h portability beyond Linux / POSIX.1-2001 */
/* sockaddr_in */
#include <netinet/in.h>

#define HT_NB 5

typedef enum {
	HT_UNKNOWN,
	HT_GENERAL,
	HT_REQUEST,
	HT_RESPONSE,
	HT_ENTITY
} header_type_t;


typedef struct HTTP_header_s {
	header_type_t type;
	char *header;
	char *value;
	struct HTTP_header_s *next;
} HTTP_header_t;

typedef struct HTTP_request_s {
	char *Method;
	char *Request_URI;
	char *HTTP_Version;
	HTTP_header_t *headers;
	char *message_body;
} HTTP_request_t;

typedef struct HTTP_response_s {
	char *HTTP_Version;
	char *Status_Code;
	char *Reason_Phrase;
	HTTP_header_t *headers;
	char *message_body;
} HTTP_response_t;

/*
typedef struct HTTP_response_s {
	char *HTTP_Version;
	char *Status_Code;
	char *Reason_Phrase;
	HTTP_header_t *general_headers;
	HTTP_header_t *response_headers;
	HTTP_header_t *entity_headers;
	char *message_body;
} HTTP_response_t;
*/
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


/* socket fds made global to be seen by signal handler */
/* static to avoid linter error */
extern int server_fd;
extern int client_fd;

/* API_server.c */
void errorExit(char *error_msg);
int newTCPIPv4Socket(uint16_t port, struct sockaddr_in *addr);
int API_server(void);
/* int main(void) */

/* parse_request.c */
char *strtokSubstr(char *str, char *delim);
HTTP_request_t *parseHTTPRequest(char *recv_str);

/* responses.c */
char *get1XX_3XXReasonPhrase(unsigned int Status_Code);
char *get4XX_5XXReasonPhrase(unsigned int Status_Code);
char *getReasonPhrase(unsigned int Status_Code);
void HTTP_response(unsigned int Status_Code,
		   HTTP_header_t *message_headers, char *message_body);


#endif /* API_SERVER_H */
