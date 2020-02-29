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
    XYPos sentinel;    //the position of the avatar that doesn't move
    sentinel.x = ntohl(pos[0].x);
    sentinel.y = ntohl(pos[0].y);
    //temporary direction to go west. If the server responds with a valid message
    while (1) {
        if (server_avatar_turn.type == ntohl(AM_MAZE_SOLVED)) {
            break;
        }
        if (AvatarId == 0 || avatar_moved(pos[AvatarId], sentinel) == 8) {
            server_avatar_turn = validMessageTurn(1, comm_sock, avatarTurn, AvatarId, M_NULL_MOVE, server_avatar_turn);
            continue;
        }
        if (TurnId%nAvatars == AvatarId) {
            XYPos oldLoc;
            oldLoc.x = oldx;
            oldLoc.y = oldy;
            server_avatar_turn = validMessageTurn(1, comm_sock, avatarTurn, AvatarId, Direction, server_avatar_turn);
            XYPos newLoc;
            newLoc.x = ntohl(server_avatar_turn.avatar_turn.Pos[AvatarId].x);
            newLoc.y = ntohl(server_avatar_turn.avatar_turn.Pos[AvatarId].y);
            avatarTurned (filename, AvatarId, nAvatars, newLoc, oldLoc, pos);
            Direction = decide_simplerighthand(Direction, oldLoc, newLoc);
            oldx = newLoc.x;
            oldy = newLoc.y;
        } else {
            server_avatar_turn = validMessageTurn(0, comm_sock, avatarTurn, AvatarId, Direction, server_avatar_turn);
        }
        TurnId = ntohl(server_avatar_turn.avatar_turn.TurnId);
    }
    mazeSolved(filename, ntohl(server_avatar_turn.maze_solved.nAvatars), ntohl(server_avatar_turn.maze_solved.Difficulty),ntohl(server_avatar_turn.maze_solved.nMoves), ntohl(server_avatar_turn.maze_solved.Hash));
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

