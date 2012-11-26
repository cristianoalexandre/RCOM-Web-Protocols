all:	clientTCP.c	getip.c	ftpClient.c
	gcc -std=gnu99	-o	client	ftpClient.c	getip.c	clientTCP.c	-Wall	-lm
clean:
	rm client