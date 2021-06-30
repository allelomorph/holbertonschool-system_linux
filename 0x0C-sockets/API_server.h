#ifndef API_SERVER_H
#define API_SERVER_H

#include <errno.h>
/* size_t */
#include <stddef.h>
/* netinet/in.h also includes sys/types.h and sys/socket.h */
/* sys/types.h needed for socket.h portability beyond Linux / POSIX.1-2001 */
/* sockaddr_in */
#include <netinet/in.h>


#define MAX_PENDING 10
#define RECV_BUFSZ 1024
#define SEND_BUFSZ 1024
#define HT_NB 5


/**
 * enum header_type_e - enumerates the different HTTP header types.
 *
 * @HT_UNKNOWN: header type unknown
 * @HT_GENERAL: general-header
 * @HT_REQUEST: request-header
 * @HT_RESPONSE: response-header
 * @HT_ENTITY: entity-header
 */
typedef enum header_type_e
{
	HT_UNKNOWN,
	HT_GENERAL,
	HT_REQUEST,
	HT_RESPONSE,
	HT_ENTITY
} header_type_t;

/**
 * struct HTTP_header_s - contains type, key and value for HTTP header
 *
 * @type: header type
 * @name: header name/key
 * @value: header value (may be multiple comma delimited for select headers)
 * @next: pointer to next node in SLL, or NULL at tail
 */
typedef struct HTTP_header_s
{
	header_type_t type;
	char *name;
	char *value;
	struct HTTP_header_s *next;
} HTTP_header_t;

/**
 * struct HTTP_request_s - contains values parsed from HTTP request received
 *
 * @Method: HTTP method requested
 * @Request_URI: path to resource [ + url-encoded query ]
 * @HTTP_Version: HTTP version, should be "HTTP/1.1" for this implementation
 * @URI_query: URL-encoded query '?'-delimited from Request-URI
 * @headers: SLL of HTTP_header_t headers
 * @message_body: message-body, may be NULL or ""
 */
typedef struct HTTP_request_s
{
	char *Method;
	char *Request_URI;
	char *HTTP_Version;
	char *URI_query;
	HTTP_header_t *headers;
	char *message_body;
} HTTP_request_t;

/**
 * struct HTTP_response_s - contains values to be sent as HTTP response
 *
 * @HTTP_Version: HTTP version, should be "HTTP/1.1" for this implementation
 * @Status_Code: repsonse code, indicates state of server at time of response
 * @Reason_Phrase: describes response code
 * @headers: SLL of HTTP_header_t headers
 * @message_body: message-body, may be NULL or ""
 */
typedef struct HTTP_response_s
{
	char *HTTP_Version;
	char *Status_Code;
	char *Reason_Phrase;
	HTTP_header_t *headers;
	char *message_body;
} HTTP_response_t;

/**
 * struct todo_s - simple in-memory representation of todo list storage for API
 *   (normally would be file storage or DB in real application)
 *
 * @id: unique id number for every allocated todo struct since process started
 * @title: title of todo item
 * @description: description of todo item
 * @next: pointer to next node in SLL, or NULL at tail
 */
typedef struct todo_s
{
	size_t id;
	char *title;
	char *description;
	struct todo_s *next;
} todo_t;


/* socket fds made global to be seen by signal handler */
/* definition in *-API_server.c */
extern int server_fd;
extern int client_fd;

/* global head of list, storage in process memory instead of file or DB */
/* definition in todos.c */
extern todo_t *todos;


/* *-API_server.c */
void errorExit(char *error_msg);
void listenTCPIPv4Socket(uint16_t port);
int API_server(void);
/* int main(void) */

/* setSigHandler.c */
void SIGINT_SIGTERM_handler(int signum);
void setSigHandler(void);

/* tokenizeBySubstr.c */
char *strtokSubstr(char *str, const char *delim);
size_t countTokensBySubstr(const char *str, const char *delim);
char **tokenizeBySubstr(char *str, const char *delim, size_t *token_ct);

/* parseRequest.c */
header_type_t getHeaderType(char *hdr_name);
int parseHeaders(char *recv_str, char **message_lines,
		 size_t message_line_ct, HTTP_request_t *request);
char **parseMessageLines(char *recv_str, HTTP_request_t *request,
			 size_t *message_line_ct);
HTTP_request_t *parseRequest(char *recv_str);

/* HTTP_response.c */
char *get1XX_3XXReasonPhrase(unsigned int Status_Code);
char *get4XX_5XXReasonPhrase(unsigned int Status_Code);
char *getReasonPhrase(unsigned int Status_Code);
void HTTP_response(unsigned int Status_Code,
		   HTTP_header_t *message_headers, char *message_body);

/* *-valid_fields.c */
int isMethodImplemented(const char *Method);
int isPathValid(const char *Request_URI);

/* parseRequestLine.c */
int isVersionSupported(const char *HTTP_Version);
int parseRequestLine(char *recv_str, char **message_lines,
		     HTTP_request_t *request);

/* ?-printRequest.c */
void printRequest(HTTP_request_t *request);

/* cleanup.c */
void freeRequest(HTTP_request_t *request);
void freeTodos(void);

/*
 * typedef struct HTTP_response_s {
 *	char *HTTP_Version;
 *	char *Status_Code;
 *	char *Reason_Phrase;
 *	HTTP_header_t *general_headers;
 *	HTTP_header_t *response_headers;
 *	HTTP_header_t *entity_headers;
 *	char *message_body;
 *} HTTP_response_t;
 */
/*
 *typedef struct HTTP_message_s {
 *	union start_line_s {
 *		struct Request_line_s {
 *			char *Method;
 *			char *Request_URI;
 *			char *HTTP_Version;
 *		} Request_line;
 *		struct Response_line_s {
 *			char *HTTP_Version;
 *			char *Status_Code;
 *			char *Reason_Phrase;
 *		} Response_line;
 *	} start_line;
 *	HTTP_header_t *general_headers;
 *	HTTP_header_t *request_headers;
 *	HTTP_header_t *response_headers;
 *	HTTP_header_t *entity_headers;
 *	char *message_body;
 *} HTTP_message_t;
 */

#endif /* API_SERVER_H */
