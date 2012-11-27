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

    if (parseAdd(argv[1]))
    {
        perror("ERROR: Invalid ftp string!");
        perror("Should be: ftp://[<user>:<password>@]<host>/<url-path>!");
        exit(1);
    }

    printf("String compiled sucessfully\n\n");

    char shiftedBuffer[strlen(argv[1]) - 5];
    strncpy(shiftedBuffer, argv[1] + 6, strlen(argv[1]) - 5);


    puts(shiftedBuffer);
    /* Has username and password - passing them! */

    char buffer[HOST_LENGTH];
    bzero(buffer, HOST_LENGTH);
    int foundit = 0;
    int j = 0;
    int k = 0;
    int l = 0;

    int atPosition = -1; /* -1 if before at, 1 if after or at @ */
    int colonPosition = -1; /* -1 if before colon, 1 if after or colon : */
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    bzero(username, USERNAME_LENGTH);
    bzero(password, PASSWORD_LENGTH);

    for (int i = 0; i < strlen(shiftedBuffer); i++)
    {
        if (shiftedBuffer[i] == '@') {
            atPosition = 1;
            foundit = 1;
            continue;
        }

        if (shiftedBuffer[i] == ':') {
            colonPosition = 1;
            continue;
        }

        if (atPosition == -1 && colonPosition == 1) {
            password[l] = shiftedBuffer[i];
            l++;
        }

        if (atPosition == -1 && colonPosition == -1) {
            username[k] = shiftedBuffer[i];
            k++;
        }

        if (foundit && shiftedBuffer[i] != '/') {
            buffer[j] = shiftedBuffer[i];
            j++;
        }

        if (shiftedBuffer[i] == '/') {
            foundit = 0;
            break;
        }
    }
    
    /* No user and pass: assumes anonymous and blank password*/

    foundit = 0;
    j = 0;
    if (buffer[0] == '\0')
    {
        strcpy(username, "anonymous");
        strcpy(password, "");
        
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (argv[1][i] == '/' && argv[1][i + 1] == '/') {
                foundit = 1;
                continue;
            }

            if (foundit && argv[1][i] != '/') {
                buffer[j] = argv[1][i];
                j++;
            }

            if (argv[1][i] == '/' && argv[1][i + 1] != '/' && argv[1][i - 1] != '/') {
                foundit = 0;
                break;
            }
        }
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

    printf("username %s, password %s\n", username, password);

    char * h_address = (char*) getip(buffer);

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
    printf("aqui");
    printf("rec: %i", rec);

    if(rec == -1) {
    	perror("Access denied 3! Exiting...\n");
    	exit(1);
    }

    /* Connecting to new port */
    int auxsockfd = connect_socket(h_address, rec);

    printf("fd: %i", auxsockfd);

    rec = receive(auxsockfd);

    return 0;
}

int receive_data(int sockfd) {
	char buf[MESSAGE_LENGTH];

	int status = read(sockfd, buf, MESSAGE_LENGTH);

	if(status > 0) {
		printf("%s\n", buf);
		char tempbuf[MESSAGE_LENGTH];
		bzero(tempbuf, MESSAGE_LENGTH);
		strcpy(tempbuf, buf);
		char statuscode[3];
		bzero(statuscode, 3);
		strncpy(statuscode, tempbuf, 3);
		statuscode[3] = '\0';


		int recstatus = atoi(statuscode);

		if (recstatus == PORT_STATUS) {
			int comma = 0;
			char firstnumber[10];
			char secondnumber[10];
			bzero(firstnumber, strlen(firstnumber));
			bzero(secondnumber, strlen(secondnumber));
			int j = 0;
			int k = 0;


			for(int i = 0; i < strlen(buf); i++) {

				if (buf[i] == ',') {
					comma++;
					continue;
				}
				if (comma == 4) {
					firstnumber[j] = buf[i];
					++j;
				}

				if (comma == 5) {
					secondnumber[k] = buf[i];
					k++;
				}
			}

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
