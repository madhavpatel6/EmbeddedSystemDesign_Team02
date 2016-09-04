// Created By: Chris Cox
// Date: 8/31/16
// File: debug.c
#include "debug.h"

/* Purpose:
 *      This function takes in a preset location value and prints it 
 *      to the I/O lines: RE0-RE7
 *                      Pins: 30-37
 *      This value can be seen using the logic analyzer
*/
void dbgOutputValue(char outVal){
    // Clear port
    // Set port to val
    SYS_PORTS_Clear(PORTS_ID_0, PORT_CHANNEL_E, 0x00FF);
    SYS_PORTS_Set(PORTS_ID_0, PORT_CHANNEL_E, outVal, 0x00FF);
}

/* Purpose:
 *      This function takes in a preset location value and prints it 
 *      to the I/O lines: RD10,RD5,RB11,RB13,RB12,RG8,RA10,RF0
 *                      Pins: 38-45
 *      This value can be seen using the logic analyzer
*/
void dbgOutputLoc(unsigned int outVal){
    // Clear ports
    SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_10);
    SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_5);
    SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_10);
    SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_11);
    SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_12);
    SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13);
    SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_8);
    SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_0);
    // Set pin to val
    if(outVal & 0x0001){
        SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_10);
    }
    else{
        SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_10);
    }
    if(outVal & 0x0002){
        SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_5);
    }
    else{
        SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_5);
    }
    if(outVal & 0x0004){
        SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_11);
    }
    else{
        SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_11);
    }
    if(outVal & 0x0008){
        SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13);
    }
    else{
        SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_13);
    }
    if(outVal & 0x0010){
        SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_12);
    }
    else{
        SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_12);
    }
    if(outVal & 0x0020){
        SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_8);
    }
    else{
        SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_8);
    }
    if(outVal & 0x0040){
        SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_10);
    }
    else{
        SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_10);
    }
    if(outVal & 0x0080){
        SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_0);
    }
    else{
        SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_0);
    }
}

void dbgLocTrigger(bool trigger){
    if(trigger == true){
        SYS_PORTS_PinSet(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_1);
    }
    else{
        SYS_PORTS_PinClear(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_1);
    }
}
/* *****************************************************************************
 End of File
 */
