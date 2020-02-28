/**
 * decision.h
 * 
 * module to decide what direction an avatar should head into next
 */ 

#ifndef __DECISION_H
#define __DECISION_H

#include <stdio.h>
#include <stdlib.h>
#include "../amazing.h"

/**
 * simplest right-hand-follow maze solve algorithm
 * assumes that the avatar does the moving and gives old and new positions
 * 
 * this would not handle null-moves; it should only be called when it wants to move for sure
 * 
 * requires:
 *   lastHeading of the avatar that calls it
 *   x,y tuple oldLocation
 *   x,y tuple newLocation
 * 
 * returns:
 *   a new heading for the avatar to then send as a move request message to the server
 */
int decide_simplerighthand(int lastHeading, XYPos *oldLoc, XYPos *newLoc);

#endif // __DECISION_H