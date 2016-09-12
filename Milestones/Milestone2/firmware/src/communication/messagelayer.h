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

typedef enum DestinationType { PIC32_SearcherMover, PIC32_TargetLocator, PIC32_PathFinder, PIC32_TargetGrabber } DestionationType;
bool ParseMessage(char* packedData, char* messageData);

bool CreateMessage(char* messageData, DestionationType destination,char* packedData);

#endif /* _MESSAGELAYER_H */

/* *****************************************************************************
 End of File
 */
