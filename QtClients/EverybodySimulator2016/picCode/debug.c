/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Team 2

  @File Name
    debug.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "debug.h"

/**
 * [dbgOutputVal Debug function that is utilized to output an 8 bit value to
 *               PORT E; ChipKIT pins 37-30 with the MSB at Pin 30 and LSB at Pin 37]
 * @param outVal [Value to put to the bus]
 */
void dbgOutputVal(unsigned char outVal) {
}

/**
 * [dbgOutputLoc Debug function that is utilized to output an 8 bit value to ChipKIT pins 47-40
 *               with the MSB at Pin 40 and LSB at Pin 47]
 * @param outVal [The location value to put to the bus; see debug.h for the LocationEnum]
 */
void dbgOutputLoc(unsigned char outVal) {

}

/**
 * [dbgOutputBlock This function will perform a check on the input value. If the input is 0 then we hault all ISR(s)/Timers/Tasks]
 * @param check [value to check]
 */
void dbgOutputBlock(int check) {

}

/**
 * [dbgOutputBlockISR This function will perform a check on the input value. If the input is 0 then we hault all ISR(s)/Timers/Tasks]
 * @param check [value to check]
 */
void dbgOutputBlockISR(int check) {

}
/* *****************************************************************************
 End of File
 */
