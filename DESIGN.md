# Maze Project Design Spec
## Team Willpower: Celina Tala, Jeff Liu, Willem Klein Wassink
## CS50 Winter 2020

### User interface
The command line usage is: 
`usage: ./AMStartup -n nAvatar -d Difficulty -h Hostname`

where
* nAvatar: is the number of avatars we want in our maze (from 1 to 10)
* Difficulty: is the difficulty of our maze (from 0 to 9)
* Hostname: the hostname of our server

We will be checking to see if the command line has all seven parameters. Because we utilize command line options, the order of the arguments does not matter.


### Input & Outputs
#### Inputs
The only user interaction with the user is through the command line. Refer to user interface above.

The program will take inputs from the server.

#### Outputs
Our program will maintain a single log file logging the actions of each Avatar. The first line of the log file should have the MazePort, AvatarId, and the date and time. It will then track the movements of each avatar, documenting which avatar moved and if the avatar ran into a wall. The file will either end with an error message or `AM_MAZE_SOLVED`. 

The program will also output a ASCII user interface that prints out the maze and where each avatar is. With each movement, a new maze is printed out showcasing the new position of each avatar. The top of the graphic will explicitly state which avatar is moving to which position.


### Decomposition into modules

* AMStartup
    * Parses user input
    * Initializes the game
    * Starts server communication
    * Initialize our global maze
    * Sets up threads for avatars
    * Starts the game
    * After all the threads exit, clean up data

* clientParameter
    * We create the `struct avatar_p` required to pass parameters into our avatar method
* avatar
    * This is where each of the individual threads will start
    * Keeps track of the last move request and the last location as well as the updated location
    * Holds a loop where it listens for server messages and sends its move messages when necessary
    * Updates the global maze when wall updates
* move decider
    * Determines what move to attempt next based on the limited information it has
    * Planned implementation: enhanced right-hand-follower where interprocess communication is used to expedite checking and block off dead ends
* log writer
    * Function to write the results of messages to the logfile
    * This function will have a mutex lock on it so only one thread can access it at once
* maze update
    * This is where we will update our global maze based one each server message
    * This will also have a mutex lock on it as only one thread should be able to upate the global maze at once
* UI writer
    * prints the status of the maze during every move at the command line

### Pseudocode for logic & algorithmic flow

**AMStartup**
```
send init_message to server
listen for response
    if response received is NOT init_message.ok
        exit program
open log file and write the first line w/ username etc.
make global maze, allocate 2d array, fill w/ mazenodes
    NSEW pointers all "null" for unexplored
    avatarID -1 for no avatar
    coordinates as they appear in the array
for each avatar
    create a thread by calling avatar method
When all of the individual threads exit
    do clean up actions such as closing log files
```

**avatar**
```
send AM_avatar_ready message to server
listen for response in a loop:
    if the response is AM_maze_solved or another stop game state
        exit the loop
    if the response is AM_avatar_turn
        compare new location to last location, see how the move went, update the move in the map using maze_update
        print out the new gamestate with UI_printer
        if the moveID is this avatar's ID
            record the avatar move that just happened using log_writer
            pass our last point, current point, and last move direction into move_decider to calculate next direction
            send server request to move in that direction
            record the move request for reference next time
after loop exit:
close the connection if necessary
clean up data structures
```

**move_decider**
```
use avatar_moved to determine if we moved last time
    set proposed heading to the right of the the direction we last moved in
else (location did not change, movement failed):
    set proposed heading to the left of the last proposed heading
check if our proposed direction will run into a wall by checking the move with our global map struct
    if so, reprocess move heading until we head into a traversible or unknown direction
return proposed move heading
```

**log_writer**
```
Extract message from AM_AVATAR_TURN and look at turnID
Get the location of the avatar associated with the turnID
Utilize avatar_moved to check if the avatar moved.
If we receive 8 (null move):
    write that it hit a wall
else:
    Write the updated position of all avatars
```

**maze_update**
```
Extract message from AM_AVATAR_TURN
Utilize avatar_moved to see if all of the avatars have moved
Update the global mazenodes depending on which moves succeeded and which failed (failed = closed wall, success = open)
Update the avatar locations inside the maze data structure
```

**avatar_moved**
```
Takes in last and current location
Calculate the direction it moved
Returns integer corresponding to what happened
    return 0-3 (NSEW) as true/successful move specifying the direction(look at amazing.h for specification)
    return 8 (nullmove) if the avatar did not move; this functions as the false condition
```


**UI_printer**
```
print a divider of *** characters
print a message based on what happened (insert, move avatar, etc)
print out the maze by doing the following:
    print a 2 line header with the x indices and the top walls of the maze
    for each row except the last:
        print the row number
        print the left wall
        for each column:
            print the intersections
            (optional)print all bottom and side edges at each maze location, noting whether they're unexplored, open, or closed
            if a coordinate matches the location of an avatar, print the avatar number there
        print the right wall
    for the bottom row:
        do the same except print the continuous bottom wall
print an end divider of *** characters    
```

### Data Structures

* Maze struct: represents full maze (interprocess global struct)
    * int width, int height
    * 2d array of mazenodes
* Mazenodes: each represents one (x,y) location in the maze. 
    * contains its own coordinates for reference from inside
    * contains four pointers to adjacent nodes that can be either walls, represented by a pointer to itself, unexplored, which is null, or open, which points to an adjacent node in the array
    * contains an integer for avatarID which would be -1 if no avatar or 0-9 if an avatar is present
* avatar_p
    * contains all six parameters necessary to start an avatar in a tuple so they can be passed together
    * The parameters include avatarId, numAvatar, difficulty, hostname, mazeport, and filename.

* XYPos (amazing.h)
    * a struct that holds x and y coordinates

* Avatar (amazing.h)
    * A struct representing each avatar in the maze
    * It holds the id and current position of each avatar 

* AM_Message (amazing.h)
    * This is how the client and server communciates with each other
    * There are more specific messages within this data structure (refer to "amazing.h" for the exact messages)
