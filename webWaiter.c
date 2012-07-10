#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "webWaiter.h"
#include "config.h"
#include "startup.h"
#include "request.h"
#include "debug.h"

void *serveRequest(void *param)
{
	int *clientSockPtr = (int *) param; char *response = calloc(BUFFERSIZE,sizeof(char));  char *request = calloc(BUFFERSIZE,sizeof(char));
	if (getRequestHeaders(clientSockPtr, response, request))
		fullfillRequestType(clientSockPtr, request, response);	
	
	close(*clientSockPtr); free(clientSockPtr); free(request); free(response); 

	return NULL;	
}

int main(void)
{
	debug("webWaiter was compiled with verbose debugging enabled\n", 0);	
	chrootSrvRoot();	
	bindSocket();
	listenSocket();
	acceptRequests();
	return 0;
}
