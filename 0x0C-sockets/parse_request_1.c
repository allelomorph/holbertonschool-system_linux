#include "API_server.h"

/* strlen */
#include <string.h>
/* printf */
#include <stdio.h>
/* free */
#include <stdlib.h>


header_type_t getHeaderType(char *hdr_name)
{
	int i, j, gen_hdr_ct = 8, req_hdr_ct = 19,
		res_hdr_ct = 9, ent_hdr_ct = 10;
	char *gen_hdrs[] = {"Cache-Control", "Connection", "Date",
			    "Pragma", "Trailer", "Transfer-Encoding",
			    "Upgrade", "Warning"};
	char *req_hdrs[] = {"Accept", "Accept-Charset", "Accept-Encoding",
			  "Accept-Language", "Authorization", "Expect",
			  "From", "Host", "If-Match", "If-Modified-Since",
			  "If-None-Match", "If-Range", "If-Unmodified-Since",
			  "Max-Forwards", "Proxy-Authorization", "Range",
			  "Referer", "TE", "User-Agent"};
	char *res_hdrs[] = {"Accept-Ranges", "Age", "ETag", "Location",
			  "Proxy-Authenticate", "Retry-After", "Server",
			  "Vary", "WWW-Authenticate"};
	char *ent_hdrs[] = {"Allow", "Content-Encoding", "Content-Language",
			  "Content-Length", "Content-Location", "Content-MD5",
			  "Content-Range", "Content-Type",
			  "Expires", "Last-Modified"};

	if (!hdr_name)
		return (HT_UNKNOWN);
	for (i = 0; i < gen_hdr_ct; i++)
		if (strncmp(gen_hdrs[i], hdr_name,
			    strlen(gen_hdrs[i]) + 1) == 0)
			return (HT_GENERAL);
	for (i = 0; i < req_hdr_ct; i++)
		if (strncmp(req_hdrs[i], hdr_name,
			    strlen(req_hdrs[i]) + 1) == 0)
			return (HT_REQUEST);
	for (i = 0; i < res_hdr_ct; i++)
		if (strncmp(res_hdrs[i], hdr_name,
			    strlen(res_hdrs[i]) + 1) == 0)
			return (HT_RESPONSE);
	for (i = 0; i < ent_hdr_ct; i++)
		if (strncmp(ent_hdrs[i], hdr_name,
			    strlen(ent_hdrs[i]) + 1) == 0)
			return (HT_ENTITY);
	return (HT_UNKNOWN);
}

int parseHeaders(char *recv_str, char **message_lines,
		 size_t message_line_ct, HTTP_request_t *request)
{
	size_t i, headers_end_i;
	HTTP_header_t *hdrs_head = NULL, *new_hdr, *hdrs_tail;

	if (!request || !message_lines || !recv_str)
	{
		HTTP_response(500, NULL, NULL);
		return (1);
	}

	headers_end_i = message_line_ct - 2;
	for (i = 1;  i < headers_end_i; i++)
	{
		new_hdr = malloc(sizeof(HTTP_header_t));
		if (!new_hdr)
		{
			HTTP_response(500, NULL, NULL);
			return (1);
		}
		/* validate <name>": "<value> header syntax */
		if (countTokensBySubstr(message_lines[i], ": ") != 2)
		{
			HTTP_response(400, NULL, NULL);
			return (1);
		}
		new_hdr->name = strtokSubstr(message_lines[i], ": ");
		new_hdr->value = strtokSubstr(NULL, ": ");
		new_hdr->type = getHeaderType(new_hdr->name);

		if (!hdrs_head)
			hdrs_tail = hdrs_head = new_hdr;
		else
		{
			hdrs_tail->next = new_hdr;
			new_hdr->next = NULL;
			hdrs_tail = new_hdr;
		}
	}

	return (0);
}


int parseRequestLine(char *recv_str, char **message_lines,
		     HTTP_request_t *request)
{
	if (!request || !message_lines || !recv_str)
	{
		HTTP_response(500, NULL, NULL);
		return (1);
	}

	request->Method = strtok(message_lines[0], " ");
	request->Request_URI = strtok(NULL, " ");
	request->HTTP_Version = strtok(NULL, " ");
	/* validate expected HTTP/1.1 syntax of 3 SP-delimited tokens */
	if (!request->Method || !strlen(request->Method) ||
	    !request->Request_URI || !strlen(request->Request_URI) ||
	    !request->HTTP_Version || !strlen(request->HTTP_Version))
	{
		HTTP_response(400, NULL, NULL);
		return (1);
	}
	/* RFC 2616 recommends URIs <= 255 chars for legacy compliance */
	if (strlen(request->Request_URI) > 255)
	{
		HTTP_response(414, NULL, NULL);
		return (1);
	}

	request->Request_URI = strtok(request->Request_URI, "?");
	request->URI_query = strtok(NULL, "?");

	if (!isMethodImplemented(request->Method) ||
	    !isPathRecognized(request->Request_URI))
	{
		HTTP_response(404, NULL, NULL);
		return (1);
	}
	if (!isVersionSupported(request->HTTP_Version))
	{
		HTTP_response(505, NULL, NULL);
		return (1);
	}

	return(0);
}


char **parseMessageLines(char *recv_str, HTTP_request_t *request,
			 size_t *message_line_ct)
{
	char **message_lines = NULL;

	if (!request || !recv_str)
	{
		HTTP_response(500, NULL, NULL);
		return (NULL);
	}

	/* ignore leading CRLFs */
	while (*recv_str == '\r' && *(recv_str + 1) == '\n')
		recv_str += 2;

	message_lines = tokenizeBySubstr(recv_str, "\r\n", &message_line_ct);
	if (!message_lines)
	{
	        HTTP_response(500, NULL, NULL);
		return (NULL);
	}

	/* validate expected HTTP/1.1 syntax of CRLF-delimited lines */
	if (message_line_ct < 2 || !strlen(message_lines[0]) ||
	    strlen(message_lines[message_line_ct - 2]))
	{
		HTTP_response(400, NULL, NULL);
		free(message_lines);
		return (NULL);
	}

	request->message_body = message_lines[message_line_ct - 1];

	return (message_lines);
}


/**
 * parseHTTPRequest - parses raw HTTP request to populate request data struct
 *
 * @recv_str: raw request string recieved from client
 * Return: pointer to populated HTTP_request_s, or NULL on failure
 */
HTTP_request_t *parseHTTPRequest(char *recv_str)
{
	char **message_lines;
	size_t message_line_ct;
	HTTP_request_t *request = NULL;

	request = malloc(sizeof(HTTP_request_t));
	if (!request || !recv_str)
	{
		HTTP_response(500, NULL, NULL);
		return (NULL);
	}
	memset(request, 0, sizeof(HTTP_request_t));

        message_lines = parseMessageLines(recv_str, request,
					  &message_line_ct);
	if (!message_lines)
	{
		free(request);
		return (NULL);
	}

        if (parseRequestLine(recv_str, message_lines, request) != 0 ||
	    parseHeaders(recv_str, message_lines,
			 message_line_ct, request) != 0)
	{
		free(request);
		free(message_lines);
		return (NULL);
	}

	free(message_lines);
	return (request);
}
