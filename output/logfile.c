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


pthread_mutex_t lock; 
void writetoFile (char *filename, int AvatarId, int x, int y, XYPos pos[])
{
    pthread_mutex_lock(&lock);
    fprintf("%s\n", filename);
    FILE *fp;
    fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "failed to open file\n");
        return;
    }
    fprintf(fp, "Inserted Avatar %d at (%d, %d)\n", AvatarId, x, y);
    int i = 0;
    while (pos[i].x!= NULL) {
        fprintf(fp, "%d: (%d, %d); ", AvatarId, x, y);
        i++;
    }
    fprintf(fp, "\n");
    fclose(fp);
    pthread_mutex_unlock(&lock); 
}