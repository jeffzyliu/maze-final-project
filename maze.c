/*
 * Willem Klein Wassink, CS50, Winter 2020
 * 
 * maze.c - module for encoding a maze
 * 
 * see maze.h for more information.
 */

#include <stdio.h>

// ------------------- local types
typedef struct mazenode {
    // (unexplored = null)
    // (open space = pointer to next node)
    // (wall = pointer to "wall node")
    mazenode_t *north;
    mazenode_t *south;
    mazenode_t *east;
    mazenode_t *west;

    // (x,y) coordnates
    int x;
    int y;
} mazenode_t;

// ------------------- global types
typedef struct maze {
    int height;
    int width;
    mazenode_t *array[][];
} maze_t;

// ------------------- local functions prototypes
static mazenode_t *mazenode_new(int x, int y);

// ------------------- global functions


// ------------------- local functions
static mazenode_t *mazenode_new(int x, int y) {
    mazenode_t *node = malloc(sizeof(mazenode_t));

    if (node == NULL) {
        
    }
}