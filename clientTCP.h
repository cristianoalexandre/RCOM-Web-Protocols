#ifndef CLIENTTCP_H
#define	CLIENTTCP_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>

#define SERVER_PORT 80

int sendMessage(char* hostip, char* buf);

#endif	/* CLIENTTCP_H */