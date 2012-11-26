#ifndef GETIP_H
#define GETIP_H

#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * Returns ip respective to the host given.
 * @param host name or ip host to convert.
 * @return ip correspondent.
 */
char* getip(char * host);

#endif /* GETIP_H */