#ifndef _ADC_APP_PUBLIC_H    /* Guard against multiple inclusion */
#define _ADC_APP_PUBLIC_H


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
    
// This struct contains the 8 IR sensor values in uint32_t because
// that is what the adc outputs
typedef struct {
    float IR_0;
    float IR_1;
    float IR_2;
    float IR_3;
    float IR_4;
    float IR_5;
    float IR_6;
    float IR_7;
} LineObj;
    
    int ADC_THREAD_SendToQueue(LineObj lineObj);
    int ADC_THREAD_SendToQueueISR(LineObj lineObj, BaseType_t *pxHigherPriorityTaskWoken);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
