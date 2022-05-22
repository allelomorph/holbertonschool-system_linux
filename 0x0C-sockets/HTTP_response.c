#include "API_server.h"

/* strlen strncmp strdup */
#include <string.h>
/* fprintf printf */
#include <stdio.h>
/* malloc free */
#include <stdlib.h>
/* close */
#include <unistd.h>


/**
 * get1XX_3XXReasonPhrase - helper to getReasonPhrase, finds HTTP/1.1 defined
 *   Reason-Phrase for a given Status-Codes up to 400
 *
 * @Status_Code: code indicating state of server or issue with last request
 * Return: matching Reason-Phrase, or NULL if none found
 */
char *get1XX_3XXReasonPhrase(unsigned int Status_Code)
{
	switch (Status_Code)
	{
	case 100: return ("Continue");
	case 101: return ("Switching Protocols");
	case 200: return ("OK");
	case 201: return ("Created");
	case 202: return ("Accepted");
	case 203: return ("Non-Authoritative Information");
	case 204: return ("No Content");
	case 205: return ("Reset Content");
	case 206: return ("Partial Content");
	case 300: return ("Multiple Choices");
	case 301: return ("Moved Permanently");
	case 302: return ("Found");
	case 303: return ("See Other");
	case 304: return ("Not Modified");
	case 305: return ("Use Proxy");
	case 307: return ("Temporary Redirect");
	default:
		fprintf(stderr, "getReasonPhrase: unknown Status_Code %u\n",
			Status_Code);
		return (NULL);
	};
}


/**
 * get4XX_5XXReasonPhrase - helper to getReasonPhrase, finds HTTP/1.1 defined
 *   Reason-Phrase for a given Status-Code at or above 400
 *
 * @Status_Code: code indicating state of server or issue with last request
 * Return: matching Reason-Phrase, or NULL if none found
 */
char *get4XX_5XXReasonPhrase(unsigned int Status_Code)
{
	switch (Status_Code)
	{
	case 400: return ("Bad Request");
	case 401: return ("Unauthorized");
	case 402: return ("Payment Required");
	case 403: return ("Forbidden");
	case 404: return ("Not Found");
	case 405: return ("Method Not Allowed");
	case 406: return ("Not Acceptable");
	case 407: return ("Proxy Authentication Required");
	case 408: return ("Request Time-out");
	case 409: return ("Conflict");
	case 410: return ("Gone");
	case 411: return ("Length Required");
	case 412: return ("Precondition Failed");
	case 413: return ("Request Entity Too Large");
	case 414: return ("Request-URI Too Large");
	case 415: return ("Unsupported Media Type");
	case 416: return ("Requested range not satisfiable");
	case 417: return ("Expectation Failed");
	case 422: return ("Unprocessable Entity");
		/* 422 is not part of RFC 2616, appears in RFC 4918 */
	case 500: return ("Internal Server Error");
	case 501: return ("Not Implemented");
	case 502: return ("Bad Gateway");
	case 503: return ("Service Unavailable");
	case 504: return ("Gateway Time-out");
	case 505: return ("HTTP Version not supported");
	default:
		fprintf(stderr, "getReasonPhrase: unknown Status_Code %u\n",
			Status_Code);
		return (NULL);
	};
}


/**
 * getReasonPhrase - finds HTTP/1.1 defined Reason-Phrase for a given
 *   Status-Code
 *
 * @Status_Code: code indicating state of server or issue with last request
 * Return: matching Reason-Phrase, or NULL if none found
 */
char *getReasonPhrase(unsigned int Status_Code)
{
	if (Status_Code < 400)
		return (get1XX_3XXReasonPhrase(Status_Code));

	return (get4XX_5XXReasonPhrase(Status_Code));
}


/**
 * HTTP_response - sends HTTP/1.1 formatted response to client
 *
 * @Status_Code: code indicating state of server or issue with last request
 * @message_headers: SLL of HTTP_header_t headers, or NULL if none
 * @message_body: message-body, could be "" or NULL if none
 */
void HTTP_response(unsigned int Status_Code,
		   HTTP_header_t *message_headers, char *message_body)
{
	char send_buf[SEND_BUFSZ] = {0};
	char *HTTP_Version = "HTTP/1.1", *Reason_Phrase, *buf_temp;
	HTTP_header_t *hdr_temp;

	Reason_Phrase = getReasonPhrase(Status_Code);
	if (!Reason_Phrase)
	{
		Status_Code = 500;
		Reason_Phrase = getReasonPhrase(Status_Code);
		message_headers = NULL;
		message_body = NULL;
	}

	buf_temp = send_buf + sprintf(send_buf, "%s %u %s\r\n",
				      HTTP_Version, Status_Code,
				      Reason_Phrase);

	for (hdr_temp = message_headers; hdr_temp; hdr_temp = hdr_temp->next)
		buf_temp += sprintf(buf_temp, "%s: %s\r\n",
				    hdr_temp->name, hdr_temp->value);

	buf_temp += sprintf(buf_temp, "\r\n");

	if (message_body)
		buf_temp += sprintf(buf_temp, "%s", message_body);

	if (send(client_fd, (void *)send_buf, strlen(send_buf), 0) == -1)
		errorExit("HTTP_response: send");

	if (close(client_fd) != 0)
		errorExit("HTTP_response: close");
	client_fd = -1;

#if SRC_VERSION >= 8
	printf("-> %u %s\n", Status_Code, Reason_Phrase);
#endif
}
