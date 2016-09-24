#ifndef _ADC_APP_PUBLIC_H    /* Guard against multiple inclusion */
#define _ADC_APP_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include "system_config.h"
#include "system_definitions.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    
    int adc_app_SendValToMsgQ(float adcVal);
    int adc_app_SendValToMsgQFromISR(float adcVal, BaseType_t *pxHigherPriorityTaskWoken);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
