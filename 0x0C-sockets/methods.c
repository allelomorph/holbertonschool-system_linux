#include "API_server.h"



void methodPOST(HTTP_request *request)
{
}


void methodGET(HTTP_request *request)
{
}


void methodHEAD(HTTP_request *request)
{
}


void methodDELETE(HTTP_request *request)
{
}


void runMethod(HTTP_request *request)
{
	if (!request)
	{
		HTTP_response(500, NULL, NULL);
		return;
	}

	if (strncmp("POST", request->Method, strlen("POST")) == 0)
		return (methodPOST(request));

	if (strncmp("GET", request->Method, strlen("GET")) == 0)
		return (methodGET(request));

	if (strncmp("HEAD", request->Method, strlen("HEAD")) == 0)
		return (methodHEAD(request));

	if (strncmp("DELETE", request->Method, strlen("DELETE")) == 0)
		return (methodDELETE(request));

	/* redundant check, methods already filtered by isMethodImplemented() */
	HTTP_response(404, NULL, NULL);
}
