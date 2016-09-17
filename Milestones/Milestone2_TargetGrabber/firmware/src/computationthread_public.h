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

#ifndef _COMPUTATIONTHREAD_PUBLIC_H    /* Guard against multiple inclusion */
#define _COMPUTATIONTHREAD_PUBLIC_H

#include "queue.h"

#define COMPUTATION_ISR_Q_SIZE float

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    int computationthread_SendValToSensorQFromISR(float sensorAdcVal, BaseType_t *pxHigherPriorityTaskWoken);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _COMPUTATIONTHREAD_PUBLIC_H */

/* *****************************************************************************
 End of File
 */
