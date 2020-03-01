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
void errorMessage (AM_Message server_message)
{
    int errortype = ntohl(server_message.type);
    switch(errortype) {
        case AM_INIT_FAILED:
            fprintf(stderr, "AM_INIT is not processed by server\n");
            break;
        case AM_NO_SUCH_AVATAR:
            fprintf(stderr, "Received message from an Unknown AvatarId\n");
            break;
        case AM_UNKNOWN_MSG_TYPE:
            fprintf(stderr, "Can't understand message\n");
            break;
        case AM_UNEXPECTED_MSG_TYPE:
            fprintf(stderr, "Message type unexpected\n");
            break;
        case AM_TOO_MANY_MOVES:
            fprintf(stderr, "Moves exceeded maximum number\n");
            break;
        case AM_SERVER_TIMEOUT:
            fprintf(stderr, "Time elapsed between messages exceeded limit\n");
            break;
        case AM_SERVER_DISK_QUOTA:
            fprintf(stderr, "Server can't write or create to a file\n");
            break;
        case AM_SERVER_OUT_OF_MEM:
            fprintf(stderr, "Server cannot allocate enough memory to serve a maze\n");
            break;
    }
}

/**
 * Checking if the server message is valid
 */
void validMessageTurn (int turn, int comm_sock, AM_Message client, int AvatarId, int Direction, AM_Message server_avatar_turn)
{
    if (turn == 1) {
        memset(&client, 0, sizeof(AM_Message));
        client.type = htonl(AM_AVATAR_MOVE);
        client.avatar_move.AvatarId= htonl(AvatarId);
        client.avatar_move.Direction = htonl(Direction);
        if (write(comm_sock, &client, sizeof(AM_Message)) < 0 ) {
            fprintf(stderr, "failed to send to server\n");
        }
    }
}

AM_Message receiveMessage (int comm_sock, AM_Message server_avatar_turn)
{
    int receive = read(comm_sock, &server_avatar_turn, sizeof(AM_Message));
    //If it is less than or equal to 0, then the connection closed
    if (receive <= 0) {
        fprintf(stderr, "Connection to Server Closed\n");
        server_avatar_turn.type = AM_SOCKET_BREAK;
    }
    if (IS_AM_ERROR(ntohl(server_avatar_turn.type))) {
        errorMessage(server_avatar_turn);
    } 
    return server_avatar_turn;
}
