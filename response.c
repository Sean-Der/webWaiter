#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>

#include "config.h"
#include "response.h"
#include "debug.h"

void sendBadHTTPRequestPage(int *clientSock, char *response)
{
	snprintf(response, (sizeof(char *) * BUFFERSIZE), "%s",
		"<html><body>\n"
		"<h2>Malformed HTTP Request</h2>\n"
		"webWaiter requires that all requests specify an operation (GET is the only one supported at this time).\n"
		"</body></html>");		
	
	 if (send(*clientSock, response, strlen(response) * sizeof(char), 0) == -1)
                perror("send");
}
int sendRequestedFile(int *clientSock, char *requestedFile)
{
	char * fileContents; FILE * pFile; long lSize; size_t result;
	
	pFile = fopen (requestedFile , "rb" );
	fseek(pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);
	
	fileContents = (char *) malloc (sizeof(char) * lSize);
	result = fread(fileContents ,1,lSize,pFile);
	
	if(!result)
		return 0;
	
	if (send(*clientSock, fileContents, result, 0) == -1)
                perror("send");
	
	free(fileContents); fclose(pFile);
	return 1;
}	

void sendRequestedDirectory(int *clientSock, char *requestedDirectory, char *response)
{
	snprintf(response, BUFFERSIZE, "%s%s%s", 
		"\n"
		"<html><body>\n"
		"<h2>Directory Listing Of ",
		requestedDirectory,
		" </h2>\n");
	
	struct dirent **namelist;
	int n;
	
	if ((n = scandir(requestedDirectory , &namelist, 0, alphasort)) < 0) {
		//printf("Unable to open %s : %s\n", strerror(errno), whichDir);
	} else {
		while (n > 1) {
			--n;
			strncat(response, "<a href=\"", BUFFERSIZE);	
			strncat(response, requestedDirectory, BUFFERSIZE);
			if (strncmp(requestedDirectory, "/", 2) != 0){
				strncat(response, "/", BUFFERSIZE);
			}
			strncat(response, namelist[n]->d_name, BUFFERSIZE);
			strncat(response, " \"> ", BUFFERSIZE);	
			strncat(response, namelist[n]->d_name, BUFFERSIZE);	
			strncat(response, " </a> <br />", BUFFERSIZE);	
			free(namelist[n]);	
		}
		free(namelist[n-1]);
		free(namelist);
	}			
	strncat(response, "</table> </body> </html>", BUFFERSIZE);

	if (send(*clientSock, response, strlen(response) * sizeof(char), 0) == -1 )
      	 	perror("send");
}
	
void sendPageNotFoundPage(int *clientSock, char *requestedFile, char *response)
{
	snprintf(response, (sizeof(char *) * BUFFERSIZE), "%s%s%s", 
		"<html><body>\n"
		"<h2>Page Not Found</h2>\n"
		"webWaiter was not able to find the file ",
		requestedFile,
	       	"</body></html>");
	
	if (send(*clientSock, response, strlen(response) * sizeof(char), 0) == -1)
		perror("send");
}
void sendPageAccessDeniedPage(int *clientSock, char *requestedFile, char *response)
{
	snprintf(response, (sizeof(char *) * BUFFERSIZE), "%s%s%s", 
		"<html><body>\n"
		"<h2>Access Denied</h2>\n"
		"webWaiter thinks snooping is rude, so you are not allowed to see ",
		requestedFile,
	       	"</body></html>");
	
	if (send(*clientSock, response, strlen(response) * sizeof(char), 0) == -1)
		perror("send");
}
