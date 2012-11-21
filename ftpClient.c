#include "ftpClient.h"
#include "getip.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>

int main(int argc, char * argv[]) {
	if (argc <= 1 || argc > 2) {
		perror("ERROR: Invalid usage!");
		perror("Usage: client ftp://[<user>:<password>@]<host>/<url-path>!");
		exit(1);
	}

	if (parseAdd(argv[1])) {
		perror("ERROR: Invalid ftp string!");
		perror("Should be: ftp://[<user>:<password>@]<host>/<url-path>!");
		exit(1);
	}

	printf("String compiled sucessfully");

	char buffer[HOSTLENGTH];
	int foundit = 0;
	int j = 0;

	for (int i = 0; i < strlen(argv[1]); i++) {
		if (argv[1][i] == '@') {
			foundit = 1;
		}

		if (foundit && argv[1][i] != '/') {
			buffer[j] = argv[1][i];
			j++;
		}
	}

	if (!foundit) {
		for (int i = 0; ; i++) {
			if (argv[1][i] != '/') break;

			buffer[j] = argv[1][i];
			j++;
		}
	}



	return 0;
}

int parseAdd(char * address) {
	regex_t preg;
	size_t nmatch=strlen(address);
	regmatch_t pmatch[nmatch];
	int totalMatch;
	int matchesBegin;

	if (regcomp(&preg, "ftp://(([A-Za-z0-9])*:([A-Za-z0-9])*)*@([A-Za-z0-9])+/([[A-Za-z0-9/])+", REG_EXTENDED) != 0) {
		perror("Could not compile regular expression");
		exit(1);
	}

	regexec(&preg, address, nmatch, pmatch, REG_EXTENDED);

	regfree(&preg);

	totalMatch=pmatch[0].rm_eo;
	matchesBegin=pmatch[0].rm_so;

	if(totalMatch != nmatch || matchesBegin != 0) {
		return 1;
	}

	return 0;
}
