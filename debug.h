#ifndef _log_h
#define _log_h
void debug(char *string, _Bool DIE);

void debugToSyslog(char *string);

void debugToStdOut(char *string);

#endif
