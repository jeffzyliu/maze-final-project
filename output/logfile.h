/**
 * logfile.h
 * 
 * collection of function(s) to print updates to the log file from avatars
 * 
 * AUTHOR: Celina Tala, Jeff Liu, Willem Klein Wassink
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

/**
 * SUMMARY: this method will write the initial starting positions of all the avatars
 * 
 * PARAMETERS:
 *      char *filename      the file we are writing to
 *      int AvatarId        the Avatar that is writing to the file
 *      int x               x position of the Avatar   
 *      int y               y position of Avatar
 *      XYPos *pos          array of positions of other avatars
 */
void startingState (char *filename, int AvatarId, int x, int y, XYPos *pos);

#endif // __LOGWRITE_H