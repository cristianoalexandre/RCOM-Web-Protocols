all:	clientTCP.c	getip.c
	gcc	-std=c99	-o	clienttcp	clientTCP.c	-Wall	-lm
	gcc	-std=c99	-o	getip	getip.c	-Wall	-lm
clean:
	rm clienttcp
	rm getip