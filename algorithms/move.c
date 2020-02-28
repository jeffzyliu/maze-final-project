/**
 * move.c
 * 
 * module about move information and algorithms
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "../amazing.h"
#include "move.h"

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
int decide_maprighthand(int lastHeading, XYPos *oldLoc, XYPos *newLoc/*, (maze_t *maze*/)
{
    return 0;
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
    }
}