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
    
    sprintf(buf, "%x%c%x%x%s",0,destination,0,0,messageData);
    buf[0] = 2;
    buf[2] = 37;
    buf[3] = strlen(messageData);
    int i;
    for (i = 0; i < (strlen(buf)); i++) {
        dbgOutputVal(buf[i]);
    }
    while (i < 512) {
        buf[i] = 3;
        dbgOutputVal(buf[i]);
        i++;
    }
    
    dbgOutputLoc(CREATE_MESSAGE_DONE);
}

/* *****************************************************************************
 End of File
 */
