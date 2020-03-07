/**
 * ui.h
 * 
 * Function to print ASCII UI to the command line
 * 
 * AUTHOR: Celina Tala, Jeff Liu, Willem Klein Wassink
 */ 


#include "../amazing.h"
#include "../mazedata/maze.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ------------------- print_ui
/* Prints out the maze UI to stdout
 * 
 * Inputs:
 *      A pointer to a valid maze
 * 
 * Outputs:
 *      A printed maze in stdout
 */ 
void print_ui(maze_t *maze, char *status);
