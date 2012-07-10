#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>

#include "config.h"
#include "debug.h"

void debug(char *string, _Bool DIE)
{
	
	if(SYSLOG) debugToSyslog(string);
	if(STDOUTLOG) debugToStdOut(string);	
	
	if(DIE){ 
		exit(EXIT_FAILURE);
		pthread_exit(NULL);
	}	
}

void debugToSyslog(char *string)
{
	openlog ("webWaiter", LOG_PID , LOG_LOCAL2);	
	syslog(LOG_NOTICE, string);
}

void debugToStdOut(char *string)
{
	printf("%s\n", string);
}
