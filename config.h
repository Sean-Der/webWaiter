#ifndef _config_h
#define _config_h
/*
 * The following variables are all config related and are safe to change
 */
#define BACKLOG 10
#define PORT "8080"
#define SYSLOG 0
#define STDOUTLOG 1
#define BUFFERSIZE 4096
#define srvRoot "/home/sean/public_html/"
/* Disable chroot at your own risk!  */
#define doChroot 0
#define indexFile "index.html"
/*
 * End Config!
 */

#endif
