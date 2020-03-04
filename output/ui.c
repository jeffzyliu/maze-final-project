/**
 * ui.c
 * 
 * collection of function(s) to print ASCII UI to the command line
 * see ui.h for documentation
 * 
 * AUTHOR: Celina Tala, Jeff Liu, Willem Klein Wassink
 */ 

#include "../amazing.h"
#include "../mazedata/maze.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"

void print_ui(maze_t *maze)
{
    printf("****************************************\n");

    // print status message; do we want to have this in logfile.c/h?
    // do we want to have logfile.c/h return strings, and mutex lock each avatar instead
    // of each function that changes things? we can centralize mutex locks that way
    // and have auxilliary functions only manipulate data

    // waiting on maze_t to finish this

    // TODO: When you get to printing the avatar ID, comment that out. Jeff's might change implementation

    printf("Status message\n");
    printf("     ");

    // Initial column numbers
    for (int x=0; x < maze->width; x++) {
        printf(" %d  ", x);
    }
    printf("\n+");

    // Upper border
    for (int x=0; x < maze->width; x++) {
        printf("---+");
    }
    printf("\n");

    // Each row
    for (int y=0; y < maze->height; y++) {
        printf("%3d|", y); // Row number
        for (int x=0; x < maze->width; x++) {
            // Print node contents
            if (maze->array[y][x] == -1) {
                printf(" %d ", maze->array[y][x]->avatar); // TODO: Change this to match Jeff's boolean list implementation
            } else {
                printf("   ");
            }

            // Print column dividers
            if (x == maze->width-1) {
                printf("|\n");
            } else {
                printf(" ");
            }
        }

        // Print row dividers
        for (int x=0; x < maze->width; x++) {
            printf("   +");
        }
        printf("\n");
    }

    // print "     " (5 spaces)
    // for each column
        // print " N  " where N  is row number
    // print "\n+"
    // for each column
        // print "---+"
    // print "\n"
    // for each row
        // print "%3d|" where the number is row number, increasing from 0
        // for each column
            // print " N " where N is the avatar number (or space if no avatar)
            // if this is the last column
                // print "|\n"
            // else
                // print " "
        // print "   +"
        // for each column
            // print "   +"
        // print "\n"
    

    printf("****************************************\n\n");
}