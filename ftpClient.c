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
        if (shiftedBuffer[i] == '@')
        {
            atPosition = 1;
            foundit = 1;
            continue;
        }

        if (shiftedBuffer[i] == ':')
        {
            colonPosition = 1;
            continue;
        }

        if (atPosition == -1 && colonPosition == 1)
        {
            password[l] = shiftedBuffer[i];
            l++;
        }

        if (atPosition == -1 && colonPosition == -1)
        {
            username[k] = shiftedBuffer[i];
            k++;
        }

        if (foundit && shiftedBuffer[i] != '/')
        {
            buffer[j] = shiftedBuffer[i];
            j++;
        }

        if (shiftedBuffer[i] == '/')
        {
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
        strcpy(password, "\n");
        
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (i >= HOST_LENGTH)
                puts("Shit! Something's wrong!");

            if (argv[1][i] == '/' && argv[1][i + 1] == '/')
            {
                foundit = 1;
                continue;
            }

            if (foundit && argv[1][i] != '/')
            {
                buffer[j] = argv[1][i];
                j++;
            }

            if (argv[1][i] == '/' && argv[1][i + 1] != '/' && argv[1][i - 1] != '/')
            {
                foundit = 0;
                break;
            }
        }
    }

    printf("username %s, password %s\n", username, password);

    char * h_address = (char*) getip(buffer);

    int status = sendMessage(h_address, "");

    return 0;
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
