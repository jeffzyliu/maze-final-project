/**
 * logfile.h
 * 
 * collection of function(s) to print updates to the log file from avatars
 * see logfile.c for documentation
 * 
 * AUTHOR: Celina Tala, Jeff Liu, Willem Klein Wassink
 */ 

#include "../amazing.h"
#include "ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logfile.h"
#include <pthread.h> 
#include <unistd.h> 
#include <netinet/in.h>
#include <stdbool.h>
#include "../mazedata/maze.h"


pthread_mutex_t lock; 
/**
 * The first few lines of the file that logs where each Avatar is inserted
 */
void startingState (char *filename, int AvatarId, int x, int y, XYPos *pos, maze_t *maze)
{
    pthread_mutex_lock(&lock);
    FILE *fp;
    fp = fopen(filename, "a");
    if (fp == NULL) {
        fprintf(stderr, "failed to open file\n");
        return;
    }
    char startposition[100];
    sprintf(startposition, "Inserted %d at %d,%d\n", AvatarId, x, y);
    fprintf(fp, "%s", startposition);
    if (AvatarId == 0) {
        fprintf(fp, "Avatar 0 will not move and all other avatars will move here\n");
    }
    fprintf(fp, "Avatar Locations: ");
    for (int i = 0; i <= AvatarId; i++) {
        fprintf(fp, "%d: (%d, %d); ", i, ntohl(pos[i].x), ntohl(pos[i].y));
    }
    fprintf(fp, "\n");
    fclose(fp);
    print_ui(maze, startposition);
    pthread_mutex_unlock(&lock); 
}

/**
 * the method that calls if the avatar is able to successfully move
 */
void avatarTurned (bool last, char *filename, int AvatarId, int nAvatars, XYPos newPos, XYPos oldPos, XYPos *pos, int d, maze_t *maze)
{
    FILE *fp;
    fp = fopen(filename, "a");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }
    char movedAvatar[100];
    char avatarStay[100];
    char *Direction;
    if (newPos.x == oldPos.x && newPos.y == oldPos.y) {
        if (d == 0) {
            Direction = "West";
        } else if (d == 1) {
            Direction = "North";
        } else if (d == 2) {
            Direction = "South";
        } else {
            Direction = "East";
        }
        sprintf(avatarStay, "Avatar %d hit %s wall\n", AvatarId, Direction);
        fprintf(fp, "%s", avatarStay);
        print_ui(maze, avatarStay);
    } else {
        sprintf(movedAvatar, "Avatar %d moved from %d, %d to %d, %d\n", AvatarId, oldPos.x, oldPos.y, newPos.x, newPos.y);
        fprintf(fp, "%s", movedAvatar);
        print_ui(maze, movedAvatar);
    }
    fprintf(fp, "Avatar Locations: ");
    if (last) {
        for (int i = 0; i < nAvatars; i++) {
            fprintf(fp, "%d: (%d, %d); ", i, newPos.x, newPos.y);
        }
    } else {
        for (int i = 0; i < nAvatars; i++) {
            fprintf(fp, "%d: (%d, %d); ", i, ntohl(pos[i].x), ntohl(pos[i].y));
        }
    }  
    fprintf(fp, "\n");
    fclose(fp);
}


/**
 * Writing to our file when we solved the maze
 */
void exitGame (char *filename, AM_Message finalMessage)
{
    int nAvatars = ntohl(finalMessage.maze_solved.nAvatars);
    int Difficulty = ntohl(finalMessage.maze_solved.Difficulty);
    int nMoves = ntohl(finalMessage.maze_solved.nMoves);
    int Hash = ntohl(finalMessage.maze_solved.Hash);
    FILE *fp;
    fp = fopen(filename, "a");
    if (fp == NULL) {
        fprintf(stderr, "failed to open file\n");
        return;
    }
    char solvedMessage[100];
    sprintf(solvedMessage, "Mazed solved with %d avatars at Difficult %d in %d moves at hash %u\n", nAvatars, Difficulty, nMoves, Hash);
    fprintf(fp, "%s", solvedMessage);
    fclose(fp);
}
