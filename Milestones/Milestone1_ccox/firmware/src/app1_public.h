// Author: Chris Cox
// File Name: app1_public.h
// Date: 9/1/16

#ifndef _APP1_PUBLIC_H    /* Guard against multiple inclusion */
#define _APP1_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "queue.h"
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    int app1SendTimerValToMsgQ(char msgVal);
    int app1SendTimerValToMsgQFromISR(char msgVal, BaseType_t *pxHigherPriorityTaskWoken);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
