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

    char teste[strlen(argv[1])];
    bzero(teste, strlen(argv[1]));
    char * bar = strrchr(argv[1], '/');

    char file[MAXLENGTH];

    /* Removes last bar if exists and gets file */
    if (argv[1][strlen(argv[1]) - 1] == '/')
    {
        strncpy(teste, argv[1], strlen(argv[1]) - 1);
        bzero(argv[1], strlen(argv[1]));
        strcpy(argv[1], teste);

        bar = strrchr(argv[1], '/');
        int position = (int) (bar - argv[1]);
        strncpy(file, argv[1] + position + 1, position);

        printf("path: %s\n", file);
    }

        /* Gets path */
    else
    {
        bar = strrchr(argv[1], '/');
        int position = (int) (bar - argv[1]);
        strncpy(file, argv[1] + position + 1, position);

        printf("path: %s\n", file);
    }

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

    if (hasuser != NULL)
    {
        /* Copys localy string to parse */
        char shiftedBuffer[strlen(argv[1]) - 5];
        strcpy(shiftedBuffer, ftpFreeBuffer);

        /* Find and get username */
        char * p = strchr(shiftedBuffer, ':');

        int position = (int) (p - shiftedBuffer);
        strncpy(username, shiftedBuffer, position);

        /* Shifts string to get password */
        strncpy(shiftedBuffer, shiftedBuffer + position + 1, strlen(shiftedBuffer) - position);

        /* Finds and gets password */
        p = strchr(shiftedBuffer, '@');
        position = (int) (p - shiftedBuffer);
        strncpy(password, shiftedBuffer, position);

        /* Shifts string to get host */
        strncpy(shiftedBuffer, shiftedBuffer + position + 1, strlen(shiftedBuffer) - position);

        /* Gets hostname */
        p = strchr(shiftedBuffer, '/');
        position = (int) (p - shiftedBuffer);
        strncpy(host, shiftedBuffer, position);

        /* Shifts string to get url */
        strncpy(shiftedBuffer, shiftedBuffer + position + 1, strlen(shiftedBuffer) - position);

        /* Gets url path */
        strcpy(url, shiftedBuffer);


        printf("host %s, username %s, pass %s, url %s\n", host, username, password, url);
    }
    else
    {
        /* Copys localy string to parse */
        char shiftedBuffer[strlen(argv[1]) - 5];
        strcpy(shiftedBuffer, ftpFreeBuffer);

        /* Username and Password defined */
        strcpy(username, "anonymous");
        strcpy(password, "foo");

        /* Gets hostname */
        char * p = strchr(shiftedBuffer, '/');
        int position = (int) (p - shiftedBuffer);
        strncpy(host, shiftedBuffer, position);

        /* Shifts string to get url */
        strncpy(shiftedBuffer, shiftedBuffer + position + 1, strlen(shiftedBuffer) - position);

        /* Gets url path */
        strcpy(url, shiftedBuffer);

        printf("host %s, username %s, pass %s, url %s\n", host, username, password, url);
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

    printf("username %spassword %s\n", user, pass);

    char * h_address = (char*) getip(host);

    int sockfd = connect_socket(h_address, 21);

    int rec = receive(sockfd);
    if (rec > 3)
    {
        perror("Access denied! Exiting...\n");
        exit(1);
    }

    /* Sending username */
    int bytes = write(sockfd, user, strlen(user));
    if (bytes <= 0)
    {
        perror("Error trying to write username! Exiting...\n");
        exit(1);
    }

    rec = receive(sockfd);
    if (rec == -1 || rec > 3)
    {
        perror("Access denied trying to read username response! Exiting...\n");
        exit(1);
    }

    /* Sending password */
    bytes = write(sockfd, pass, strlen(pass));
    if (bytes <= 0)
    {
        perror("Error trying to write password! Exiting...\n");
        exit(1);
    }

    rec = receive(sockfd);
    if (rec == -1 || rec > 6)
    {
        perror("Access denied trying to read password response! Exiting...\n");
        exit(1);
    }


    /* Sending pasv */
    bytes = write(sockfd, "pasv\r\n", 6);
    if (bytes <= 0)
    {
        perror("Error trying to write pasv! Exiting...\n");
        exit(1);
    }

    rec = receive_data(sockfd);
    if (rec == -1)
    {
        perror("Access denied to read pasv response! Exiting...\n");
        exit(1);
    }

    printf("Connecting to %d!\n", rec);

    /* Connecting to new port */
    int auxsockfd = connect_socket(h_address, rec);

    printf("Connected!\n");

    char getfile[URL_LENGTH];
    bzero(getfile, URL_LENGTH);
    strcpy(getfile, "RETR /");
    strcat(getfile, url);
    strcat(getfile, "\r\n");

    write(sockfd, getfile, strlen(getfile));

    /* Gets first response: file exists or not */buf
    rec = receive(sockfd);
    if (rec == -1 || rec > 3)
    {
        perror("Access denied trying to read if file exists or not! Exiting...\n");
        exit(1);
    }

    /* Response in socket opened with previous port with content of file */
    writefile(auxsockfd, file);

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);

    shutdown(auxsockfd, SHUT_RDWR);
    close(auxsockfd);

    return 0;
}

int writefile(int sockfd, char * file)
{
    int fd = open(file, O_CREAT | O_RDWR | O_EXCL | O_TRUNC, S_IRWXU);
    if (fd < 0)
    {
        perror("Error trying to open file!");
        exit(1);
    }

    char buf[512];


    int status;


    while ((status = read(sockfd, buf, 512)))
    {
    	printf("%s\n", buf);
    	buf[status] = '\0';
        write(fd, buf, strlen(buf));
        bzero(buf, strlen(buf));
    }

    close(fd);

    return 0;
}

int receive_data(int sockfd)
{
    char buf[MAXLENGTH];
    bzero(buf, MAXLENGTH);

    int status = read(sockfd, buf, MAXLENGTH);

    if (status > 0)
    {
        printf("Message received: %s\n", buf);

        /* Gets status */
        char tempBuf[strlen(buf)];
        bzero(tempBuf, strlen(buf));
        strcpy(tempBuf, buf);
        char statuscode[3];
        bzero(statuscode, 3);
        strncpy(statuscode, tempBuf, 3);
        statuscode[3] = '\0';

        int recstatus = atoi(statuscode);

        char firstnumber[3];
        char secondnumber[5];
        bzero(firstnumber, strlen(firstnumber));
        bzero(secondnumber, strlen(secondnumber));

        /* Checks if coded received is status good to continue*/
        if (recstatus == PORT_STATUS)
        {
            /* Gets second number */
            char * p = strrchr(tempBuf, ',');
            int position = (int) (p - tempBuf);
            strncpy(secondnumber, tempBuf + position + 1, strlen(tempBuf) - position - 4);

            /* Shifts string */
            char teste[strlen(tempBuf)];
            strncpy(teste, tempBuf, position);

            /* Gets the first number */
            p = strrchr(teste, ',');
            position = (int) (p - teste);
            strncpy(firstnumber, teste + position + 1, strlen(teste) - position - 1);

            int number1 = atoi(firstnumber);
            int number2 = atoi(secondnumber);

            printf("number1: %d, number2: %d\n", number1, number2);

            int port = number1 * 256 + number2;

            printf("port: %i\n", port);
            return port;
        }
    }
    return -1;
}

int receive(int sockfd)
{
    char buf[MAXLENGTH];
    bzero(buf, strlen(buf));
    int recstatus;
    sleep(1);

    int status = read(sockfd, buf, MAXLENGTH);
    buf[status] = '\0';
    if (status)
    {
        printf("Message received: %s\n", buf);

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

    if (regcomp(&preg, "ftp://(([A-Za-z0-9])*:([A-Za-z0-9])*@)*([A-Za-z0-9.~-])+/([[A-Za-z0-9/~._-])+", REG_EXTENDED) != 0)
    {
        perror("Could not compile regular expression");
        exit(1);
    }

    regexec(&preg, address, nmatch, pmatch, REG_EXTENDED);
    regfree(&preg);

    totalMatch = pmatch[0].rm_eo;
    matchesBegin = pmatch[0].rm_so;

    if (totalMatch != nmatch || matchesBegin != 0)
        return 1;

    return 0;
}
