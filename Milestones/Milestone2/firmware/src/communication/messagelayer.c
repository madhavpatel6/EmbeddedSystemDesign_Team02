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


bool ParseMessage(char* packedData, char* messageData) {
    if(packedData == NULL || messageData == NULL) {
        dbgOutputBlock(false);
    }
    
}

bool CreateMessage(char* messageData, DestionationType destination, char* packedData) {
    if(packedData == NULL || messageData == NULL) {
        dbgOutputBlock(false);
    }
}

/* *****************************************************************************
 End of File
 */
