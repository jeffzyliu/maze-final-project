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
#include "../output/logfile.h"

typedef struct avatar_parameter avatar_p; //our parameter for the avatar method

/**
 * SUMMARY: The main avatar function that each thread calls
 * 
 * PARAMETER:
 *      void *arg      all the parameters needed to start multithreading socket reading
 * 
 * RETURN:
 *      NONE
 */
void *avatar (void *arg);

// /**
//  * avatar_p *clientParameters()
//  * SUMMARY: This function creates the struct avatar_p, which is the parameter we need to
//  * pass to our avatar method when we create new threads
//  * 
//  * PARAMETERS:
//  *      int AvatariD        ID for each avatar starting at 0
//  *      int nAvatars        total number of avatars
//  *      int Difficulty      difficulty level
//  *      char *hostname      hostname
//  *      int mazeport        the port of maze location
//  *      char *filename      the file we are writing to
//  * 
//  * RETURN:
//  *      avatar_p *parameter the data struct with necessary parameters
//  */
avatar_p *clientParameters(int AvatarId, int nAvatars, int Difficulty, char *hostname, int mazeport, char *filename, AM_Message *finalMessage);

#endif //__AVATAR_H
