/**
 * move.c
 * 
 * module about move information and algorithms
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "../amazing.h"
#include "../mazedata/maze.h"
#include <stdbool.h>
#include <pthread.h>
#include "move.h"

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

static int turnLeft(int heading);
static int turnRight(int heading);
static int turnAround(int heading);
static XYPos otherSide(int heading, XYPos loc);

/**
 * checks if an avatar moved
 * 
 * if so, returns move direction
 * else returns null move
 */ 
int avatar_moved(XYPos oldLoc, XYPos newLoc)
{
    if (oldLoc.x != newLoc.x) {
        return newLoc.x > oldLoc.x ? M_EAST : M_WEST;
    } else if (oldLoc.y != newLoc.y) {
        return newLoc.y > oldLoc.y ? M_NORTH : M_SOUTH;
    } else { // no movement
        return M_NULL_MOVE; // == 8
    }
}

/**
 * simplest right-hand-follow maze solve algorithm
 * assumes that the avatar does the moving and gives old and new positions
 */ 
int decide_simplerighthand(int lastHeading, XYPos oldLoc, XYPos newLoc)
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
int decide_maprighthand(int lastHeading, XYPos oldLoc, XYPos newLoc, maze_t *maze)
{
    pthread_mutex_lock(&mutex1);
    // do {

    // } while ();

    pthread_mutex_unlock(&mutex1);
    return 0;
}

/**
 * updates the maze depending on what we learned from moving; also check if we just exited a dead end
 */ 
void maze_update(int lastHeading, XYPos oldLoc, XYPos newLoc, maze_t *maze)
{
    pthread_mutex_lock(&mutex1);
    int direction = avatar_moved(oldLoc, newLoc);
    if (direction != M_NULL_MOVE) { // moved in a direction, set new path in direction moved
        if (wall_count(maze, oldLoc.x, oldLoc.y) >= 3) { // exited a dead-end, mark as closed
            set_neighbor(maze, oldLoc.x, oldLoc.y, direction, oldLoc.x, oldLoc.y); // cannot go forward
            set_neighbor(maze, newLoc.x, newLoc.y, turnAround(direction), newLoc.x, newLoc.y); // cannot go back
        } else {
            set_neighbor(maze, oldLoc.x, oldLoc.y, direction, newLoc.x, newLoc.y); // mark as open
            set_neighbor(maze, newLoc.x, newLoc.y, turnAround(direction), oldLoc.x, oldLoc.y); // can go back
        }
    } else { // didn't move, set last heading to wall
        set_neighbor(maze, oldLoc.x, oldLoc.y, lastHeading, oldLoc.x, oldLoc.y); // cannot go through
        XYPos otherside = otherSide(lastHeading, oldLoc);
        set_neighbor(maze, otherside.x, otherside.x, turnAround(lastHeading), otherside.x, otherside.y); // cannot come through
    }
    pthread_mutex_unlock(&mutex1);
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

static int turnAround(int heading)
{
    switch (heading) {
        case M_WEST:
            return M_EAST;
        case M_SOUTH:
            return M_NORTH;
        case M_EAST:
            return M_WEST;
        case M_NORTH:
            return M_SOUTH;
        default:
            return M_NULL_MOVE;
    }
}

static XYPos otherSide(int heading, XYPos loc)
{
    XYPos otherside;
    switch (heading) {
        case M_WEST:
            otherside.x = loc.x - 1;
            otherside.y = loc.y;
            break;
        case M_SOUTH:
            otherside.x = loc.x;
            otherside.y = loc.y - 1;
            break;
        case M_EAST:
            otherside.x = loc.x + 1;
            otherside.y = loc.y;
            break;
        case M_NORTH:
            otherside.x = loc.x;
            otherside.y = loc.y + 1;
            break;
        default:
            return loc;
    }
    return otherside;
}