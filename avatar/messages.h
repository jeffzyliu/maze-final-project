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

#endif //_MESSAGES_H