#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>

#include "config.h"
#include "request.h"
#include "response.h"
#include "debug.h"

int getRequestHeaders(int *clientSock, char *response, char *request)
{
	if (recv(*clientSock, request, (sizeof(char *) * BUFFERSIZE), 0) <= 0) {		
		//printf("initial request: %s", strerror(errno));
		return 0;
	}	
	
	if ( (strstr(request, "HTTP" ) == NULL) || (strstr(request, "Host:" ) == NULL) ){
		debug("We had an invalid HTTP Request", 0);
		sendBadHTTPRequestPage(clientSock, response);	
		return 0;
	}	
	
	debug(request, 0);
	return 1;
}

void fullfillRequestType(int *clientSock, char *HTTPHeaders, char *response)
{
	DIR *dp;	
	char *requestValue, requestedFile[BUFFERSIZE] = "\0", requestedIndex[BUFFERSIZE] = "\0";
	if ((requestValue = strstr(HTTPHeaders, "GET")) != NULL){
		//We start at 4 to consume "GET " , also note the ascii value of ' ' is 32
		for(int i = 4; requestValue[i] != 32; i++){	
			//If chroot is disabled we still want to make it respect our srvRoot
      if(!doChroot && i == 4 && requestValue[i] == 47){
        strcpy(requestedFile, srvRoot);
      } else {
        requestedFile[strlen(requestedFile)] = requestValue[i];
      }
          }
		requestedFile[strlen(requestedFile)+1] = '\0';	
		strcpy(requestedIndex, requestedFile);
		
		//debug(strcat("We had a GET request for %s", requestedFile));	
		
		if ((dp = opendir(requestedFile)) != NULL) {
			if(indexFile && (opendir(strcat(requestedIndex, indexFile)) == NULL) && (errno == ENOTDIR)) {
				sendRequestedFile(clientSock, requestedIndex);			
			} else {
				sendRequestedDirectory(clientSock, requestedFile, response);
			}
		} else if (errno == ENOTDIR){
			if(!sendRequestedFile(clientSock, requestedFile))
				sendPageAccessDeniedPage(clientSock, requestedFile, response);
		} else {
			sendPageNotFoundPage(clientSock, requestedFile, response);
		} 
	
	free(dp);	
	}	
}
