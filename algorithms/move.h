/**
 * move.h 
 * author: Jeff Liu, Willem Klein Wassink, Celina tala
 * 
 * 
 * module about move information and algorithms
 */ 

#ifndef __MOVE_H
#define __MOVE_H

#include <stdio.h>
#include <stdlib.h>
#include "../amazing.h"
#include "../mazedata/maze.h"
#include <stdbool.h>
#include <pthread.h>

/**
 * checks if an avatar moved and returns its direction
 * 
 * compares locations and returns the right direction as an int
 * returns null-move if the move failed
 */ 
int avatar_moved(XYPos oldLoc, XYPos newLoc);

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
int decide_simplerighthand(int lastHeading, XYPos oldLoc, XYPos newLoc);

/**
 * uses the maze map to expedite right hand following by not trying duplicate sides
 * 
 * calls decide_simplerighthand first
 * after, repeatedly turns left if necessary until the direction heads into an unknown path or a known open path
 */ 
int decide_maprighthand(int lastHeading, XYPos oldLoc, XYPos newLoc, maze_t *maze);

/**
 * updates the maze depending on last move
 * 
 * if we didn't move, set the location we were just in to have a wall in the direction we tried to move in
 * if we did move, find the direction we just moved in
 *   if we left a dead end (all 3 walls blocked), block it off so nobody enters it again
 * 
 * also blocks off the counterpart of the wall since each wall connects with one on the other side
 */ 
void maze_update(int lastHeading, XYPos oldLoc, XYPos newLoc, maze_t *maze, int avatarID);

#endif // __MOVE_H
