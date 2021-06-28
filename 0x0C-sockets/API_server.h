#ifndef API_SERVER_H
#define API_SERVER_H

/* size_t */
#include <stddef.h>


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

#endif /* API_SERVER_H */
