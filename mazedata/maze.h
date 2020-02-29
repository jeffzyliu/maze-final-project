/*
 * Willem Klein Wassink, CS50, Winter 2020
 * 
 * maze.h - header file for the client-side procedurally created maze
 * 
 * A maze is a 2d array of mazenode structs. It starts with all nodes
 * being null and creates them as an avatar requests to move into a place.
 * 
 * A mazenode is an element of a maze representing one square. See maze.c for more information
 * 
 */

#include "../amazing.h"

#ifndef __MAZE_H
#define __MAZE_H

// ------------------- global types
typedef struct maze maze_t;

// ------------------- functions

// ------------------- maze_new
/* Initializes a new maze
 * 
 * Inputs:
 *      An integer width and height for the maze
 * 
 * Outputs:
 *      Pointer to new maze, or NULL if error.
 * 
 * Caller responsibilities:
 *      Deleting the maze later with maze_delete
 */
maze_t *maze_new(int height, int width);

// ------------------- set_wall
/* Sets a maze's neighbor to a passed-in node
 * 
 * Inputs:
 *      a maze pointer
 *      integer (x,y) coordinates
 *      an integer 'd' representing a direction
 *      the contents of the new neighbor
 * 
 * Outputs:
 *      NULL if failure
 *      otherwise, a pointer to the updated node
 */
mazenode_t *set_neighbor(maze_t *maze, int x, int y, const int d, mazenode_t *new_neighbor);

// ------------------- wall_count
/* Returns the number of walls a mazenode is neighbors with
 *
 * Inputs:
 *      a pointer to a maze
 *      integer (x,y) coordinates
 * 
 * Outputs:
 *      NULL if failure
 *      number of walls in a node on success
 */
int wall_count(maze_t *maze, int x, int y);

// ------------------- maze_delete
/* Deletes all the mazenodes in a maze and then the maze itself
 * 
 * Inputs:
 *      a pointer to a valid maze
 * 
 * Ignores NULL mazes    
 */
void maze_delete(maze_t *maze);

#endif // __MAZE_H