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


/**
 * LocationEnum - This enum contains all of the locations at which to print debug
 *                information from using dbgOutputLoc()
 */
enum LocationEnum {
    //ISR
    DRIVERTMR0_ENTER_ISR = 1,
    DRIVERTMR0_LEAVE_ISR, //2
    DRIVERTMR0_BEFORE_SEND_TO_QUEUE, //3
    DRIVERTMR0_AFTER_SEND_TO_QUEUE, //4
    DRIVERTMR0_BEFORE_RECEIVE_FR_QUEUE, //5
    DRIVERTMR0_AFTER_RECEIVE_FR_QUEUE, //6
    //Task Specific
    APP1_ENTER_TASK = 20,
    APP1_BEFORE_WHILELOOP, //21
    APP1_BEFORE_SEND_TO_QUEUE, //22
    APP1_AFTER_SEND_TO_QUEUE, //23
    APP1_BEFORE_RECEIVE_FR_QUEUE, //24
    APP1_AFTER_RECEIVE_FR_QUEUE, //25

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

void dbgOutputBlock(int check);

void dbgOutputBlockISR(int check);

#endif

/*******************************************************************************
 End of File
 */
