/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Team 2

  @File Name
    debug.h

  @Summary
    This file serves as a header file for useful debug utilities.

  @Description
    This file serves as a header file for useful debug utilities.
 */
/* ************************************************************************** */

#ifndef _DEBUG_H    /* Guard against multiple inclusion */
#define _DEBUG_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
//#include "system_config.h"
//#include "system_definitions.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * LocationEnum - This enum contains all of the locations at which to print debug
 *                information from using dbgOutputLoc()
 */
enum LocationEnum {
    // USART ISR
    ENTER_USART0_ISR = 1,                // 1
    LEAVE_USART0_ISR,                    // 2
    BEFORE_SEND_TO_QUEUE_USART0_ISR,     // 3
    AFTER_SEND_TO_QUEUE_USART0_ISR,      // 4
    BEFORE_RECEIVE_FR_QUEUE_USART0_ISR,  // 5
    AFTER_RECEIVE_FR_QUEUE_USART0_ISR,   // 6
    
    // ADC ISR
    ENTER_ADC_ISR = 10,         // 10
    LEAVE_ADC_ISR,              // 11
    BEFORE_SEND_TO_Q_ISR,       // 12
    BEFORE_RECEIVE_FROM_Q_ISR,  // 13
    AFTER_SEND_TO_Q_ISR,        // 14
    AFTER_RECEIVE_FROM_Q_ISR,   // 15
    ADDING_ADC_VAL_ISR,         // 16

    // RX THREAD Debug
    ENTER_RXTHREAD = 26,                              // 26
    LEAVE_RXTHREAD,                                   // 27
    BEFORE_WHILELOOP_RXTHREAD,                        // 28
    BEFORE_SEND_TO_QUEUE_RXTHREAD,                    // 29
    AFTER_SEND_TO_QUEUE_RXTHREAD,                     // 30
    BEFORE_RECEIVE_FR_QUEUE_RXTHREAD,                 // 31
    AFTER_RECEIVE_FR_QUEUE_RXTHREAD,                  // 32
    BEFORE_RECEIVE_FR_QUEUE_READFROMQUEUE_RXTHREAD,   // 33
    AFTER_RECEIVE_FR_QUEUE_READFROMQUEUE_RXTHREAD,    // 34
    
    // ADC APP DEBUG
    ENTER_TASK_ADC_APP = 40,            // 40
    BEFORE_WHILE_ADC_APP,               // 41
    BEFORE_SEND_TO_Q_ADC_APP,           // 42
    BEFORE_RECEIVE_FROM_Q_ADC_APP,      // 43
    AFTER_SEND_TO_Q_ADC_APP,            // 44
    AFTER_RECEIVE_FROM_Q_ADC_APP,       // 45
    ENTER_CONVERTTOCM_ADC_APP,          // 46
    LEAVE_CONVERTTOCM_ADC_APP,          // 47
           
     
    //Messages.c Debug
    ENTER_CHECKSUM_MESSAGE_C = 60,                              // 60
    LEAVE_CHECKSUM_MESSAGE_C,                                   // 61
    ENTER_CREATEMESSAGE_MESSAGE_C,                              // 62
    LEAVE_CREATEMESSAGE_MESSAGE_C,                              // 63
    ENTER_PARSEMESSAGE_MESSAGE_C,                               // 64
    LEAVE_PARSEMESSAGE_MESSAGE_C,                               // 65
    CASE_IDLE_STATE_PARSEMESSAGE_MESSAGE_C,                     // 66
    CASE_CHECK_DESTINATION_CHAR_PARSEMESSAGE_MESSAGE_C,         // 67
    CASE_CHECK_SOURCE_CHAR_PARSEMESSAGE_MESSAGE_C,              // 68
    CASE_CHECK_MESSAGE_COUNT_PARSEMESSAGE_MESSAGE_C,            // 69
    CASE_GET_DATALENGTH_UPPER_PARSEMESSAGE_MESSAGE_C,           // 70
    CASE_GET_DATALENGTH_LOWER_PARSEMESSAGE_MESSAGE_C,           // 71
    CASE_GET_DATA_PARSEMESSAGE_MESSAGE_C,                       // 72
    CASE_GET_CHECK_SUM_PARSEMESSAGE_MESSAGE_C,                  // 73
    CASE_CHECK_ENDCHAR_PARSEMESSAGE_MESSAGE_C,                  // 74
    BEFORE_FIRST_IF_GET_DATA_PARSEMESSAGE_MESSAGE_C,            // 75
    IN_FIRST_IF_GET_DATA_PARSEMESSAGE_MESSAGE_C,                // 76
    IN_SECOND_IF_GET_DATA_PARSEMESSAGE_MESSAGE_C,               // 77
    IN_THIRD_IF_GET_DATA_PARSEMESSAGE_MESSAGE_C,                // 78
    AFTER_THIRD_IF_GET_DATA_PARSEMESSAGE_MESSAGE_C,             // 79
    
    // MESSAGE CONTROLLER THREAD Debug
    ENTER_MESSAGE_CONTROLLER_THREAD = 90,                       // 90
    LEAVE_MESSAGE_CONTROLLER_THREAD,                            // 91
    CASE_EXTERNAL_REQUEST_RESPONSE_MESSAGE_CONTROLLER_THREAD,   // 92
    CASE_SEND_REQUEST_MESSAGE_CONTROLLER_THREAD,                // 93
    CASE_UPDATE_MESSAGE_CONTROLLER_THREAD,                      // 94
    BEFORE_READ_FROM_Q_MESSAGE_CONTROLLER_THREAD,               // 95
    AFTER_READ_FROM_Q_MESSAGE_CONTROLLER_THREAD,                // 96
    
    // TIMER INSTANCE 0 ISR Debug
    ENTER_TMR_INSTANCE_0_ISR = 130,       // 130
    LEAVE_TMR_INSTANCE_0_ISR,             // 131
    SET_ADC_FLAG_TMR_INSTANCE_0_ISR,      // 132
    
    // TIMER INSTANCE 1 ISR Debug
    ENTER_TMR_INSTANCE_1_ISR = 140,       // 140
    LEAVE_TMR_INSTANCE_1_ISR,             // 141
    BEFORE_SEND_TO_Q_TMR_INSTANCE_1_ISR,  // 142
    AFTER_SEND_TO_Q_TMR_INSTANCE_1_ISR,   // 143
    
    
    // TX THREAD Debug
    ENTER_TXTHREAD = 160,                 // 160
    BEFORE_WHILELOOP_TXTHREAD,            // 161
    BEFORE_SEND_TO_QUEUE_TXTHREAD,        // 162
    AFTER_SEND_TO_QUEUE_TXTHREAD,         // 163
    BEFORE_RECEIVE_FR_QUEUE_TXTHREAD,     // 164
    AFTER_RECEIVE_FR_QUEUE_TXTHREAD,      // 165
    
    
    
    CREATE_MESSAGE_DONE = 255,                // 255
};

/**
 * [dbgOutputVal Debug function that is utilized to output an 8 bit value to
 *               PORT E; ChipKIT pins 37-30 with the MSB at Pin 30 and LSB at Pin 37]
 * @param outVal [Value to put to the bus]
 */
void dbgOutputVal(unsigned char outVal);

/**
 * [dbgOutputLoc Debug function that is utilized to output an 8 bit value to ChipKIT pins 47-40
 *               with the MSB at Pin 40 and LSB at Pin 47]
 * @param outVal [The location value to put to the bus; see debug.h for the LocationEnum]
 */
void dbgOutputLoc(unsigned char outVal);

/**
 * [dbgOutputBlock This function will perform a check on the input value. If the input is 0 then we hault all ISR(s)/Timers/Tasks]
 * @param check [value to check]
 */
void dbgOutputBlock(int check);

/**
 * [dbgOutputBlockISR This function will perform a check on the input value. If the input is 0 then we hault all ISR(s)/Timers/Tasks]
 * @param check [value to check]
 */
void dbgOutputBlockISR(int check);

#endif

/*******************************************************************************
 End of File
 */
