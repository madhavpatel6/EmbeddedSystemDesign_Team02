/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _MESSAGELAYER_H    /* Guard against multiple inclusion */
#define _MESSAGELAYER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define STARTOFTEXT 0x02
#define ENDOFTEXT 0x03
#define SEARCHERMOVER 'S'
#define TARGETLOCATOR 'L'
#define PATHFINDER 'P'
#define TARGETGRABBER 'G'
#define SERVER 'H'
#define MAXMESSAGESIZE 300
#define MYMODULE PATHFINDER

#ifdef __cplusplus
extern "C" {
#endif

int CreateMessage(char buf[], char messageData[], char destination, char messagecount);

void BuildJSON(char buf[]);

char checksum(char* s);

typedef enum {
    IDLE_STATE,
    CHECK_DESTINATION_CHAR,
    CHECK_SOURCE_CHAR,
    CHECK_MESSAGE_COUNT,
    GET_DATALENGTH_UPPER,
    GET_DATALENGTH_LOWER,
    GET_DATA,
    GET_CHECK_SUM,
    CHECK_ENDCHAR,
} STATES;

bool ParseMessage(char c, char data[], char* source, char* messageCount, bool *isError);

#ifdef __cplusplus
}
#endif



#endif /* _MESSAGELAYER_H */

/* *****************************************************************************
 End of File
 */
