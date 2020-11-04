#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <sys/sendfile.h>
#define SERVER_PORT 8000
char webpage[]=
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>Rainbow Six: Siege</title>\r\n"
"<meta charset=\"utf-8\">\r\n"
"<style>body { background-color: #000000 }</style></head>\r\n"

    "<script>\r\n"	
	"function readURL(input){\r\n"
  		"if(input.files && input.files[0]){\r\n"
		"var imageTagID = input.getAttribute(\"targetID\");\r\n"
		"var reader = new FileReader();\r\n"
		"reader.onload = function (e) {\r\n"
			"var img = document.getElementById(imageTagID);\r\n"
			"img.setAttribute(\"src\", e.target.result)\r\n"
		"}\r\n"
		"reader.readAsDataURL(input.files[0]);\r\n"
		"}\r\n"
	"}\r\n"	
    "</script>\r\n"
    
"<body><font face=\"Verdana\" color=\"blue\">\r\n"
      "<h4 align=\"center\" style=\"color: white\">圖片預覽</h4>\r\n"
   "</font>\r\n"
   "<font face=\"Verdana\" size=\"2\">\r\n"
      "<center>\r\n"
        "<table>\r\n"
          "<tr>\r\n"
            "<td align=\"center\">\r\n"
              "<form action=\"/somewhere/to/upload\" enctype=\"multipart/form-data\">\r\n"
	      	"<input type=\"file\" onchange=\"readURL(this)\" targetID=\"preview_progressbarTW_img\" accept=\"image/gif, image/jpeg, image/png\"/ >\r\n"
	      	"<img id=\"preview_progressbarTW_img\" src=\"#\" />\r\n"
              "</form>\r\n"
            "</td>\r\n"
          "</tr>\r\n"
        "</table>\r\n"
      "</center>\r\n"
    "</font>\r\n"
"<center><img src=\"rainbow_six.jpg\"></center></body></html>\r\n";

char imageheader[] = 
"HTTP/1.1 200 Ok\r\n"
"Content-Type: image/jpeg\r\n\r\n";

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client;
	char buf[2048];
	int fdimg;
	int on =1;
	
	struct stat st;
	
	fd_server = socket(AF_INET, SOCK_STREAM, 0 );
	if(fd_server < 0){
		perror("socket");
		exit(1);
	}
	
	setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_PORT);
	
	if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind");
		close(fd_server);
		exit(1);
	}
	
	if(listen(fd_server, 10) == -1)
	{
		perror("listen");
		close(fd_server);
		exit(1);
	}
	printf("waiting for a connection on port %d\n",SERVER_PORT);
	while(1){
		fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);
		if(fd_client == -1)
		{
			perror("Connetion Failed......\n");
			continue;
		}
		
		printf("Got client connection......\n");
		
		if(!fork())
		{
			/* child process */
			close(fd_server);
			memset(buf, 0, 2048);
			read(fd_client, buf, 2047);
			
			printf("%s\n", buf);
			
			if(!strncmp(buf, "GET /favicon.ico", 16))
			{
				fdimg = open("favicon.ico", O_RDONLY);
				sendfile(fd_client, fdimg, NULL, 4000);
				close(fdimg);
			}
			if(!strncmp(buf, "GET /rainbow_six.jpg", 16))
			{
				stat("rainbow_six.jpg", &st);
				
				printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    				// Send header first: (Note, should loop in case of partial write or EINTR)
    				write(fd_client, imageheader, sizeof(imageheader) - 1);
    				// Send image content
    				fdimg = open("rainbow_six.jpg", O_RDONLY);
    				printf("%ld\n",st.st_size);
    				int sent = sendfile(fd_client, fdimg, NULL, st.st_size);
    				printf("sent: %d\n", sent);
    				close(fdimg);

			}
			if(!strncmp(buf, "GET /1cIVHK2A.jpg", 16))
			{
				stat("1cIVHK2A.jpg", &st);
				
				printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    				// Send header first: (Note, should loop in case of partial write or EINTR)
    				write(fd_client, imageheader, sizeof(imageheader) - 1);
    				// Send image content
    				fdimg = open("1cIVHK2A.jpg", O_RDONLY);
    				printf("%ld\n",st.st_size);
    				int sent = sendfile(fd_client, fdimg, NULL, st.st_size);
    				printf("sent: %d\n", sent);
    				close(fdimg);

			}
			else
				write(fd_client, webpage , sizeof(webpage) -1);
			close(fd_client);
			printf("closing...\n");
			exit(0);
		}
		/* parent process */
		close(fd_client);
	}
	return 0;
}
