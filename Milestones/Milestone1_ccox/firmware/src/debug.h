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

#include "../../../../framework/system/ports/sys_ports.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
#define DEBUG_ON
#ifdef DEBUG_ON

    // ISR debug locations
#define ENTER_TIMER_ISR 0x00
#define LEAVE_TIMER_ISR 0x01
#define BEFORE_SEND_TO_Q_ISR 0x02
#define BEFORE_RECEIVE_FROM_Q_ISR 0x03
#define AFTER_SEND_TO_Q_ISR 0x04
#define AFTER_RECEIVE_FROM_Q_ISR 0x05   
    
    // TASK debug locations
#define ENTER_TASK_APP1 0x06
#define BEFORE_WHILE_APP1 0x07
#define BEFORE_SEND_TO_Q_APP1 0x08
#define BEFORE_RECEIVE_FROM_Q_APP1 0x09
#define AFTER_SEND_TO_Q_APP1 0x10
#define AFTER_RECEIVE_FROM_Q_APP1 0x11
    
    //Broken function
#define ERROR_RETURN 0x12
    
#endif
    
    void dbgOutputValue(char outVal);
    void dbgOutputLoc(unsigned int outVal);
    void dbgLocTrigger(bool trigger);
    void dbgCheckFuncReturnISR(int checkVal);
    void dbgCheckFuncReturn(int checkVal);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
