#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include "getip.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>
#include <strings.h>

#define HOST_LENGTH 64
#define USERNAME_LENGTH 255
#define PASSWORD_LENGTH 255

/**
 * Checks if address is in type: ftp://[<user>:<password>@]<host>/<url-path>
 * @param address buffer with address to check
 * @return 0 if yes, 1 otherwise
 */
int parseAdd(char * address);

#endif /* FTPCLIENT_H */