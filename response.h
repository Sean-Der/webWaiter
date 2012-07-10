#ifndef _response_h
#define _response_h

void sendBadHTTPRequestPage(int *clientSock, char *response);

void sendPageNotFoundPage(int *clientSock, char *requestedFile, char *response);

int sendRequestedFile(int *clientSock, char *requestedFile);

void sendRequestedDirectory(int *clientSock, char *requestedFile, char *response);

void sendPageAccessDeniedPage(int *clientSock, char *requestedFile, char *response);
#endif
