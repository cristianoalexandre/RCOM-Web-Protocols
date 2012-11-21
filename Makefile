all:	clientTCP.c	getip.c	ftpClient.c getip.c
	gcc	-std=c99	-o	clienttcp	clientTCP.c	getip.c	-Wall	-lm
	gcc -std=c99	-o	client	ftpClient.c	-Wall	-lm
clean:
	rm clienttcp
	rm getip
	rm client