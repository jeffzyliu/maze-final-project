/**
 * This is the main file called when a new thread for each avatar is called
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>	  
#include <netdb.h>	
#include <string.h>
#include <pthread.h>
#include "messages.h"
#include "../amazing.h"
#include "../output/logfile.h"

//our data struct for passing a paramter to our avatar thread for each avatar
typedef struct avatar_paramter {
    int AvatarId;
    int nAvatars;
    int Difficulty;
    char *hostname;
    int mazeport;
    char *filename;
} avatar_p;

/**
 * The main avatar function that each thread calls
 */
void *avatar (void *arg)
{
    avatar_p *parameter = (avatar_p *)arg;
    int mazeport = parameter->mazeport;
    char *hostname = parameter->hostname;
    int AvatarId = parameter->AvatarId;
    char *filename = parameter->filename;
    printf("The file name is %s\n", filename);
    int comm_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (comm_sock < 0) {
        perror("opening socket");
        exit(3);
    }
    struct sockaddr_in server;  // address of the server
    server.sin_family = AF_INET;
    server.sin_port = htons(mazeport);

    struct hostent *hostp = gethostbyname(hostname);
    if (hostp == NULL) {
        fprintf(stderr, "unknown host '%s'\n", hostname);
        exit(3);
    }  
    memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

    // 3. Connect the socket to the server   
    if (connect(comm_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connecting stream socket");
        exit(4);
    }
    //keep trying to send the server a ready message until it is accepted
    AM_Message ready;
    AM_Message server_avatar_turn;
    do {
        memset(&ready, 0, sizeof(AM_Message));
        ready.type = htonl(AM_AVATAR_READY);
        ready.avatar_ready.AvatarId= htonl(AvatarId);
        if (write(comm_sock, &ready, sizeof(AM_Message)) < 0 ) {
            fprintf(stderr, "failed to send to servor\n");
            exit(5);
        }
        int receive = read(comm_sock, &server_avatar_turn, sizeof(AM_Message));
        if (receive < 0) {
            fprintf(stderr, "Failed to Receive Message from Server\n");
            exit(6);
        } 
        //If it is equal to 0, then the connection closed
        if (receive == 0) {
            fprintf(stderr, "Connection to Server Closed\n");
            exit(7);
        }
        if (IS_AM_ERROR(ntohl(server_avatar_turn.type))) {
            errorMessage(server_avatar_turn);
        } 
    } while (IS_AM_ERROR(ntohl(server_avatar_turn.type)));

    int TurnId = ntohl(server_avatar_turn.avatar_turn.TurnId);
    int x = ntohl(server_avatar_turn.avatar_turn.Pos[AvatarId].x);
    int y = ntohl(server_avatar_turn.avatar_turn.Pos[AvatarId].y);
    printf("The turnID for avatar %d is %d\n", AvatarId, TurnId);
    printf("The position for ID %d is at (%d, %d)\n", AvatarId, x, y );
    writetoFile(filename, AvatarId, x, y, ntohl(server_avatar_turn.avatar_turn.Pos));

    free(parameter);
    return NULL;
}

/**
 * Creating the parameter we need for our avatar method
 */
avatar_p *clientParameters(int AvatarId, int nAvatars, int Difficulty, char *hostname, int mazeport, char *filename)
{
    avatar_p *parameter = malloc(sizeof(avatar_p));
    if (parameter == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    parameter->AvatarId= AvatarId;
    parameter->nAvatars = nAvatars;
    parameter->Difficulty = Difficulty;
    parameter->hostname = hostname;
    parameter->mazeport = mazeport;
    parameter->filename = filename;
    return parameter;
}
