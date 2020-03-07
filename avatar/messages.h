/**
 * Willem Klein Wassink, Jeff Liu, and Celina Tala, CS50 Winter 2020
 * 
 * The header file for our messages.c file, which handles the messages sent from client to server
 */

#ifndef __MESSAGES_H
#define __MESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include "../amazing.h"
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>	  
#include <netdb.h>	
#include <pthread.h> 

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

#endif //_MESSAGES_H
