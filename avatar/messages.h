/**
 * The header file for our messages.c file, which handles the messages
 * sent from client to server
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

/**
 * SUMMARY: Checks the type of the message from the server and prints the correct error message
 * 
 * PARAMETERS:
 *     AM_Message server_message    the message sent by the server
 * 
 * RETURN:
 *      none:
 */
int errorMessage(char *filename, AM_Message server_message);

/**
 * SUMMARY: check to see if the server message is valid when an avatar attempts to move
 * 
 * PARAMETERS:
 *      int turn            whether or not if it is this thread's turn
 *      int comm_sock       the socket
 *      AM_Message client   message avatar send to server
 *      int AvatarId        the Avatar that sent the message
 *      int Direction       the proposed direction
 *      AM_Message server   the message received by the server
 * 
 * RETURN:
 *      AM_Message server   the message received by the server
 */

int validMessageTurn (int comm_sock, AM_Message client, int AvatarId, int Direction, AM_Message server_avatar_turn);

AM_Message receiveMessage (int comm_sock, AM_Message server_avatar_turn);

#endif //_MESSAGES_H
