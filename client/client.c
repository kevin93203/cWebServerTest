
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
#define SERVER_PORT 80
const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void send_file(FILE *fp, int sockfd){
  printf("Sending Picture as Byte Array\n");
  char send_buffer[SIZE]; // no link between BUFSIZE and the file size
  int nb = fread(send_buffer, 1, sizeof(send_buffer), fp);
  while(!feof(fp)) {
    write(sockfd, send_buffer, nb);
    nb = fread(send_buffer, 1, sizeof(send_buffer), fp);
    // no need to bzero
  }
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;
  char filename[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  /*
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);*/
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
 printf("[+]Connected to Server.\n");

  printf("\nPlease enter file name to send:\n"); 
  scanf("%s", filename); 
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }
  
  send_file(fp, sockfd);
  printf("[+]File data sent successfully.\n");

  printf("[+]Closing the connection.\n");
  close(sockfd);

  return 0;
}
