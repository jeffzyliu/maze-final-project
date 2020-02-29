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

void writetoFile (char *filename, int AvatarId, int x, int y, XYPos pos[]);

#endif // __LOGWRITE_H
