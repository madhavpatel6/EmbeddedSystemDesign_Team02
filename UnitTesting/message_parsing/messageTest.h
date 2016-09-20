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
#define MYMODULE 'p'
#define MAXMESSAGESIZE 512

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

#endif /* _MESSAGELAYER_H */

/* *****************************************************************************
 End of File
 */
