/**
 * This is the main file called when a new thread for each avatar is called
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>	  
#include <netdb.h>	
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "messages.h"
#include "../amazing.h"
#include "../output/logfile.h"
#include "../output/ui.h"
#include "../algorithms/move.h"
#include "../mazedata/maze.h"

//our data struct for passing a paramter to our avatar thread for each avatar
typedef struct avatar_paramter {
    int AvatarId;
    int nAvatars;
    int Difficulty;
    char *hostname;
    int mazeport;
    char *filename;
    maze_t *maze;
} avatar_p;

// -------------------------- local function prototypes
static bool isGameOver (AM_Message server_avatar_turn);
bool status = true;
bool last = true;

/******************* Local Functions
 * The main avatar function that each thread calls
 */
void *avatar (void *arg)
{
    int exitCode;
    avatar_p *parameter = (avatar_p *)arg;
    int mazeport = parameter->mazeport;
    char *hostname = parameter->hostname;
    int AvatarId = parameter->AvatarId;
    char *filename = parameter->filename;
    int nAvatars = parameter->nAvatars;
    maze_t *maze = parameter->maze;
    
    int comm_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (comm_sock < 0) {
        perror("opening socket");
        exitCode = 4;
        pthread_exit(&exitCode);
    }
    struct sockaddr_in server;  // address of the server
    server.sin_family = AF_INET;
    server.sin_port = htons(mazeport);

    struct hostent *hostp = gethostbyname(hostname);
    if (hostp == NULL) {
        fprintf(stderr, "unknown host '%s'\n", hostname);
        exitCode = 5;
        pthread_exit(&exitCode);
    }  
    memcpy(&server.sin_addr, hostp->h_addr_list[0], hostp->h_length);

    // 3. Connect the socket to the server   
    if (connect(comm_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connecting stream socket");
        exitCode = 6;
        pthread_exit(&exitCode);
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
            close(comm_sock);
            exitCode = 7;
            status = false;
            pthread_exit(&exitCode);
        }
        int receive = read(comm_sock, &server_avatar_turn, sizeof(AM_Message));
        if (receive <= 0) {
            fprintf(stderr, "Connection to Server Closed\n");
            exitCode = 8;
            close(comm_sock);
            status = false;
            pthread_exit(&exitCode);
        } 
        if (IS_AM_ERROR(ntohl(server_avatar_turn.type))) {
            errorMessage(filename, server_avatar_turn);
        } 
    } while (IS_AM_ERROR(ntohl(server_avatar_turn.type)) & status);

    //getting the server response 
    int TurnId = ntohl(server_avatar_turn.avatar_turn.TurnId);
    XYPos *pos = server_avatar_turn.avatar_turn.Pos;
    int startingX = ntohl(pos[AvatarId].x);
    int startingY = ntohl(pos[AvatarId].y);  

    //inserting all of the starting avatars into our maze
    if (AvatarId == nAvatars-1) {
        for (int i = 0; i < nAvatars; i++) {
            set_avatar(maze, ntohl(pos[i].x),ntohl(pos[i].y), i, true);
        }
    }
    //writing the starting state to our log file
    startingState(filename, AvatarId, startingX, startingY, pos, maze);

    AM_Message avatarTurn;
    int lastHeading = M_NORTH;
    avatarTurn.type = AM_UNKNOWN_MSG_TYPE;
    int Direction;
    XYPos sentinel;    //the position of the avatar that doesn't move
    sentinel.x = ntohl(pos[0].x);
    sentinel.y = ntohl(pos[0].y);
    XYPos oldLoc;
    oldLoc.x = startingX;
    oldLoc.y = startingY+1;
    XYPos newLoc;
    newLoc.x = startingX;
    newLoc.y = startingY;
    int turnCount = 0;
    //temporary direction to go west. If the server responds with a valid message
    while (status) {
        if (isGameOver(server_avatar_turn)) {
            break;
        }
        if (ntohl(server_avatar_turn.type) != AM_AVATAR_TURN) {
            errorMessage(filename, server_avatar_turn);
            server_avatar_turn = receiveMessage(comm_sock, server_avatar_turn);
            continue;   
        }
        TurnId = ntohl(server_avatar_turn.avatar_turn.TurnId);
        pos = server_avatar_turn.avatar_turn.Pos;
        if (TurnId%nAvatars == AvatarId) {
            newLoc.x = ntohl(pos[AvatarId].x);
            newLoc.y = ntohl(pos[AvatarId].y);
            if (AvatarId == 0 || avatar_moved(newLoc, sentinel) == M_NULL_MOVE) {
                Direction = M_NULL_MOVE;
            } else {
                if (turnCount != 0) {
                    maze_update(lastHeading, oldLoc, newLoc, maze, AvatarId);
                }
                avatarTurned (false, filename, AvatarId, nAvatars, newLoc, oldLoc, pos, Direction, maze);
                Direction = decide_maprighthand(lastHeading, oldLoc, newLoc, maze);
                oldLoc = newLoc;
                lastHeading = Direction;
                turnCount++;
            }
            exitCode = validMessageTurn(comm_sock, avatarTurn, AvatarId, Direction, server_avatar_turn);
            if (exitCode == 7) {
                close(comm_sock);
                status = false;
                free(parameter);
                pthread_exit(&exitCode);
            }
        } 
        server_avatar_turn = receiveMessage(comm_sock, server_avatar_turn);
    }
    if (ntohl(server_avatar_turn.type) == AM_MAZE_SOLVED) {
        maze_update(lastHeading, oldLoc, sentinel, maze, AvatarId);
        if (ntohl(server_avatar_turn.maze_solved.nMoves)%nAvatars==AvatarId) {
            avatarTurned (true, filename, AvatarId, nAvatars, sentinel, oldLoc, pos, Direction, maze);
            exitGame(filename, server_avatar_turn, maze);
        }
        exitCode = 0; 
    }  
    if (ntohl(server_avatar_turn.type) != AM_MAZE_SOLVED) {
        if (AvatarId == TurnId) {
            exitGame(filename, server_avatar_turn, maze);
        }
        exitCode = 13;
    }
    free(parameter);
    close(comm_sock);
    pthread_exit(&exitCode);
    return NULL;
}

/**
 * Creating the parameter we need for our avatar method
 */
avatar_p *clientParameters(int AvatarId, int nAvatars, int Difficulty, char *hostname, int mazeport, char *filename, maze_t *maze)
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
    parameter->maze = maze;
    return parameter;
}

/**
 * Function that checks if the game is over
 */
static bool isGameOver (AM_Message server_avatar_turn)
{
    return server_avatar_turn.type == ntohl(AM_MAZE_SOLVED) || server_avatar_turn.type == ntohl(AM_SOCKET_BREAK) || server_avatar_turn.type == ntohl(AM_TOO_MANY_MOVES) || server_avatar_turn.type == ntohl(AM_SERVER_TIMEOUT);
}
