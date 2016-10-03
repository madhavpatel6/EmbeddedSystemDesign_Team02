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
  
// This struct contains the three IR sensor values
typedef struct {
    uint32_t IR_0;
    uint32_t IR_1;
    uint32_t IR_2;
} TargetAlignment_Type;
    
    
    int adc_app_SendValToMsgQ(float adcVal);
    int adc_app_SendValToMsgQFromISR(TargetAlignment_Type adcVal, BaseType_t *pxHigherPriorityTaskWoken);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
