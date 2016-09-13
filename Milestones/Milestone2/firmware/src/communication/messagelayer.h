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

#include "debug.h"

#define STARTOFTEXT 0x02
#define SEARCHERMOVER 's'
#define TARGETLOCATOR 'l'
#define PATHFINDER 'p'
#define TARGETGRABBER 'g'
#define MAXMESSAGESIZE 512

void ParseMessage(char buf[], char messageData[]);

void CreateMessage(char buf[], char messageData[], char destination);

#endif /* _MESSAGELAYER_H */

/* *****************************************************************************
 End of File
 */
