#include "API_server.h"

/* strlen strncmp strdup */
#include <string.h>
/* fprintf printf */
#include <stdio.h>
/* malloc free */
#include <stdlib.h>

#define SEND_BUFSZ 1024

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
		fprintf(stderr, "getReasonPhrase: unrecognized Status_code\n");
		return (NULL);
	};
}


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
	case 500: return ("Internal Server Error");
	case 501: return ("Not Implemented");
	case 502: return ("Bad Gateway");
	case 503: return ("Service Unavailable");
	case 504: return ("Gateway Time-out");
	case 505: return ("HTTP Version not supported");
	default:
		fprintf(stderr, "getReasonPhrase: unrecognized Status_code\n");
		return (NULL);
	};
}

char *getReasonPhrase(unsigned int Status_Code)
{
	if (Status_Code < 400)
		return (get1XX_3XXReasonPhrase(Status_Code));

	return (get4XX_5XXReasonPhrase(Status_Code));
}



void HTTP_response(unsigned int Status_Code,
		   HTTP_header_t *message_headers, char *message_body)
{
        HTTP_response_t response = {0};
	char sc_buf[5] = {0}, send_buf[SEND_BUFSZ] = {0};
	char *Reason_Phrase, *buf_temp, *err_msg = NULL;
	HTTP_header_t *hdr_temp;

	if (snprintf(sc_buf, 5, "%u", Status_Code) < 0)
	{
		err_msg = "HTTP_response: snprintf";
		Status_Code = 500;
	}

	Reason_Phrase = getReasonPhrase(Status_Code);
	if (!Reason_Phrase)
		Status_Code = 500;

        response.HTTP_Version = "HTTP/1.1";
	response.Status_Code = sc_buf;
	response.Reason_Phrase = Reason_Phrase;
	response.headers = message_headers;
	response.message_body = message_body;

	buf_temp = send_buf + sprintf(send_buf, "%s %s %s\r\n",
				      response.HTTP_Version,
				      response.Status_Code,
				      Reason_Phrase);
        for (hdr_temp = response.headers; hdr_temp; hdr_temp = hdr_temp->next)
		buf_temp += sprintf(buf_temp, "%s: %s\r\n",
				    hdr_temp->header, hdr_temp->value);
	buf_temp += sprintf(buf_temp, "\r\n");
	if (response.message_body)
		buf_temp += sprintf(buf_temp, "%s", response.message_body);

	if (send(client_fd, (void *)send_buf, strlen(send_buf), 0) == -1)
		errorExit("HTTP_response: send");

	if (Status_Code == 500)
		errorExit(err_msg);
}
