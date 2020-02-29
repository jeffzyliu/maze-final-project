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

// ------------------- set_wall
/* Sets a maze's neighbor to a passed-in node
 * 
 * Inputs:
 *      a maze pointer
 *      integer (x,y) coordinates for the current node
 *      an integer 'd' representing a direction, 0 for West, 1 for North, 2 for South, 3 for East.
 *      integer (x,y) coordinates for the new neighbor
 * 
 * Outputs:
 *      False if failure (bad input for d)
 *      True on success
 */
bool set_neighbor(maze_t *maze, int x, int y, const int d, int neighbor_x, int neighbor_y);

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

// ------------------- wall_count
/* Returns the number of walls a mazenode is neighbors with
 *
 * Inputs:
 *      a pointer to a maze
 *      integer (x,y) coordinates
 * 
 * Outputs:
 *      -1 on failure
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

// ------------------- unit_mazenode_print
/* Prints out the node at (x,y) in the maze
 * 
 * Inputs:
 *      a pointer to a valid maze
 *      integer x and y coordinates
 *      a pointer to a file open for writing
 * 
 * Guarantees
 *      Ignores NULL mazes
 *      Ignores NULL file pointers
 *      Ignores coordinates below zero and above width/height
 */
void unit_mazenode_print(maze_t *maze, int x, int y, FILE *fp);

// ------------------- unit_maze_print
/* Iterates over the entire maze, calling unit_mazenode_print on each location
 * 
 * Inputs:
 *      a pointer to a valid maze
 *      a pointer to a file open for writing
 */
void unit_maze_print(maze_t *maze, FILE *fp);

// ------------------- set_avatar
/* Puts an avatar of a given ID into a location in a maze
 * 
 * Inputs:
 *      a pointer to a valid maze
 *      integer (x,y) coordinates
 *      an integer avatar ID
 */
bool set_avatar(maze_t *maze, int x, int y, int avatar_id);

#endif // __MAZE_H