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
#define SEARCHERMOVER 's'
#define TARGETLOCATOR 'l'
#define PATHFINDER 'p'
#define TARGETGRABBER 'g'
#define MAXMESSAGESIZE 512
#define WHATPICAMI 'p'

typedef enum CommunicationType_enum { 
    INT,
    FLOAT,
    STRING,
} CommunicationType;

//typedef CommunicationType_enum CommunicationType;

typedef struct CommunicationObject_struct {
    CommunicationType type;
    int intVal;
    float floatVal;
    char string[MAXMESSAGESIZE];
} CommunicationObject;

int CreateMessage(char buf[], char messageData[], char destination);

void BuildJSON(char buf[]);

char checksum(char* s);

typedef enum {
    IDLE_STATE,
    CHECK_DESTINATION_CHAR,
    CHECK_MESSAGE_COUNT,
    GET_DATALENGTH_UPPER,
    GET_DATALENGTH_LOWER,
    GET_DATA,
    GET_CHECK_SUM,
    CHECK_ENDCHAR,
} STATES;

bool ParseMessage(char c, char data[], size_t *size);


#endif /* _MESSAGELAYER_H */

/* *****************************************************************************
 End of File
 */
