#include "ftpClient.h"
#include "clientTCP.h"

int main(int argc, char * argv[])
{
    if (argc <= 1 || argc > 2)
    {
        perror("ERROR: Invalid usage!");
        perror("Usage: client ftp://[<user>:<password>@]<host>/<url-path>!");
        exit(1);
    }

    /* Checks if well writen usage */
    if (parseAdd(argv[1]))
    {
        perror("ERROR: Invalid ftp string!");
        perror("Should be: ftp://[<user>:<password>@]<host>/<url-path>!");
        exit(1);
    }

    printf("String compiled sucessfully\n\n");

    /* Removes "ftp://" from string */
    char ftpFreeBuffer[strlen(argv[1]) - 5];
    strncpy(ftpFreeBuffer, argv[1] + 6, strlen(argv[1]) - 5);

    /* Defines strings to use and cleans them */
    char username[USERNAME_LENGTH];
    bzero(username, USERNAME_LENGTH);
    char password[PASSWORD_LENGTH];
    bzero(password, PASSWORD_LENGTH);
    char host[HOST_LENGTH];
    bzero(host, HOST_LENGTH);
    char url[URL_LENGTH];
    bzero(url, URL_LENGTH);

    /* Checks if has password and username defined */
    char * hasuser = strchr(ftpFreeBuffer, '@');

    if (hasuser != NULL) {
    	/* Copys localy string to parse */
    	char shiftedBuffer[strlen(argv[1]) - 5];
    	strcpy(shiftedBuffer, ftpFreeBuffer);

    	/* Find and get username */
		char * p = strchr(shiftedBuffer, ':');

		int position = (int)(p-shiftedBuffer);
		strncpy(username, shiftedBuffer, position);

		/* Shifts string to get password */
		strncpy(shiftedBuffer, shiftedBuffer + position + 1, strlen(shiftedBuffer)-position);

		/* Finds and gets password */
		p = strchr(shiftedBuffer, '@');
		position = (int)(p-shiftedBuffer);
		strncpy(password, shiftedBuffer, position);

		/* Shifts string to get host */
		strncpy(shiftedBuffer, shiftedBuffer + position + 1, strlen(shiftedBuffer)-position);

		/* Gets hostname */
		p = strchr(shiftedBuffer, '/');
		position = (int)(p-shiftedBuffer);
		strncpy(host, shiftedBuffer, position);

		/* Shifts string to get url */
		strncpy(shiftedBuffer, shiftedBuffer + position + 1, strlen(shiftedBuffer)-position);

		/* Gets url path */
		strcpy(url, shiftedBuffer);


		printf("host %s, username %s, pass %s, url %s\n", host, username, password, url);
    }
    else {
		/* Copys localy string to parse */
		char shiftedBuffer[strlen(argv[1]) - 5];
		strcpy(shiftedBuffer, ftpFreeBuffer);

		/* Username and Password defined */
		strcpy(username, "anonymous");
		strcpy(password, "");

		/* Gets hostname */
		char * p = strchr(shiftedBuffer, '/');
		int position = (int)(p-shiftedBuffer);
		strncpy(host, shiftedBuffer, position);

		/* Shifts string to get url */
		strncpy(shiftedBuffer, shiftedBuffer + position + 1, strlen(shiftedBuffer)-position);

		/* Gets url path */
		strcpy(url, shiftedBuffer);

		printf("secopdn host %s, username %s, pass %s, url %s\n", host, username, password, url);
    }

    char user[USERNAME_LENGTH];
    bzero(user, USERNAME_LENGTH);
    strcpy(user, "USER ");
    strcat(user, username);
    strcat(user, "\r\n");

    char pass[PASSWORD_LENGTH];
    bzero(pass, PASSWORD_LENGTH);
    strcpy(pass, "PASS ");
    strcat(pass, password);
    strcat(pass, "\r\n");

    printf("username %s, password %s\n", user, pass);

    char * h_address = (char*) getip(host);

    int sockfd = connect_socket(h_address, 21);

    /* Receives first message */
    int rec = receive(sockfd);
    if (rec > 3) {
    	perror("Access denied 1! Exiting...\n");
    	exit(1);
    }

    /* Sending username */
    write(sockfd, user, strlen(user));
    rec = receive(sockfd);
    if (rec == -1 || rec > 3) {
    	perror("Access denied 1! Exiting...\n");
    	exit(1);
    }

    /* Sending password */
    write(sockfd, pass, strlen(pass));
    rec = receive(sockfd);
    if (rec == -1 || rec > 6) {
		perror("Access denied 2! Exiting...\n");
		exit(1);
	}

    /* Sending pasv */
    char pasv[MESSAGE_LENGTH];
    bzero(pasv, MESSAGE_LENGTH);

    write(sockfd, "pasv\r\n", 6);
    rec = receive_data(sockfd);
    if(rec == -1) {
    	perror("Access denied 3! Exiting...\n");
    	exit(1);
    }

    printf("Connecting to %d!\n", rec);

    /* Connecting to new port */
    int auxsockfd = connect_socket(h_address, rec);
    rec = receive(auxsockfd);
    if(rec == -1) {
		perror("Access denied 4! Exiting...\n");
		exit(1);
	}

    printf("Connected!\n");



    return 0;
}

int receive_data(int sockfd) {
	char buf[MESSAGE_LENGTH];
	bzero(buf, MESSAGE_LENGTH);

	int status = read(sockfd, buf, MESSAGE_LENGTH);

	if(status > 0) {
		printf("%s\n", buf);

		/* Gets status */
		char tempBuf[strlen(buf)];
		bzero(tempBuf, strlen(buf));
		strcpy(tempBuf, buf);
		char statuscode[3];
		bzero(statuscode, 3);
		strncpy(statuscode, tempBuf, 3);
		statuscode[3] = '\0';

		int recstatus = atoi(statuscode);

		char firstnumber[10];
		char secondnumber[10];
		bzero(firstnumber, strlen(firstnumber));
		bzero(secondnumber, strlen(secondnumber));

		/* Checks if coded received is status good to continue*/
		if (recstatus == PORT_STATUS) {
			/* Shifts string to the first "," */
			char * p = strchr(tempBuf, ',');
			int position = (int)(p-tempBuf);
			strncpy(tempBuf, tempBuf + position + 1, strlen(tempBuf)-position);

			/* Shifts string to the second "." */
			p = strchr(tempBuf, ',');
			position = (int)(p-tempBuf);
			strncpy(tempBuf, tempBuf + position + 1, strlen(tempBuf)-position);

			/* Shifts string to the third "." */
			p = strchr(tempBuf, ',');
			position = (int)(p-tempBuf);
			strncpy(tempBuf, tempBuf + position + 1, strlen(tempBuf)-position);

			/* Shifts string to the fourth "," */
			p = strchr(tempBuf, ',');
			position = (int)(p-tempBuf);
			strncpy(tempBuf, tempBuf + position + 1, strlen(tempBuf)-position);

			/* Gets first number*/
			p = strchr(tempBuf, ',');
			position = (int)(p-tempBuf);
			strncpy(firstnumber, tempBuf, position);

			/* Shifts to the fifth "," */
			strncpy(tempBuf, tempBuf + position + 1, strlen(tempBuf)-position);

			/* Gets second number */
			p = strchr(tempBuf, ')');
			position = (int)(p-tempBuf);
			strncpy(secondnumber, tempBuf, position);

			int number1 = atoi(firstnumber);
			int number2 = atoi(secondnumber);

			printf("number1: %d, number2: %d\n", number1, number2);

			int port = number1*256+number2;

			printf("port: %i\n", port);
			return port;
		}
	}

	return -1;
}

int receive(int sockfd) {
	char buf[MESSAGE_LENGTH];
	int recstatus;

	int status = read(sockfd, buf, MESSAGE_LENGTH);

	printf("status: %d\n", status);

	if(status > 0) {
		printf("%s\n", buf);

		char statuscode[3];
		strncpy(statuscode, buf, 3);
		statuscode[3] = '\0';

		char firstint[2];
		firstint[0] = statuscode[0];
		firstint[1] = '\0';

		recstatus = atoi(firstint);

		return recstatus;
	}

	return -1;
}

int parseAdd(char * address)
{
    regex_t preg;
    size_t nmatch = strlen(address);
    regmatch_t pmatch[nmatch];
    int totalMatch;
    int matchesBegin;

    if (regcomp(&preg, "ftp://(([A-Za-z0-9])*:([A-Za-z0-9])*@)*([A-Za-z0-9.])+/([[A-Za-z0-9/])+", REG_EXTENDED) != 0)
    {
        perror("Could not compile regular expression");
        exit(1);
    }

    regexec(&preg, address, nmatch, pmatch, REG_EXTENDED);

    regfree(&preg);

    totalMatch = pmatch[0].rm_eo;
    matchesBegin = pmatch[0].rm_so;

    if (totalMatch != nmatch || matchesBegin != 0)
    {
        return 1;
    }

    return 0;
}
