// Created By: Chris Cox
// Date: 8/31/16
// File: debug.h

#ifndef _DEBUG_H    /* Guard against multiple inclusion */
#define _DEBUG_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "system_config.h"
#include "system_definitions.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
#define DEBUG_ON
#ifdef DEBUG_ON

    // ISR debug locations
#define ENTER_ADC_ISR 0x00
#define LEAVE_ADC_ISR 0x01
#define BEFORE_SEND_TO_Q_ISR 0x02
#define BEFORE_RECEIVE_FROM_Q_ISR 0x03
#define AFTER_SEND_TO_Q_ISR 0x04
#define AFTER_RECEIVE_FROM_Q_ISR 0x05  
#define ADDING_ADC_VAL_ISR 0x06
    
    // TASK debug locations
#define ENTER_TASK_ADC_APP 0x07
#define BEFORE_WHILE_ADC_APP 0x08
#define BEFORE_SEND_TO_Q_ADC_APP 0x09
#define BEFORE_RECEIVE_FROM_Q_ADC_APP 0x10
#define AFTER_SEND_TO_Q_ADC_APP 0x11
#define AFTER_RECEIVE_FROM_Q_ADC_APP 0x12
    
    //Broken function
#define ERROR_RETURN 0x13
#define CORRECT_RETURN 0x14
    
#endif
    
    void dbgOutputValue(char outVal);
    void dbgOutputLoc(unsigned int outVal);
    void dbgLocTrigger(bool trigger);
    void dbgCheckFuncReturn(int checkVal);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
