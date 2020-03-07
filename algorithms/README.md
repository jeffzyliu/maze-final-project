# CS50 Final Project -- Maze Challenge, Winter 2020
## Team Willpower--Jeff Liu, Willem Klein Wassink, Celina Tala

### algorithms

The *algorithms* library consists of a set of algorithms governing move decisions and information tracking. It currently implements a simple right-hand-follower and an enhanced follower that makes use of the *mazedata* library.

### Usage

*algorithms* compiles a library that is to be used by any avatars in need of deciding moves in the maze. It also draws upon the *mazedata* library to record prior learned information.

It exports a set of functions through *move.h*.

### Implementation

move.h functions:
```c
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
```

### Compilation

To compile, `make`. This program can also be compiled recursively by calling `make` on the project-maze-challenge-willpower directory, which will call make on its children directories.

### Testing

To test, uncomment out -DALG_TEST in the makefile, and call `make test`. Be sure to recompile `mazedata.a` properly by calling `make clean` and then `make` in the mazedata directory, making sure to comment out the unit test flag there.

Then run ./move to see a unit test of the algorithms.

### Assumptions

The algorithms assume that the structure of the problem is as stated in the instructions, and that any avatar calling its decide functions know for sure that they want to move. In other words, they only pick directions instead of deciding whether a null-move is necessary.
