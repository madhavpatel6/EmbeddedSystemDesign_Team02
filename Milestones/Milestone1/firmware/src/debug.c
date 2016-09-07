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
    SYS_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_E, outVal);
}

/**
 * [dbgOutputLoc Debug function that is utilized to output an 8 bit value to ChipKIT pins 47-40
 *               with the MSB at Pin 40 and LSB at Pin 47]
 * @param outVal [The location value to put to the bus; see debug.h for the LocationEnum]
 */
void dbgOutputLoc(unsigned char outVal) {
    SYS_PORTS_Clear(PORTS_ID_0, PORT_CHANNEL_B, 0b0011100000000000);
    SYS_PORTS_Clear(PORTS_ID_0, PORT_CHANNEL_G, 0b100000000);
    SYS_PORTS_Clear(PORTS_ID_0, PORT_CHANNEL_A, 0b10000000000);
    SYS_PORTS_Clear(PORTS_ID_0, PORT_CHANNEL_F, 0b11);
    SYS_PORTS_Clear(PORTS_ID_0, PORT_CHANNEL_D, 0b1000000);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 11,(outVal & 0b10000000) >> 7);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 13,(outVal & 0b1000000) >> 6);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 12,(outVal & 0b100000) >> 5);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 8,(outVal & 0b10000) >> 4);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, 10,(outVal & 0b1000) >> 3);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_F, 1,(outVal & 0b10));
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_F, 0,(outVal & 0b100) >> 2);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, 6,(outVal & 0b1));
}

/**
 * [dbgOutputBlock This function will perform a check on the input value. If the input is 0 then we hault all ISR(s)/Timers/Tasks]
 * @param check [value to check]
 */
void dbgOutputBlock(int check) {
    if(!check) {
        dbgOutputLoc(0xff);
        DRV_TMR0_Stop();
        vTaskSuspendAll();
    }
}

/**
 * [dbgOutputBlockISR This function will perform a check on the input value. If the input is 0 then we hault all ISR(s)/Timers/Tasks]
 * @param check [value to check]
 */
void dbgOutputBlockISR(int check) {
    if(!check) {
        dbgOutputLoc(0xff);
        DRV_TMR0_Stop();
        vTaskSuspendAll();
    }
}
/* *****************************************************************************
 End of File
 */
