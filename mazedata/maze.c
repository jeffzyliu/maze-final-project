/*
 * Willem Klein Wassink, CS50, Winter 2020
 * 
 * maze.c - module for encoding a maze
 * 
 * see maze.h for more information.
 */

#include <stdio.h>
#include <stdbool.h>

// ------------------- local types
typedef struct mazenode {
    // (unexplored = null)
    // (open space = pointer to next node)
    // (wall = pointer to "wall node") TODO: How should this be identified?
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
    mazenode_t ***array;
} maze_t;

// ------------------- local functions prototypes
static mazenode_t *mazenode_new(int x, int y);

// ------------------- local functions
// Initalize a new mazenode & return it. NULL on failure. Caller must delete
static mazenode_t *mazenode_new(int x, int y) {
    mazenode_t *node = malloc(sizeof(mazenode_t));

    if (node == NULL) {
        return NULL;
    } else {
        node->x = x;
        node->y = y;
        node->north = NULL;
        node->south = NULL;
        node->east = NULL;
        node->west = NULL;
        return node;
    }
}

// ------------------- global functions
// Creates a new maze of set width and height, filled with mazenodes. Caller has to delete it.
maze_t *maze_new(int height, int width) {
    maze_t *maze = malloc(sizeof(maze_t));
    if (maze == NULL) {
        return NULL;
    } else {
        maze->height = height;
        maze->width = width;
        mazenode_t ***array = calloc(height, sizeof(mazenode_t[width]));
        maze->array = array;
        for (int y=0; y<height; y++) {
            mazenode_t **row = calloc(width, sizeof(mazenode_t));
            maze->array[y] = row;
            for (int x=0; x<width; x++) {
                row[x] = mazenode_new(x, y);
            }
        }
        return maze;
    }
}

bool set_wall(maze_t maze, int x, int y, const int d) {
    mazenode_t node = maze->array[x][y];
    switch (direction) {
        case M_NORTH: // Get these from amazing.h
        case M_SOUTH: 
        case M_EAST:
        case M_WEST:
    }
}