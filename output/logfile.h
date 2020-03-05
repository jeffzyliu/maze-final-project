/**
 * logfile.h
 * 
 * Jeff Liu, Willem Klein Wassink, Celina tala
 * 
 * collection of function(s) to print updates to the log file from avatars
 * 
 */ 

#ifndef __LOGWRITE_H
#define __LOGWRITE_H

#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include "../amazing.h"
#include <netinet/in.h>
#include <stdbool.h>
#include "ui.h"
#include "../mazedata/maze.h"

/**
 * this method will write the initial starting positions of all the avatars
 * 
 * Input:
 *      char *filename      the file we are writing to
 *      int AvatarId        the Avatar that is writing to the file
 *      int x               x position of the Avatar   
 *      int y               y position of Avatar
 *      XYPos *pos          array of positions of other avatars
 */
void startingState (char *filename, int AvatarId, int x, int y, XYPos *pos, maze_t *maze);

/**
 * This prints the update when each avatar tries to move
 * 
 * Inputs:
 *      bool last           boolean if it is the last move
 *      char *filename      the file we are writing to
 *      int AvatarId        the Avatar that is writing to the file
 *      int nAvatars        total number of Avatars
 *      XYPos newPos        the new position of the avatar
 *      XYPos oldPos        the old position of the avatar
 *      XYPos *pos          array of positions of other avatars
 */     
void avatarTurned (bool last, char *filename, int AvatarId, int nAvatars, XYPos newPos, XYPos oldPos, XYPos *pos, int d, maze_t *maze);

/**
 * This prints a message whenever the game exists
 * 
 * Inputs:
 *      char *filename          the file we are writing to
 *      AM_Message finalmessage the finalMessage sent by the server
 */

void exitGame (char *filename, AM_Message finalMessage);

#endif // __LOGWRITE_H
