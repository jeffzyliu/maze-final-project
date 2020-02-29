/**
 * move.c
 * 
 * module about move information and algorithms
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "../amazing.h"
#include "move.h"
#include "../mazedata/maze.h"
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

static int turnLeft(int heading);
static int turnRight(int heading);

/**
 * checks if an avatar moved
 * 
 * if so, returns move direction
 * else returns null move
 */ 
int avatar_moved(XYPos *oldLoc, XYPos *newLoc)
{
    if (oldLoc->x != newLoc->x) {
        return newLoc->x > oldLoc->x ? M_EAST : M_WEST;
    } else if (oldLoc->y != newLoc->y) {
        return newLoc->y > oldLoc->y ? M_NORTH : M_SOUTH;
    } else { // no movement
        return M_NULL_MOVE; // == 8
    }
}

/**
 * simplest right-hand-follow maze solve algorithm
 * assumes that the avatar does the moving and gives old and new positions
 */ 
int decide_simplerighthand(int lastHeading, XYPos *oldLoc, XYPos *newLoc)
{
    if (avatar_moved(oldLoc, newLoc) != M_NULL_MOVE) {
        return turnRight(lastHeading); // move success
    } else {
        return turnLeft(lastHeading); // move failed
    }    
}

/**
 * todo: program this algorithm which uses the map to optimize a bit
 */ 
int decide_maprighthand(int lastHeading, XYPos *oldLoc, XYPos *newLoc, maze_t *maze)
{
    pthread_mutex_lock(&mutex1);
    pthread_mutex_unlock(&mutex1);
}

/**
 * updates the maze depending on what we learned from moving; also check if we just exited a dead end
 */ 
void maze_update(int lastHeading, XYPos *oldLoc, XYPos *newLoc, maze_t *maze)
{
    pthread_mutex_lock(&mutex2);
    int direction = avatar_moved(oldLoc, newLoc);
    if (direction != M_NULL_MOVE) { // moved in a direction, set new path in direction moved
        if (wall_count(maze, oldLoc->x, oldLoc->y) >= 3) { // exited a dead-end, mark as closed
            set_neighbor(maze, oldLoc->x, oldLoc->y, direction, oldLoc->x, oldLoc->y);
        } else {
            set_neighbor(maze, oldLoc->x, oldLoc->y, direction, newLoc->x, newLoc->y); // mark as open
        }
    } else { // didn't move, set last heading to wall
        set_neighbor(maze, oldLoc->x, oldLoc->y, lastHeading, oldLoc->x, oldLoc->y);
    }
    pthread_mutex_unlock(&mutex2);
}

static int turnLeft(int heading)
{
    switch (heading) {
    case M_WEST:
        return M_SOUTH;
    case M_SOUTH:
        return M_EAST;
    case M_EAST:
        return M_NORTH;
    case M_NORTH:
        return M_WEST;
    default:
        return M_NULL_MOVE;
    }
}

static int turnRight(int heading)
{
    switch (heading) {
    case M_WEST:
        return M_NORTH;
    case M_SOUTH:
        return M_WEST;
    case M_EAST:
        return M_SOUTH;
    case M_NORTH:
        return M_EAST;
    default:
        return M_NULL_MOVE;
    }
}
