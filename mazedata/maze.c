/*
 * Willem Klein Wassink, CS50, Winter 2020
 * 
 * maze.c - module for encoding a maze
 * 
 * see maze.h for more information.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../amazing.h"
#include "maze.h"

// ------------------- global types
typedef struct mazenode {
    // Array of 4 mazenodes. Index 0 is west, 1 is north, 2 is south, 3 is east
        // (unexplored = null)
        // (open space = pointer to next node)
        // (wall = pointer to self)
    struct mazenode *neighbors[4];

    // array of booleans to show whether each avatarID is present
    bool avatars[10];

    // (x,y) coordnates
    int x;
    int y;
} mazenode_t;


typedef struct maze {
    int height;
    int width;
    int numAvatars;
    mazenode_t ***array;    // 2d array of mazenodes. Index like this: array[y][x]
} maze_t;


// ------------------- local functions prototypes
static mazenode_t *mazenode_new(int x, int y);


// ------------------- local functions
// Initalize a new mazenode & return it. NULL on failure. Caller must delete
// Neighbors init to NULL, caller gives (x,y), avatar init to -1
static mazenode_t *mazenode_new(int x, int y) {
    mazenode_t *node = malloc(sizeof(mazenode_t));
    if (node == NULL) {
        return NULL;
    } else {
        node->neighbors[M_WEST] = NULL;
        node->neighbors[M_NORTH] = NULL;
        node->neighbors[M_SOUTH] = NULL;
        node->neighbors[M_EAST] = NULL;
        node->x = x;
        node->y = y;
        for (int ID = 0; ID < 10; ID++) {
            node->avatars[ID] = false;
        }
        return node;
    }
}


// Frees contents of a mazenode
static void mazenode_delete(mazenode_t *node) {
    free(node->neighbors);
}


// ------------------- global functions

// Caller passes in the maze, (x,y) coordinates for a location in it, and a pointer to the new neighbor.
// unexplored = null, open space = pointer to next node, wall = pointer to self
// 0 for West, 1 for North, 2 for South, 3 for East.
// Returns true on success, false on failure
bool set_neighbor(maze_t *maze, int x, int y, const int d, int neighbor_x, int neighbor_y) {
    if (maze == NULL) {
        fprintf(stderr, "Error: NULL maze passed in");
        return false;
    }

    if (d < 0 || d > 3) {
        fprintf(stderr, "Error: 'd' must be 0 (West), 1 (North), 2 (South), or 3 (East)\n");
        return false;
    }

    if (x < 0 || y < 0 || neighbor_x < 0 || neighbor_y < 0) {
        fprintf(stderr, "Error: Coordinates cannot be negative");
        return false;
    }

    if (x >= maze->width || y >= maze->height || neighbor_x >= maze->width || neighbor_y >= maze->height) {
        fprintf(stderr, "Error: Coordinates must be less than height/width. Max x value: %d. Max y value: %d, (%d,%d)", maze->width-1, maze->height-1, x, y);
        return false;
    }

    maze->array[y][x]->neighbors[d] = maze->array[neighbor_y][neighbor_x];
    return true;
}


// Creates a new maze of set width and height, filled with mazenodes. Caller has to delete it.
maze_t *maze_new(int height, int width, int numAvatars) {
    maze_t *maze = malloc(sizeof(maze_t));
    if (maze == NULL) {
        fprintf(stderr, "Error: maze allocation failed\n");
        return NULL;
    } else {
        if (height < 1 || width < 1) {
            fprintf(stderr, "Error: width and height must be at least 1");
        }
        maze->height = height;
        maze->width = width;
        maze->numAvatars = numAvatars;
        mazenode_t ***array = calloc(height, sizeof(mazenode_t **));
        maze->array = array;

        for (int y = 0; y < height; y++) {
            // Create each row
            mazenode_t **row = calloc(width, sizeof(mazenode_t *));
            maze->array[y] = row;
            for (int x = 0; x < width; x++) {
                // Fill the rows
                row[x] = mazenode_new(x, y);

                // If the node is along the border, set it to a wall
                if (x == 0) {
                    set_neighbor(maze, x, y, M_WEST, x, y); // Western border wall
                }
                if (x == width-1) {
                    set_neighbor(maze, x, y, M_EAST, x, y); // Eastern border wall
                }
                if (y == 0) {
                    set_neighbor(maze, x, y, M_NORTH, x, y); // Northern border wall
                }
                if (y == height-1) {
                    set_neighbor(maze, x, y, M_SOUTH, x, y); // Southern border wall
                }
            }
        }
        return maze;
    }
}


// Caller passes in maze and (x,y) coords for a node to be examined
// Returns number of neighbors in the node that are walls.
int wall_count(maze_t *maze, int x, int y) {
    if (maze == NULL) {
        fprintf(stderr, "Error: NULL maze passed in\n");
        return -1;
    }

    if (x < 0 || y < 0) {
        fprintf(stderr, "Error: Coordinates cannot be negative");
        return false;
    }

    if (x >= maze->width || y >= maze->height) {
        fprintf(stderr, "Error: Coordinates must be less than height/width. Max x value: %d. Max y value: %d", maze->width-1, maze->height-1);
        return false;
    }

    mazenode_t *node = maze->array[y][x];

    int wall_count = 0;
    for (int i = 0; i < 4; i++) {
        if (node->neighbors[i] == NULL) {
            continue;
        } else if (node->neighbors[i]->x == node->x && node->neighbors[i]->y == node->y) {
            wall_count++;
        }
    }
    return wall_count;
}


// Frees all the mazenodes in a maze
void maze_delete(maze_t *maze) {
    if (maze != NULL) {
        for (int y = 0; y < maze->height; y++) {
            for (int x = 0; x < maze->width; x++) {
                if (maze->array[y][x] != NULL) {
                    mazenode_delete(maze->array[y][x]);
                }
            }
            free(maze->array[y]);
        }
        free(maze->array);
        free(maze);
    }
}


// Prints each mazenode. Not final print method, intended for unit testing
void unit_mazenode_print(maze_t *maze, int x, int y, FILE *fp) {
    if (fp != NULL) {
        if (maze != NULL) {
            if (x >= 0 || y >= 0 || x < maze->width || y < maze->height) {
                if (maze->array[y][x] != NULL) {
                    fputc('[', fp);

                    // Print the avatar status
                    fprintf(fp, " {%d} ", get_avatar(maze, x, y));

                    // Print the current node's coordinates
                    fprintf(fp, " {(%d,%d)} ", x, y);

                    // Print the neighbors' coordinates
                    for (int i=0; i<4; i++) {
                        fputs(" {", fp);
                        switch(i) {
                            case M_WEST:         // West neighbor
                                fputs("W:", fp);
                                if (x <= 0) {
                                    // Border walls
                                    fputs("(border)", fp);
                                } else if (maze->array[y][x]->neighbors[i] == NULL) {
                                    // Unexplored directions
                                    fputs("(null)", fp);
                                } else if (maze->array[y][x]->neighbors[i]->x == maze->array[y][x]->x && maze->array[y][x]->neighbors[i]->y == maze->array[y][x]->y) {
                                    // Walls (self-references)
                                    fputs("(wall)", fp);
                                } else {
                                    // Comfirmed neighbors
                                    fprintf(fp, "(%d,%d)", x-1, y);
                                }
                                break;
                            
                            case M_SOUTH:         // South neighbor
                                fputs("S:", fp);
                                if (y >= maze->height-1) {
                                    // Border walls
                                    fputs("(border)", fp);
                                } else if (maze->array[y][x]->neighbors[i] == NULL) {
                                    // Unexplored directions
                                    fputs("(null)", fp);
                                } else if (maze->array[y][x]->neighbors[i]->x == maze->array[y][x]->x && maze->array[y][x]->neighbors[i]->y == maze->array[y][x]->y) {
                                    // Walls (self-references)
                                    fputs("(wall)", fp);
                                } else {
                                    // Comfirmed neighbors
                                    fprintf(fp, "(%d,%d)", x, y+1);
                                }
                                break;
                            
                            case  M_NORTH:         // North neighbor
                                fputs("N:", fp);
                                if (y <= 0) {
                                    // Border walls
                                    fputs("(border)", fp);
                                } else if (maze->array[y][x]->neighbors[i] == NULL) {
                                    // Unexplored directions
                                    fputs("(null)", fp);
                                } else if (maze->array[y][x]->neighbors[i]->x == maze->array[y][x]->x && maze->array[y][x]->neighbors[i]->y == maze->array[y][x]->y) {
                                    // Walls (self-references)
                                    fputs("(wall)", fp);
                                } else {
                                    // Comfirmed neighbors
                                    fprintf(fp, "(%d,%d)", x, y-1);
                                }
                                break;
                            
                            case M_EAST:         // East neighbor
                                fputs("E:", fp);
                                if (x >= maze->width-1) {
                                    // Border walls
                                    fputs("(border)", fp);
                                } else if (maze->array[y][x]->neighbors[i] == NULL) {
                                    // Unexplored directions
                                    fputs("(null)", fp);
                                } else if (maze->array[y][x]->neighbors[i]->x == maze->array[y][x]->x && maze->array[y][x]->neighbors[i]->y == maze->array[y][x]->y) {
                                    // Walls (self-references)
                                    fputs("(wall)", fp);
                                } else {
                                    // Comfirmed neighbors
                                    fprintf(fp, "(%d,%d)", x+1, y);
                                }
                                break;
                        }
                        fputs("}", fp);
                    }

                    fputs(" ]\n", fp);
                }
            }
        }
    }
}


// Calls unit_maze_print on each node in a given maze
void unit_maze_print(maze_t *maze, FILE *fp) {
    if (maze != NULL) {
        for (int y = 0; y < maze->height; y++) {
            for (int x = 0; x < maze->width; x++) {
                /*
                if (y == 0) {
                    printf("\nNorth neighbor: (%d,%d)\n", maze->array[y][x]->neighbors[1]->x, maze->array[y][x]->neighbors[1]->y);
                }
                */
                unit_mazenode_print(maze, x, y, fp);
            }
        }
    }
}

int get_avatar(maze_t *maze, int x, int y) {
    int ID;
    for (ID = 0; ID < maze->numAvatars && !maze->array[y][x]->avatars[ID]; ID++);
    if (ID == maze->numAvatars) return -1;
    return ID;
}

// Puts an avatar into the maze
bool set_avatar(maze_t *maze, int x, int y, int avatar_id, bool status) {
    if (avatar_id >= maze->numAvatars || avatar_id < 0) {
        fprintf(stderr, "Error: Avatar ID out of bounds");
        return false;
    }
    if (x < 0 || y < 0) {
        fprintf(stderr, "Error: Coordinates cannot be negative");
        return false;
    }

    if (x >= maze->width || y >= maze->height) {
        fprintf(stderr, "Error: Coordinates must be less than height/width. Max x value: %d. Max y value: %d", maze->width-1, maze->height-1);
        return false;
    }

    if (maze != NULL) {
        if (maze->array[y][x] != NULL) {
            maze->array[y][x]->avatars[avatar_id] = status;
            return true;
        } else {
            fprintf(stderr, "Error: node at location (%d,%d) is NULL\n", x, y);
            return false;
        }
    } else {
        fprintf(stderr, "Error: maze is NULL\n");
        return false;
    }
}


// Returns XYPos of the neighbor of a node in a given direction
XYPos check_neighbor(maze_t *maze, int x, int y, int d) {
    XYPos neighbor;
    if (maze->array[y][x]->neighbors[d] == NULL) {
        neighbor.x = -1;
        neighbor.y = -1;
        return neighbor;
    }
    neighbor.x = maze->array[y][x]->neighbors[d]->x;
    neighbor.y = maze->array[y][x]->neighbors[d]->y;
    return neighbor;
}



/********************************************************
 **************** unit testing **************************
 ********************************************************/

#ifdef UNIT_TEST

int test_newmaze1() {
    printf("Creating maze of height 3 and width 3 with 3 avatars\n");
    maze_t *maze = maze_new(3, 3, 3);
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

    printf("Deleting maze\n");
    maze_delete(maze);
    printf("----------------------------------------------------------------\n");
    return 0;
}

/*
int main() {
    test_newmaze1();
    exit(0);
}
*/
#endif // UNIT_TEST
