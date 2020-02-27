/**
 * decision.c
 * 
 * module to decide what direction an avatar should head into next
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "../amazing.h"

static int turnLeft(int heading);
static int turnRight(int heading);

/**
 * simplest right-hand-follow maze solve algorithm
 * assumes that the avatar does the moving and gives old and new positions
 */ 
int decide_simplerighthand(int lastHeading, XYPos *oldLoc, XYPos *newLoc)
{
    // move success
    if (oldLoc->x != newLoc->x || oldLoc->y != newLoc->y) {
        return turnRight(lastHeading);
    }
    // should be redundant code; keeping in for the note
    // if (oldLoc->x != newLoc->x) {
    //     int direction = newLoc->x > oldLoc->x ? M_EAST : M_WEST;
    //     return turnRight(direction);
    // } else if (oldLoc->y != newLoc->y) {
    //     int direction = newLoc->y > oldLoc->y ? M_NORTH : M_SOUTH;
    //     return turnRight(direction);
    // }

    // move failed
    return turnLeft(lastHeading);
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