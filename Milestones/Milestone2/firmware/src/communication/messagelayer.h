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

bool ParseMessage(char* packedData, char* messageData);

bool CreateMessage(char* messageData, char* packedData);

#endif /* _MESSAGELAYER_H */

/* *****************************************************************************
 End of File
 */
