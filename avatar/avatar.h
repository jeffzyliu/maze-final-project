/**
 * Willem Klein Wassink, Jeff Liu, and Celina Tala, CS50 Winter 2020
 * 
 * Avatar.h - header file for the individual avatars
 * 
 * Each avatar corresponds to one client in our multithreaded environment. It 
 * receives and sends messages to the server until an error occurs or the maze is solved
 */

#ifndef __AVATAR_H
#define __AVATAR_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>	  
#include <netdb.h>	
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "messages.h"
#include "../amazing.h"
#include "../algorithms/move.h"
#include "../output/ui.h"
#include "../output/logfile.h"
#include "../mazedata/maze.h"

//our parameter for the avatar method
typedef struct avatar_parameter avatar_p; 

/**************  avatar
 * The main avatar function that each thread calls
 * 
 * Inputs:
 *      a parameter struct which includes all variables needed to start multithreading socket reading
 * 
 * Outputs:
 *      The exit code if relevant
 *      else none
 */
void *avatar (void *arg);

/************ clientParameters()
 * This function creates the struct avatar_p, which is the parameter we need to
 * pass to our avatar method when we create new threads
 * 
 * Inputs:
 *      int AvatariD        ID for each avatar starting at 0
 *      int nAvatars        total number of avatars
 *      int Difficulty      difficulty level
 *      char *hostname      hostname
 *      int mazeport        the port of maze location
 *      char *filename      the file we are writing to
 * 
 * Outputs:
 *      the data struct avatar_p with necessary parameters
 */
avatar_p *clientParameters(int AvatarId, int nAvatars, int Difficulty, char *hostname, int mazeport, char *filename, maze_t *maze);

#endif //__AVATAR_H
