/**
 * ui.c
 * 
 * collection of function(s) to print ASCII UI to the command line
 * see ui.h for documentation
 * 
 * AUTHOR: Celina Tala, Jeff Liu, Willem Klein Wassink
 */ 

#include "../amazing.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"

void print_ui(/*I don't know what to pass in yet besides maze_t*/)
{
    printf("****************************************\n");
    // print status message; do we want to have this in logfile.c/h?
    // do we want to have logfile.c/h return strings, and mutex lock each avatar instead
    // of each function that changes things? we can centralize mutex locks that way
    // and have auxilliary functions only manipulate data

    // waiting on maze_t to finish this

    printf("****************************************\n\n");
}