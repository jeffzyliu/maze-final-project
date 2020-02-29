/*
 * Willem Klein Wassink, Jeff Liu, Celina Tala, CS50, Winter 2020
 * 
 * maze.c - module for encoding a maze
 * 
 * see maze.h for more information.
 */

#include <stdio.h>
#include <stdbool.h>
#include "../amazing.h"
#include "maze.h"

// ------------------- global types
typedef struct mazenode {
    // Array of 4 mazenodes. Index 0 is west, 1 is north, 2 is south, 3 is east
        // (unexplored = null)
        // (open space = pointer to next node)
        // (wall = pointer to self)
    mazenode_t *neighbors[4];

    // For checking if the node contains an avatar.
    // -1 for no avatar. Otherwise, it's the avatar's ID.
    int avatar;

    // (x,y) coordnates
    int x;
    int y;
} mazenode_t;


typedef struct maze {
    int height;
    int width;
    mazenode_t ***array;    // 2d array of mazenodes. Index like this: array[y][x]
} maze_t;


// ------------------- local functions prototypes
static mazenode_t *mazenode_new(int x, int y);


// ------------------- local functions
// Initalize a new mazenode & return it. NULL on failure. Caller must delete
// Neighbors init to NULL, caller gives (x,y), avatar init to -1
static mazenode_t *mazenode_new(int x, int y) {
    mazenode_t *node = malloc(sizeof(mazenode_t));
    if (node == NULL) {
        return NULL;
    } else {
        node->neighbors[M_WEST] = NULL;
        node->neighbors[M_NORTH] = NULL;
        node->neighbors[M_SOUTH] = NULL;
        node->neighbors[M_EAST] = NULL;
        node->x = x;
        node->y = y;
        node->avatar = -1;
        return node;
    }
}


// Frees contents of a mazenode
static void mazenode_delete(mazenode_t *node) {
    node->neighbors[M_WEST] = NULL;
    node->neighbors[M_NORTH] = NULL;
    node->neighbors[M_SOUTH] = NULL;
    node->neighbors[M_EAST] = NULL;
    free(node->neighbors);
    free(node);
}


// ------------------- global functions
// Creates a new maze of set width and height, filled with mazenodes. Caller has to delete it.
maze_t *maze_new(int height, int width) {
    maze_t *maze = malloc(sizeof(maze_t));
    if (maze == NULL) {
        fprintf(stderr, "maze allocation failed");
        return NULL;
    } else {
        maze->height = height;
        maze->width = width;
        mazenode_t ***array = calloc(height, sizeof(mazenode_t **));
        maze->array = array;
        for (int y = 0; y < height; y++) {
            mazenode_t **row = calloc(width, sizeof(mazenode_t *));
            maze->array[y] = row;
            for (int x = 0; x < width; x++) {
                row[x] = mazenode_new(x, y);
            }
        }
        return maze;
    }
}


// Caller passes in the maze, (x,y) coordinates for a location in it, and a pointer to the new neighbor.
// unexplored = null, open space = pointer to next node, wall = pointer to self
// 0 for West, 1 for North, 2 for South, 3 for East.
// Returns pointer to the updated node if success, NULL if failure
mazenode_t *set_neighbor(maze_t *maze, int x, int y, const int d, mazenode_t *new_neighbor) {
    if (d < 0 || d > 3) {
        fprintf(stderr, "Error: 'd' must be 0 (West), 1 (North), 2 (South), or 3 (East)");
        return NULL;
    }
    mazenode_t *node = maze->array[y][x];
    node->neighbors[d] = new_neighbor;
    return node;
}


// Caller passes in maze and (x,y) coords for a node to be examined
// Returns number of neighbors in the node that are walls.
int wall_count(maze_t *maze, int x, int y) {
    if (maze == NULL) {
        fprintf(stderr, "Error: NULL maze passed in");
        return NULL;
    }
    mazenode_t *node = maze->array[x][y];
    int wall_count = 0;
    for (int i = 0; i < 4; i++) {
        if (node->neighbors[i] == node) {
            wall_count++;
        }
    }
    return wall_count;
}


// Frees all the mazenodes in a maze
void maze_delete(maze_t *maze) {
    if (maze != NULL) {
        for (int y = 0; y < maze->width; y++) {
            for (int x = 0; x < maze->height; x++) {
                if (maze->array[y][x] != NULL) {
                    mazenode_delete(maze->array[y][x]);
                }
            }
            free(maze->array[y]);
        }
        free(maze->array);
        free(maze);
    }
}