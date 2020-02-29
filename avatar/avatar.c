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
#include "../algorithms/move.h"

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
    int nAvatars = parameter->nAvatars;
    printf("%d\n", mazeport);
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
            fprintf(stderr, "failed to send to server\n");
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

    //getting the server response 
    int TurnId = ntohl(server_avatar_turn.avatar_turn.TurnId);
    XYPos *pos = server_avatar_turn.avatar_turn.Pos;
    int oldx = ntohl(pos[AvatarId].x);
    int oldy = ntohl(pos[AvatarId].y);   

    //writing the starting state to our log file
    startingState(filename, AvatarId, oldx, oldy, pos);

    AM_Message avatarTurn;
    int Direction = 1;
    int turns = 0;
    //temporary direction to go west. If the server responds with a valid message
    while (1) {
        if (server_avatar_turn.type == ntohl(AM_MAZE_SOLVED)) {
            printf("Solved Maze\n");
            break;
        }
        if (turns == 5) {
            break;
        }
        printf("The AvatarId is %d and the turnId is %d\n", AvatarId, TurnId);
        if (TurnId%nAvatars == AvatarId) {
            XYPos oldLoc;
            oldLoc.x = oldx;
            oldLoc.y = oldy;
            printf("The old location is (%d, %d)\n", x, y);
            server_avatar_turn = validMessage(comm_sock, avatarTurn, AvatarId, Direction, server_avatar_turn);
            XYPos newLoc;
            newLoc.x = ntohl(server_avatar_turn.avatar_turn.Pos[AvatarId].x);
            newLoc.y = ntohl(server_avatar_turn.avatar_turn.Pos[AvatarId].y);
            printf("The new location is (%d, %d)\n", newLoc.x, newLoc.y);
            avatarTurned (filename, AvatarId, nAvatars, newLoc, oldLoc, pos);
            Direction = decide_simplerighthand(Direction, oldLoc, newLoc);
            printf("The new direction is %d\n", Direction);
            TurnId = ntohl(server_avatar_turn.avatar_turn.TurnId);
            printf("The new TurnID is %d\n", TurnId);
            oldx = ntohl(server_avatar_turn.avatar_turn.Pos[AvatarId].x);
            oldy = ntohl(server_avatar_turn.avatar_turn.Pos[AvatarId].y);
        }
        turns++;
        
    }
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

