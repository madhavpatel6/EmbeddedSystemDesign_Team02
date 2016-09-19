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

#ifndef _UARTTXTHREAD_PUBLIC_H    /* Guard against multiple inclusion */
#define _UARTTXTHREAD_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "communication/messages.h"
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
void UARTTXTHREAD_SendToQueue(CommunicationObject buffer);

void UARTTXTHREAD_SendToQueueISR(CommunicationObject buffer, BaseType_t *pxHigherPriorityTaskWoken);
    
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
