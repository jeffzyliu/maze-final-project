# CS50 Final Project
## Willem Klein Wassink, Celina Tala, Jeff Liu, CS50 Winter 2020

### ui.c

*ui.c* exports a method that takes in a pointer to a valid maze and prints out a UI representing the maze to stdout. It is compiled into the *output.a* library.

### ui.c Usage

*output.a* is to be included within programs that need to print a maze out.

### ui.c Method

```c
/* Prints out the maze UI to stdout
 * 
 * Inputs:
 *      A pointer to a valid maze
 * 
 * Outputs:
 *      A printed maze in stdout
 */
void print_ui(maze_t *maze);
```

### ui.c Assumptions

None except what is clear from the aformentioned information

---

### logfile.c

*ui.c* exports methods that allow for the printing of updates on the
avatars' movements to the log file.

### logfile.c Usage

*output.a* is to be included within programs that need to print messages
to a logfile.

### logfile.c Method

```c
/*
 * this method will write the initial starting positions of all the avatars
 * 
 * Input:
 *      char *filename      the file we are writing to
 *      int AvatarId        the Avatar that is writing to the file
 *      int x               x position of the Avatar   
 *      int y               y position of Avatar
 *      XYPos *pos          array of positions of other avatars
 */
void startingState (char *filename, int AvatarId, int x, int y, XYPos *pos);

/*
 * This prints the update when each avatar tries to move
 * 
 * Inputs:
 *      bool last           boolean if it is the last move
 *      char *filename      the file we are writing to
 *      int AvatarId        the Avatar that is writing to the file
 *      int nAvatars        total number of Avatars
 *      XYPos newPos        the new position of the avatar
 *      XYPos oldPos        the old position of the avatar
 *      XYPos *pos          array of positions of other avatars
 */     
void avatarTurned (bool last, char *filename, int AvatarId, int nAvatars, XYPos newPos, XYPos oldPos, XYPos *pos, int d);

/*
 * This prints a message whenever the game exists
 * 
 * Inputs:
 *      char *filename          the file we are writing to
 *      AM_Message finalmessage the finalMessage sent by the server
 */
void exitGame (char *filename, AM_Message finalMessage);
```

### logfile.c Assumptions

None except what is clear from the aformentioned information

---

### Compilation

To compile, run `make` in the parent directory. The makefile should recursively call `make` in each subdirectory.

To test, uncomment the "-DUNIT_TEST" in this directory's makefile, uncomment the main method at the bottom of ui.c, run `make clean`, and then run `make unit`. Usage: "./ui".