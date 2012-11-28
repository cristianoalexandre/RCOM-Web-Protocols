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
#include <fcntl.h>

#define HOST_LENGTH 64
#define USERNAME_LENGTH 255
#define PASSWORD_LENGTH 255
#define MESSAGE_LENGTH 255
#define URL_LENGTH 255
#define FILE_LENGTH
#define PORT_STATUS 227
#define MAXLENGTH 1024

/**
 * FTP Status Codes
 *
 * 1xx	Positive Preliminary Code
 * 2xx	Positive Completion reply
 * 3xx	Positive Intermediate reply
 * 4xx	Transient Negative Completion reply
 * 5xx	Permanent Negative Completion reply
 * 6xx	Protected reply
 *
 *
 * 100		The requested action is being initiated,
 * expect another reply before proceeding with a new command.
 * 200		The requested action has been successfully completed.
 * 300		The command has been accepted, but the requested action is on hold, pending receipt of further information.
 * 400 		The command was not accepted and the requested action did not take place,
 * but the error condition is temporary and the action may be requested again.
 * 500		Syntax error, command unrecognized and the requested action did not take place.
 * This may include errors such as command line too long.
 * 600		Replies regarding confidentiality and integrity
 * 10000	Common Winsock Error Codes
 */

/**
 * Checks if address is in type: ftp://[<user>:<password>@]<host>/<url-path>
 * @param address buffer with address to check
 * @return 0 if yes, 1 otherwise
 */
int parseAdd(char * address);

/**
 * Receives message from socket
 * @param sockfd file descriptor of socket to receive message
 * @return status
 */
int receive(int sockfd);

/**
 * Receives data with next port to connect
 * @param sockfd file descriptor of socket to receive message
 * @return port to connect
 */
int receive_data(int sockfd);

/**
 * Writes information to a file.
 * @param sockfd file descriptor of socket to read message
 * @param file file to write information
 */

int writefile (int sockfd, char * file);


#endif /* FTPCLIENT_H */
