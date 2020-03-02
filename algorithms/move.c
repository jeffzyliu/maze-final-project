/**
 * move.c
 * 
 * author: Jeff Liu, Willem Klein Wassink, Celina tala
 * 
 * module about move information and algorithms
 * see move.h for documentation
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
static bool directionBlocked(maze_t *maze, XYPos currLoc, int proposedDirection);

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
        return newLoc.y > oldLoc.y ? M_SOUTH : M_NORTH;
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
 * uses a shared explored-nodes map to never pointlessly run into a wall again
 */ 
int decide_maprighthand(int lastHeading, XYPos oldLoc, XYPos newLoc, maze_t *maze)
{
    pthread_mutex_lock(&mutex1);
    int proposedHeading = decide_simplerighthand(lastHeading, oldLoc, newLoc);
    // first calculate a heading, but turn left until the direction not identified as a wall
    while (directionBlocked(maze, newLoc, proposedHeading)) {
        proposedHeading = turnLeft(proposedHeading);
    }
    pthread_mutex_unlock(&mutex1);
    return proposedHeading;
}

/**
 * updates the maze depending on what we learned from moving
 * also check if we just exited a dead end
 * 
 * sets both sides
 */ 
void maze_update(int lastHeading, XYPos oldLoc, XYPos newLoc, maze_t *maze, int avatarID)
{
    pthread_mutex_lock(&mutex1);
    int direction = avatar_moved(oldLoc, newLoc);
    if (oldLoc.y == 3 || newLoc.y == 3) printf("%d,%d\n", oldLoc.y, newLoc.y);
    if (direction != M_NULL_MOVE) { // moved in a direction, set new path in direction moved
        if (wall_count(maze, oldLoc.x, oldLoc.y) >= 3) { // exited a dead-end, mark as closed
            set_neighbor(maze, oldLoc.x, oldLoc.y, direction, oldLoc.x, oldLoc.y); // cannot go forward
            set_neighbor(maze, newLoc.x, newLoc.y, turnAround(direction), newLoc.x, newLoc.y); // cannot go back
        } else {
            set_neighbor(maze, oldLoc.x, oldLoc.y, direction, newLoc.x, newLoc.y); // mark as open
            set_neighbor(maze, newLoc.x, newLoc.y, turnAround(direction), oldLoc.x, oldLoc.y); // can go back
        }
    } else { // didn't move, set last heading to wall
        XYPos otherside = otherSide(lastHeading, oldLoc);
        set_neighbor(maze, oldLoc.x, oldLoc.y, lastHeading, oldLoc.x, oldLoc.y); // cannot go through
        
        set_neighbor(maze, otherside.x, otherside.y, turnAround(lastHeading), otherside.x, otherside.y); // cannot come through
    }
    set_avatar(maze, oldLoc.x, oldLoc.y, -1);
    set_avatar(maze, newLoc.x, newLoc.y, avatarID);
    pthread_mutex_unlock(&mutex1);
}

/**
 * converts a heading to its left one
 */ 
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

/**
 * converts a heading to its right one
 */ 
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

/**
 * converts a heading to its opposite one
 */ 
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

/**
 * gets a coordinate to the N/S/E/W of the current one
 */ 
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
            otherside.y = loc.y + 1;
            break;
        case M_EAST:
            otherside.x = loc.x + 1;
            otherside.y = loc.y;
            break;
        case M_NORTH:
            otherside.x = loc.x;
            otherside.y = loc.y - 1;
            break;
        default:
            return loc;
    }
    return otherside;
}

/**
 * checks inside the maze to see whether a path is blocked for sure
 */ 
static bool directionBlocked(maze_t *maze, XYPos currLoc, int proposedDirection)
{
    XYPos neighbor = check_neighbor(maze, currLoc.x, currLoc.y, proposedDirection);
    return currLoc.x == neighbor.x && currLoc.y == neighbor.y;
}


/********************************************************
 **************** unit testing **************************
 ********************************************************/

#ifdef ALG_TEST

/**
 * creates a new maze that looks like this:
 * +----+----+----+
 * |         | ## |
 * +    +    +    +
 * |    |         |
 * +    +    +----+
 * | 0  |         |
 * +----+----+----+
 */ 
maze_t *create_server_maze()
{
    printf("Creating maze of height 3 and width 3\n");
    maze_t *servermaze = maze_new(3, 3);
    
    printf("Setting avatar at 0,2.\n");
    set_avatar(servermaze, 0, 2, 0);

    printf("Putting walls and passages in the servermaze.\n");
    
    // (0,0)
    set_neighbor(servermaze, 0, 0, M_EAST, 1, 0);
    set_neighbor(servermaze, 0, 0, M_SOUTH, 0, 1);

    // (0,1)
    set_neighbor(servermaze, 0, 1, M_EAST, 0, 1); // east wall
    set_neighbor(servermaze, 0, 1, M_SOUTH, 0, 2);
    set_neighbor(servermaze, 0, 1, M_NORTH, 0, 0);

    // (0,2)
    set_neighbor(servermaze, 0, 2, M_NORTH, 0, 1);
    set_neighbor(servermaze, 0, 2, M_EAST, 0, 2); // east wall

    // (1,0)
    set_neighbor(servermaze, 1, 0, M_WEST, 0, 0);
    set_neighbor(servermaze, 1, 0, M_SOUTH, 1, 1);
    set_neighbor(servermaze, 1, 0, M_EAST, 1, 0); // East wall

    // (1,1)
    set_neighbor(servermaze, 1, 1, M_WEST, 1, 1); // west wall
    set_neighbor(servermaze, 1, 1, M_NORTH, 1, 0);
    set_neighbor(servermaze, 1, 1, M_SOUTH, 1, 2); 
    set_neighbor(servermaze, 1, 1, M_EAST, 2, 1);

    // (1,2)
    set_neighbor(servermaze, 1, 2, M_WEST, 1, 2); // west wall
    set_neighbor(servermaze, 1, 2, M_EAST, 2, 2);
    set_neighbor(servermaze, 1, 2, M_NORTH, 1, 1); 

    // (2,0)
    set_neighbor(servermaze, 2, 0, M_SOUTH, 2, 1);
    set_neighbor(servermaze, 2, 0, M_WEST, 2, 0); // West wall

    // (2,1)
    set_neighbor(servermaze, 2, 1, M_NORTH, 2, 0);
    set_neighbor(servermaze, 2, 1, M_SOUTH, 2, 1); // south wall
    set_neighbor(servermaze, 2, 1, M_WEST, 1, 1); 

    // (2,2)
    set_neighbor(servermaze, 2, 2, M_WEST, 1, 2);
    set_neighbor(servermaze, 2, 2, M_NORTH, 2, 2); // north wall

    unit_maze_print(servermaze, stdout);

    return servermaze;
}

/**
 * helper method to print out status reports without logfile.c
 */ 
char *direction_to_string(int direction)
{
    switch (direction)
    {
        case M_SOUTH:
            return "south";
        case M_NORTH:
            return "north";
        case M_EAST:
            return "east";
        case M_WEST:
            return "west";
        default:
            return "not a direction";
    }
}

/**
 * tests the basic RHF alg
 */ 
void test_rhf(maze_t *servermaze, XYPos target)
{
    printf("\ntesting simple right hand follow on the created maze");
    XYPos avatar;
    avatar.x = 0;
    avatar.y = 2;

    int lastHeading = M_SOUTH;
    XYPos oldLoc;
    oldLoc.x = avatar.x;
    oldLoc.y = avatar.y - 1;

    int turnCount;
    for (turnCount = 0; avatar_moved(target, avatar) != M_NULL_MOVE; turnCount++) {
        if (turnCount != 0) {
            avatar_moved(oldLoc, avatar) != 8 ? printf(" (success)") : printf(" (failed)");
        }
        lastHeading = decide_simplerighthand(lastHeading, oldLoc, avatar);
        oldLoc = avatar;
        printf("\nturn %d: avatar at (%d,%d) attempting to move %s", turnCount, avatar.x, avatar.y, direction_to_string(lastHeading));
        avatar = check_neighbor(servermaze, avatar.x, avatar.y, lastHeading);
    }
    printf(" (success)\ngame ended at (%d,%d) [expected 2,0] in %d turns [expected 22]\n", avatar.x, avatar.y, turnCount);
}

/**
 * tests the enhanced alg
 * does not test for mutex locking and starvation
 */ 
void test_maprhf(maze_t *servermaze, XYPos target)
{
    printf("\ntesting enhanced right hand follow on the created maze");
    XYPos avatar;
    avatar.x = 0;
    avatar.y = 2;

    int lastHeading = M_SOUTH;
    XYPos oldLoc;
    oldLoc.x = avatar.x;
    oldLoc.y = avatar.y - 1;
    maze_t *avatarmaze = maze_new(3, 3);
    set_avatar(avatarmaze, 0, 2, 0);

    int turnCount;
    for (turnCount = 0; turnCount < 20 && avatar_moved(target, avatar) != M_NULL_MOVE; turnCount++) {
        if (turnCount != 0) {
            avatar_moved(oldLoc, avatar) != 8 ? printf(" (success)") : printf(" (failed)");
            maze_update(lastHeading, oldLoc, avatar, avatarmaze, 0);
        }
        lastHeading = decide_maprighthand(lastHeading, oldLoc, avatar, avatarmaze);
        oldLoc = avatar;
        printf("\nturn %d: avatar at (%d,%d) attempting to move %s", turnCount, avatar.x, avatar.y, direction_to_string(lastHeading));
        avatar = check_neighbor(servermaze, avatar.x, avatar.y, lastHeading);
    }
    maze_update(lastHeading, oldLoc, avatar, avatarmaze, 0);
    printf(" (success)\ngame ended at (%d,%d) [expected 2,0] in %d turns [expected 13]\n", avatar.x, avatar.y, turnCount);
    printf("\ncheck resultant maze for correct markings and dead-end-blocking:\n");
    unit_maze_print(avatarmaze, stdout);
    maze_delete(avatarmaze);
}

/**
 * test all algorithms on a maze specified in create_server_maze
 */ 
int main() 
{
    maze_t *servermaze = create_server_maze();
    XYPos target;
    target.x = 2;
    target.y = 0;

    test_rhf(servermaze, target);

    test_maprhf(servermaze, target);

    maze_delete(servermaze);
    return 0;
}

#endif // ALG_TEST
