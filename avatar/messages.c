/**
 * The function for handling messages bewteen client and server
 */

#include <stdio.h>
#include <stdio.h>
#include "../amazing.h"
#include <netinet/in.h>

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