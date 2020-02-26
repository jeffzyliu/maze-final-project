/*
 * maze.h - header file for the client-side procedurally created maze
 * 
 * A maze is a 2d array of mazenode structs. It starts with all nodes
 * being null and creates them as an avatar requests to move into a place.
 * 
 * A mazenode is an element of a maze representing one square.
 * 
 */

#ifndef __MAZE_H
#define __MAZE_H

// ------------------- global types
typedef struct maze maze_t;