#include "ftpClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <regex.h>

int main(int argc, char * argv[]) {
	if (argc <= 0 || argc > 2) {
		perror("ERROR: Invalid usage!");
		perror("Usage: client ftp://[<user>:<password>@]<host>/<url-path>!");
		exit(1);
	}

	if (parseAdd(argv[1])) {
		perror("ERROR: Invalid ftp string!");
		perror("Should be: ftp://[<user>:<password>@]<host>/<url-path>!");
		exit(1);
	}



	return 0;
}

int parseAdd(char * address) {
	regex_t preg;
	 size_t nmatch=strlen(address);
	 regmatch_t pmatch[nmatch];

	char * regex = "ftp://[A-Za-z0-9:A-Za-z0-9@]*[A-Za-z0-9]+";

	/* teste string= "ftp://\\(\\([a-zA-Z0-9\\.]\\+:[a-zA-Z0-9]*@\\)\\?\\|@\\?\\)\\([a-zA-Z0-9]\\+[\\.]\\)\\+[a-zA-Z0-9]\\+/\\([a-zA-Z0-9]\\+[/]\\?\\)*"
	*/

	if (regcomp(&preg, regex, 0) != 0) {
		perror("Could not compile regular expression");
		exit(1);
	}

	int status = regexec(&preg, address, nmatch, pmatch, 0);

	regfree(&preg);
	
	totalMatch=pmatch[0].rm_eo;
	matchesBegin=pmatch[0].rm_so==0;


	if(status != 0) {
		printf("1\n");
		return 1;
	}

	printf("0\n");
	return 0;
}

