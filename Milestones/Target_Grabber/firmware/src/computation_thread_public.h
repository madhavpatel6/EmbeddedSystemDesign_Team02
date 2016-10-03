#ifndef _COMPUTATION_THREAD_PUBLIC_H    /* Guard against multiple inclusion */
#define _COMPUTATION_THREAD_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "queue.h"

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
  
// This struct contains the three IR sensor values in uint32_t because
// that is what the adc outputs
typedef struct {
    uint32_t IR_0;
    uint32_t IR_1;
    uint32_t IR_2;
} TargetAlignment_Type;
    
    
    int computation_thread_SendValToMsgQ(float adcVal);
    int computation_thread_SendValToMsgQFromISR(TargetAlignment_Type adcVal, BaseType_t *pxHigherPriorityTaskWoken);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _COMPUTATION_THREAD_PUBLIC_H */

/* *****************************************************************************
 End of File
 */
