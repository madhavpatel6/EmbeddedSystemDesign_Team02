/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "messagelayer.h"
#include <stdio.h>
//#include <string.h>


void ParseMessage(char *buf, char messageData[]) {
    if(messageData == NULL) {
        dbgOutputBlock(false);
    }
    
}

void CreateMessage(char buf[], char messageData[], char destination) {
    if(messageData == NULL) {
        dbgOutputBlock(false);
    }
    
    // Pack message into string 'buf'
    // Format: start byte, destination, message count, data length, data, checksum
    sprintf(buf, "%c%c%c%c%s",2,destination,37,strlen(messageData),messageData);
    int i;
    for (i = 0; i < (strlen(buf)); i++) {
        dbgOutputVal(buf[i]);
    }
    
    // Calculate checksum of message
    buf[i] = checksum(buf);
    dbgOutputVal(buf[i]);
    buf[i+1] = 4;
    dbgOutputVal(buf[i+1]);
    
    // Append checksum to message
    
    dbgOutputLoc(CREATE_MESSAGE_DONE);
}

// Simple string checksum
char checksum(char* s)
{
	signed char sum = -1;
	while (*s != 0)
	{
		sum += *s;
		s++;
	}
	return sum;
}

/* *****************************************************************************
 End of File
 */
