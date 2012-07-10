#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "config.h"
#include "startup.h"
#include "webWaiter.h"
#include "debug.h"

int sockfd, *clientSockPtr;

void chrootSrvRoot(void)
{
	if (chdir(srvRoot) != 0){
		sprintf(debugString,"%s%s%s%s\n", "Error '",strerror(errno),"' when changing to your srvRoot ", srvRoot);
		debug(debugString, 1);	
	}
     	if (chroot(srvRoot) != 0) {
       		sprintf(debugString,"%s%s\n", "Error chrooting srvRoot", strerror(errno));
		debug(debugString, 1);
	}
}
void bindSocket(void)
{
	struct addrinfo hints, *servInfo;	
	int errcode, yes=1;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; 
	
	if ((errcode = getaddrinfo(NULL, PORT, &hints, &servInfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(errcode));
		exit(1);	
	}
	if ((sockfd = socket(servInfo->ai_family, servInfo->ai_socktype, servInfo->ai_protocol)) == -1){
		perror("server: socket");
		exit(1);	
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1); 
	}

	if (bind(sockfd, servInfo->ai_addr, servInfo->ai_addrlen) == -1) {
		close(sockfd);
		perror("server: bind");
		exit(1); 
	}
	
	freeaddrinfo(servInfo);
	
}
void listenSocket(void)
{
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);	
	}
}
void acceptRequests(void)
{
	struct sockaddr_storage their_addr;    
	pthread_t newthread;
	socklen_t sin_size = sizeof(their_addr);	
	char hostAddress[100];

	signal(SIGINT, sighandler);	
	debug("WebWaiter is now ready to accept requests!", 0);

	while(1) { 
		clientSockPtr = (int *)malloc(sizeof(int)); 
		*clientSockPtr = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);        
		
		getnameinfo((struct sockaddr *)&their_addr, sin_size, hostAddress, sizeof(hostAddress), NULL, 0, NI_NUMERICHOST); 
		sprintf(debugString, "%s %s", "webWaiter just accepted a request from", hostAddress);
		debug(debugString, 0);
		
		pthread_create(&newthread , NULL, (void * ) serveRequest, clientSockPtr); 
		pthread_detach(newthread);	
	}
}

void sighandler(int sig)
{
		
	close(sockfd);	
	free(clientSockPtr);	
	debug("Exit caused from Ctrl-C", 1);	
}
