/**
 * The function for handling messages bewteen client and server
 */

#include <stdio.h>
#include <stdlib.h>
#include "../amazing.h"
#include <netinet/in.h>
#include <stdbool.h>
#include <unistd.h>	  
#include <netdb.h>	
#include <string.h>
#include <pthread.h> 

pthread_mutex_t lock; 
/**
 * Printing the different error messages
 */
void errorMessage (char *filename, AM_Message server_message)
{
    FILE *fp = NULL;
    int errortype = ntohl(server_message.type);
    switch(errortype) {
        case AM_INIT_FAILED:
            if (ntohl(server_message.init_failed.ErrNum) == AM_INIT_TOO_MANY_AVATARS) {
                fprintf(stderr, "Avatar out of range\n");
            } else if (ntohl(server_message.init_failed.ErrNum)== AM_INIT_BAD_DIFFICULTY) {
                fprintf(stderr, "Difficulty out of range\n");
            }
            return;
        fp = fopen(filename, "a");
        if (fp == NULL ){
            fprintf(fp, "failed to open file\n");
        }
        case AM_NO_SUCH_AVATAR:
            fprintf(fp, "Received message from an Unknown AvatarId\n");
            break;
        case AM_UNKNOWN_MSG_TYPE:
            fprintf(fp, "Can't understand message\n");
            break;
        case AM_UNEXPECTED_MSG_TYPE:
            fprintf(fp, "Message type unexpected\n");
            break;
        case AM_TOO_MANY_MOVES:
            fprintf(fp, "Moves exceeded maximum number\n");
            break;
        case AM_SERVER_TIMEOUT:
            fprintf(fp, "Time elapsed between messages exceeded limit\n");
            break;
        case AM_SERVER_DISK_QUOTA:
            fprintf(fp, "Server can't write or create to a file\n");
            break;
        case AM_SERVER_OUT_OF_MEM:
            fprintf(fp, "Server cannot allocate enough memory to serve a maze\n");
            break;
    }
    fclose(fp);
}

/**
 * Checking if the server message is valid
 */
int validMessageTurn (int comm_sock, AM_Message client, int AvatarId, int Direction, AM_Message server_avatar_turn)
{
    memset(&client, 0, sizeof(AM_Message));
    client.type = htonl(AM_AVATAR_MOVE);
    client.avatar_move.AvatarId= htonl(AvatarId);
    client.avatar_move.Direction = htonl(Direction);
    if (write(comm_sock, &client, sizeof(AM_Message)) < 0 ) {
        fprintf(stderr, "failed to send to server\n");
        return 7;
    }
    return 0;
}

AM_Message receiveMessage (char *filename, int comm_sock, AM_Message server_avatar_turn, int AvatarId, int TurnId)
{
    int receive = read(comm_sock, &server_avatar_turn, sizeof(AM_Message));
    //If it is less than or equal to 0, then the connection closed
    if (receive <= 0) {
        fprintf(stderr, "Connection to Server Closed\n");
        server_avatar_turn.type = AM_SOCKET_BREAK;
    }
    return server_avatar_turn;
}
