#include "amazing.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>	  
#include <netdb.h>	
#include <string.h>


int main (int argc, char *argv[])
{
if (argc != 4) {
    fprintf(stderr, "usage: ./AMinitup nAvatars Difficulty Hostname\n");
    exit(1);
}
int nAvatars;
int Difficulty;
char * Hostname = argv[3];
char *program = argv[0];
if ((sscanf(argv[1], "%d", &nAvatars) != 1) || (sscanf(argv[2], "%d", &Difficulty)!= 1)) {
    fprintf(stderr, "Please input integer for parameters\n");
    exit(2);
}
printf("%d %d\n", nAvatars, Difficulty);

int comm_sock = socket(AF_INET, SOCK_STREAM, 0);
if (comm_sock < 0) {
    perror("opening socket");
    exit(3);
}
struct sockaddr_in server;  // address of the server
server.sin_family = AF_INET;
server.sin_port = htons(atoi(AM_SERVER_PORT));

struct hostent *hostp = gethostbyname(Hostname);
if (hostp == NULL) {
    fprintf(stderr, "%s: unknown host '%s'\n", program, Hostname);
    exit(3);
}  
memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

  // 3. Connect the socket to that server   
if (connect(comm_sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
    perror("connecting stream socket");
    exit(4);
  }
  printf("Connected!\n");
  
  AM_Message init;
  init.type = htons(AM_INIT);
  init.init.nAvatars = htons(nAvatars);
  init.init.Difficulty = htons(Difficulty);

  printf("Try to send AM_INIT message to the server now \n");
  if (write(comm_sock, &init, sizeof(AM_Message)) < 0) {
      fprintf(stderr, "error\n");
      exit(5);
  }
    printf("Sent\n");

//   AM_Message server_message;  
  int receive = read(comm_sock, &init, sizeof(AM_Message));
  
  if (receive < 0) {
      fprintf(stderr, "failed\n");
  } if (receive == 0) {
      fprintf(stderr, "Closed connection\n");
  }
  if (IS_AM_ERROR(init.type)) {
      printf("error message\n");
  } 
  printf("accurate\n");
  
  int width = ntohl(init.init_ok.MazeWidth);
  int height = ntohl(init.init_ok.MazeHeight);
  int port = ntohl(init.init_ok.MazePort);

  printf("%d %d %d", width, height, port);
  

  close(comm_sock);
}