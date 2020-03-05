/**
 * ui.c
 * 
 * collection of function(s) to print ASCII UI to the command line
 * see ui.h for documentation
 * 
 * AUTHOR: Celina Tala, Jeff Liu, Willem Klein Wassink
 */ 

#include "../amazing.h"
#include "../mazedata/maze.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "../mazedata/maze.c"

void print_ui(maze_t *maze, char *status)
{
    printf("****************************************\n");

    // print status message; do we want to have this in logfile.c/h?
    // do we want to have logfile.c/h return strings, and mutex lock each avatar instead
    // of each function that changes things? we can centralize mutex locks that way
    // and have auxilliary functions only manipulate data

    // waiting on maze_t to finish this

    printf("%s\n", status);
    printf("    ");

    // Initial column numbers
    for (int x=0; x < maze->width; x++) {
        printf(" %d  ", x);
    }
    printf("\n   +");

    // Upper border
    for (int x=0; x < maze->width; x++) {
        printf("---+");
    }
    printf("\n");

    // Each row
    for (int y=0; y < maze->height; y++) {
        printf("%-3d|", y); // Row number
        for (int x=0; x < maze->width; x++) {
            // Print node contents
            if (get_avatar(maze, x, y) != -1) {
                printf(" %d ", get_avatar(maze, x, y));
            } else {
                printf("   ");
            }
            
            // Print column dividers
            if (is_wall(maze, x, y, M_EAST) == 0) { // If there's a wall to the east
                printf("|");
            } else {
                printf(" ");
            }

            if (x == maze->width-1) {
                printf("\n");
            }
        }

        // Print row dividers
        printf("   +");
        for (int x=0; x < maze->width; x++) {
            if (y == maze->height-1 || is_wall(maze, x, y, M_SOUTH) == 0) { // If there's a wall to the
                printf("---+");
            } else {
                printf("   +");
            }
        }
        printf("\n");
        
    }
    
    // print "     " (5 spaces)
    // for each column
        // print " N  " where N  is row number
    // print "\n+"
    // for each column
        // print "---+"
    // print "\n"
    // for each row
        // print "%3d|" where the number is row number, increasing from 0
        // for each column
            // print " N " where N is the avatar number (or space if no avatar)
            // if this is the last column
                // print "|\n"
            // else
                // print " "
        // print "   +"
        // for each column
            // print "   +"
        // print "\n"
    unit_maze_print(maze, stdout);
    printf("****************************************\n\n");
}


#ifdef UNIT_TEST

int test_uimaze1() {
    printf("Creating maze of height 3 and width 3\n");
    maze_t *maze = maze_new(3, 3, 3);
    printf("----------------------------------------------------------------\n");

    printf("Printing maze UI\n");
    print_ui(maze);
    printf("----------------------------------------------------------------\n");

    
    printf("Calling unit_maze_print method\n");
    unit_maze_print(maze, stdout);
    printf("----------------------------------------------------------------\n");

    printf("Assessing nodes' wallcounts. Should depend on presence of border walls\n");
    for (int y=0; y < 3; y++) {
        for (int x=0; x < 3; x++) {
            printf("Wallcount for (%d,%d): %d\n", x, y, wall_count(maze, x, y));
        }
    }
    printf("----------------------------------------------------------------\n");
    
    printf("Putting some avatars in the maze: 0 at (0,0), 1 at (2,1) and 2 at (2,2)\n");
    set_avatar(maze, 0, 0, 0, true);
    set_avatar(maze, 2, 1, 1, true);
    set_avatar(maze, 2, 2, 2, true);
    printf("----------------------------------------------------------------\n");

    printf("Printing maze UI w/ avatars\n");
    print_ui(maze);
    printf("----------------------------------------------------------------\n");

    // 0 for West, 1 for North, 2 for South, 3 for East.
    printf("Putting walls and passages the maze. Shouldn't be any nulls\n");
    
    // (0,0)
    set_neighbor(maze, 0, 0, M_EAST, 1, 0);
    set_neighbor(maze, 0, 0, M_SOUTH, 0, 0); // south wall

    // (0,1)
    set_neighbor(maze, 0, 1, M_EAST, 1, 1);
    set_neighbor(maze, 0, 1, M_SOUTH, 0, 2);
    set_neighbor(maze, 0, 1, M_NORTH, 0, 1); // NORTH wall

    // (0,2)
    set_neighbor(maze, 0, 2, M_NORTH, 0, 1);
    set_neighbor(maze, 0, 2, M_EAST, 1, 2);

    // (1,0)
    set_neighbor(maze, 1, 0, M_WEST, 0, 0);
    set_neighbor(maze, 1, 0, M_SOUTH, 1, 1);
    set_neighbor(maze, 1, 0, M_EAST, 1, 0); // East wall


    // (1,1)
    set_neighbor(maze, 1, 1, M_WEST, 0, 1);
    set_neighbor(maze, 1, 1, M_NORTH, 1, 0);
    set_neighbor(maze, 1, 1, M_SOUTH, 1, 1); // SOUTH wall
    set_neighbor(maze, 1, 1, M_EAST, 1, 1); // East wall

    // (1,2)
    set_neighbor(maze, 1, 2, M_WEST, 0, 2);
    set_neighbor(maze, 1, 2, M_EAST, 2, 2);
    set_neighbor(maze, 1, 2, M_NORTH, 1, 2); // NORTH wall

    // (2,0)
    set_neighbor(maze, 2, 0, M_SOUTH, 2, 1);
    set_neighbor(maze, 2, 0, M_WEST, 2, 0); // West wall

    // (2,1)
    set_neighbor(maze, 2, 1, M_NORTH, 2, 0);
    set_neighbor(maze, 2, 1, M_SOUTH, 2, 2);
    set_neighbor(maze, 2, 1, M_WEST, 2, 1); // West wall

    // (2,2)
    set_neighbor(maze, 2, 2, M_WEST, 1, 2);
    set_neighbor(maze, 2, 2, M_NORTH, 2, 1);
    
    printf("----------------------------------------------------------------\n");

    printf("Assessing nodes' new wallcounts. Should represent new maze\n");
    for (int y=0; y < 3; y++) {
        for (int x=0; x < 3; x++) {
            printf("Wallcount for (%d,%d): %d\n", x, y, wall_count(maze, x, y));
        }
    }
    printf("----------------------------------------------------------------\n");

    printf("Calling unit_maze_print method\n");
    unit_maze_print(maze, stdout);
    printf("----------------------------------------------------------------\n");

    printf("Testing check_neighbor function\n");
    XYPos neighbor = check_neighbor(maze, 0, 0, M_NORTH);
    printf("north of (0,0) is (%d,%d)\n", neighbor.x, neighbor.y);
    neighbor = check_neighbor(maze, 0, 0, M_SOUTH);
    printf("south of (0,0) is (%d,%d)\n", neighbor.x, neighbor.y);
    neighbor = check_neighbor(maze, 0, 0, M_EAST);
    printf("east of (0,0) is (%d,%d)\n", neighbor.x, neighbor.y);
    printf("----------------------------------------------------------------\n");
    
    printf("Printing maze UI w/ walls\n");
    print_ui(maze);
    printf("----------------------------------------------------------------\n");

    printf("Deleting maze\n");
    maze_delete(maze);
    printf("----------------------------------------------------------------\n");
    return 0;
}

int main() {
    test_uimaze1();
    exit(0);
}

#endif // UNIT_TEST
