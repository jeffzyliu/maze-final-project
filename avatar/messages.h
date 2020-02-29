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

/**
 * SUMMARY: Checks the type of the message from the server and prints the correct error message
 * 
 * PARAMETERS:
 *     AM_Message server_message    the message sent by the server
 * 
 * RETURN:
 *      none:
 */
void errorMessage(AM_Message server_message);

AM_Message validMessage (int comm_sock, AM_Message client, int AvatarId, int Direction, AM_Message server_avatar_turn);
#endif //_MESSAGES_H
