#ifndef _request_h
#define _request_h

int getRequestHeaders(int *clientSock, char *response, char *request);
void fullfillRequestType(int *clientSock, char *HTTPHeaders, char *request);
#endif
