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

#ifndef _GRABBERTHREAD_PUBLIC_H    /* Guard against multiple inclusion */
#define _GRABBERTHREAD_PUBLIC_H


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
    
#define GRABBER_CMD_Q_SIZE char
#define GRABBER_ISR_Q_SIZE float
    
    int grabberthread_SendToCMDQ(char grabberCMD);
    int grabberthread_SendValToISRQ(float grabberAdcVal);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _GRABBERTHREAD_PUBLIC_H */

/* *****************************************************************************
 End of File
 */
