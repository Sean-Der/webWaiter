#ifndef _startup_h
#define _startup_h

void chrootSrvRoot(void);
void bindSocket(void);
void listenSocket(void);
void acceptRequests(void);
void sighandler(int sig);


#endif
