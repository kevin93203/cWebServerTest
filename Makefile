all : websiteServer.c
	gcc -Wall -g -o websiteServer websiteServer.c

clean :
	rm websiteServer
