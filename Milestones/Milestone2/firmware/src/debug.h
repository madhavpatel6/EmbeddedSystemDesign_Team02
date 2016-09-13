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
#include "system_config.h"
#include "system_definitions.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
/**
 * LocationEnum - This enum contains all of the locations at which to print debug
 *                information from using dbgOutputLoc()
 */
enum LocationEnum {
    // USART ISR
    USART0_ENTER_ISR = 1,           // 1
    USART0_LEAVE_ISR,               // 2
    USART0_BEFORE_SEND_TO_QUEUE,    // 3
    USART0_AFTER_SEND_TO_QUEUE,     // 4
    USART0_BEFORE_RECEIVE_FR_QUEUE, // 5
    USART0_AFTER_RECEIVE_FR_QUEUE,  // 6
    
    // ADC ISR
    ENTER_ADC_ISR = 10,         // 10
    LEAVE_ADC_ISR,              // 11
    BEFORE_SEND_TO_Q_ISR,       // 12
    BEFORE_RECEIVE_FROM_Q_ISR,  // 13
    AFTER_SEND_TO_Q_ISR,        // 14
    AFTER_RECEIVE_FROM_Q_ISR,   // 15
    ADDING_ADC_VAL_ISR,         // 16

    //Task Specific
    UARTTXTHREAD_ENTER_TASK = 20,           // 20
    UARTTXTHREAD_BEFORE_WHILELOOP,          // 21
    UARTTXTHREAD_BEFORE_SEND_TO_QUEUE,      // 22
    UARTTXTHREAD_AFTER_SEND_TO_QUEUE,       // 23
    UARTTXTHREAD_BEFORE_RECEIVE_FR_QUEUE,   // 24
    UARTTXTHREAD_AFTER_RECEIVE_FR_QUEUE,    // 25

    UARTRXTHREAD_ENTER_TASK,                // 26
    UARTRXTHREAD_BEFORE_WHILELOOP,          // 27
    UARTRXTHREAD_BEFORE_SEND_TO_QUEUE,      // 28
    UARTRXTHREAD_AFTER_SEND_TO_QUEUE,       // 29
    UARTRXTHREAD_BEFORE_RECEIVE_FR_QUEUE,   // 30
    UARTRXTHREAD_AFTER_RECEIVE_FR_QUEUE,    // 31
    
    // ADC APP DEBUG
    ENTER_TASK_ADC_APP = 40,            // 40
    BEFORE_WHILE_ADC_APP,               // 41
    BEFORE_SEND_TO_Q_ADC_APP,           // 42
    BEFORE_RECEIVE_FROM_Q_ADC_APP,      // 43
    AFTER_SEND_TO_Q_ADC_APP,            // 44
    AFTER_RECEIVE_FROM_Q_ADC_APP,       // 45
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
