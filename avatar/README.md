# CS50 Final Project
## Willem Klein Wassink, Celina Tala, Jeff Liu, CS50 Winter 2020

### avatar.c

*avatar.c* is the method that corresponds to each avatar in our maze. When we create our multiple threads, we are calling this file. 

### avatar.c Usage

*avatar.a* is to be included within programs that use the individual avatars.

### avatar.c Method

```c
/**************  avatar
 * The main avatar function that each thread calls
 * 
 * Inputs:
 *      a parameter struct which includes all variables needed to start multithreading socket reading
 * 
 * Outputs:
 *      The exit code if relevant
 *      else none
 */
void *avatar (void *arg);

/************ clientParameters()
 * This function creates the struct avatar_p, which is the parameter we need to
 * pass to our avatar method when we create new threads
 * 
 * Inputs:
 *      int AvatariD        ID for each avatar starting at 0
 *      int nAvatars        total number of avatars
 *      int Difficulty      difficulty level
 *      char *hostname      hostname
 *      int mazeport        the port of maze location
 *      char *filename      the file we are writing to
 *      maze_t *maze        the global maze
 * 
 * Outputs:
 *      the data struct avatar_p with necessary parameters
 */
avatar_p *clientParameters(int AvatarId, int nAvatars, int Difficulty, char *hostname, int mazeport, char *filename, maze_t *maze);
```

### avatar.c Assumptions

None except what is clear from the aformentioned information

---

### messages.c

*messages.c* handles messages between avatars and server

### messages.c Usage

*avatar.a* is to be included within programs that need to use avatars and deal with messages between avatar and server.

### messages.c Method

```c
/********** errorMessage
 * Checks the type of the message from the server and prints the correct error message
 * 
 * Inputs:
 *      the file to write our error
 *      the message sent by the server
 * 
 * Outputs:
 *      exit code
 */
void errorMessage(char *filename, AM_Message server_message);

/**
 * check to see if the server message is valid when an avatar attempts to move
 * 
 * Input:
 *      int comm_sock       the socket
 *      AM_Message client   message avatar send to server
 *      int AvatarId        the Avatar that sent the message
 *      int Direction       the proposed direction
 *      AM_Message server   the message received by the server
 * 
 * Output:
 *      exitcode
 */

int validMessageTurn (int comm_sock, AM_Message client, int AvatarId, int Direction, AM_Message server_avatar_turn);

/************* receiveMessage
 * Gets the new message sent by the server
 * 
 * Input:
 *      int comm_sock       the socket
 *      AM_Message          the last server message
 * 
 * Output:
 *      the new server message
 */
AM_Message receiveMessage (int comm_sock, AM_Message server_avatar_turn);
```

### messages.c Assumptions

None except what is clear from the aformentioned information

---

### Compilation

To compile, run `make` in the parent directory. The makefile should recursively call `make` in each subdirectory.
