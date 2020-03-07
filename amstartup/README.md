# CS50 Final Project
## Willem Klein Wassink, Celina Tala, Jeff Liu, CS50 Winter 2020

### AMStartup.c

*AMStartup.c* is the program we call with our maze. It is what initially connects to the server and then kickstarts a thread for each avatar.

### AMStartup.c Usage

`./AMStarup -d difficulty -n nAvatars -h hostname`

where:

-d is the difficulty

-n is the nunber of avatars

-h is the hostname

### AMStartup.c Method
We just have the main function as we modulized our project.

```c
/**
 * The main function where we parse command line arguments and start off our threads
 */
int main(int argc, char *argv[])
```

### AMStartup.c Assumptions

None except what is clear from the aformentioned information

### Compilation

To compile, run `make` in the parent directory. The makefile should recursively call `make` in each subdirectory.
