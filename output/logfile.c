/**
 * logfile.h
 * 
 * collection of function(s) to print updates to the log file from avatars
 * see logfile.c for documentation
 * 
 * AUTHOR: Celina Tala, Jeff Liu, Willem Klein Wassink
 */ 

#include "../amazing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logfile.h"
#include <pthread.h> 
#include <unistd.h> 
#include <netinet/in.h>


pthread_mutex_t lock; 
/**
 * The first few lines of the file that logs where each Avatar is inserted
 */
void startingState (char *filename, int AvatarId, int x, int y, XYPos *pos)
{
    pthread_mutex_lock(&lock);
    FILE *fp;
    fp = fopen(filename, "a");
    if (fp == NULL) {
        fprintf(stderr, "failed to open file\n");
        return;
    }
    
    fprintf(fp, "Inserted Avatar %d at (%d, %d)\n", AvatarId, x, y);
    fprintf(fp, "Avatar Locations: ");
    for (int i = 0; i <= AvatarId; i++) {
        fprintf(fp, "%d: (%d, %d); ", i, ntohl(pos[i].x), ntohl(pos[i].y));
    }
    fprintf(fp, "\n");
    fclose(fp);
    pthread_mutex_unlock(&lock); 
}