/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    motor_controller_thread.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "debug.h"
#include "motor_controller_thread.h"
#include "motor_controller_thread_public.h"
#include "math.h"

static QueueHandle_t _queue;

#define TYPEOFQUEUE char
#define SIZEOFQUEUE 48

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

MOTOR_CONTROLLER_THREAD_DATA motor_controller_threadData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void MOTOR_CONTROLLER_THREAD_Initialize ( void )

  Remarks:
    See prototype in motor_controller_thread.h.
 */

void MOTOR_CONTROLLER_THREAD_Initialize ( void )
{
    MOTOR_CONTROLLER_THREAD_InitializeQueue();
    DRV_TMR2_Start();
    DRV_TMR3_Start();
    motor_controller_threadData.state = MOTOR_CONTROLLER_THREAD_STATE_INIT;
}

/******************************************************************************
  Function:
    void MOTOR_CONTROLLER_THREAD_Tasks ( void )

  Remarks:
    See prototype in motor_controller_thread.h.
 */

void MOTOR_CONTROLLER_THREAD_Tasks ( void )
{
    char c;
    MessageObj obj;
    int leftCount = 0;
    int rightCount = 0;
    float x = 0;
    float y = 0;
    float orientation = 0;
    float ticksPerCm = 23.65;
    float circumference = 40.84;
    int leftSign = 1;
    int rightSign = 1;
    int distance = 0;
    
    obj.Type = UPDATE;
    obj.Update.Type = POSITION;
    
    while(1) {
        /* Check the application's current state. */
        switch ( motor_controller_threadData.state )
        {
            /* Application's initial state. */
            case MOTOR_CONTROLLER_THREAD_STATE_INIT:
            {
                MOTOR_CONTROLLER_THREAD_OFF();
                MOTOR_CONTROLLER_THREAD_FORWARD();
                motor_controller_threadData.state = MOTOR_CONTROLLER_THREAD_STATE_SERVICE_TASKS;
                break;
            }
            case MOTOR_CONTROLLER_THREAD_STATE_SERVICE_TASKS:
            {
                // Read direction of travel from queue
                dbgOutputLoc(BEFORE_RECEIVE_FR_QUEUE_MOTORCONTROLLERTHREAD);
                MOTOR_CONTROLLER_THREAD_ReadFromQueue(&c);
                dbgOutputLoc(AFTER_RECEIVE_FR_QUEUE_MOTORCONTROLLERTHREAD);
                
                switch(c) {
                    case 'F': {
                        MOTOR_CONTROLLER_THREAD_OFF();
                        MOTOR_CONTROLLER_THREAD_FORWARD();
                        MOTOR_CONTROLLER_THREAD_ON();
                        leftSign = 1;
                        rightSign = 1;
                        break;
                    }
                    case 'B': {
                        MOTOR_CONTROLLER_THREAD_OFF();
                        MOTOR_CONTROLLER_THREAD_REVERSE();
                        MOTOR_CONTROLLER_THREAD_ON();
                        leftSign = -1;
                        rightSign = -1;
                        break;
                    }
                    case 'L': {
                        MOTOR_CONTROLLER_THREAD_OFF();
                        MOTOR_CONTROLLER_THREAD_LEFT();
                        MOTOR_CONTROLLER_THREAD_ON();
                        leftSign = -1;
                        rightSign = 1;
                        break;
                    }
                    case 'R': {
                        MOTOR_CONTROLLER_THREAD_OFF();
                        MOTOR_CONTROLLER_THREAD_RIGHT();
                        MOTOR_CONTROLLER_THREAD_ON();
                        leftSign = 1;
                        rightSign = -1;
                        break;
                    }
                    case 'S': {
                        MOTOR_CONTROLLER_THREAD_OFF();
                        break;
                    }
                    default: {
                        break;
                    }
                }
                
                // Get encoder values from each motor
                leftCount = PLIB_TMR_Counter16BitGet(TMR_ID_3) * leftSign;
                rightCount = PLIB_TMR_Counter16BitGet(TMR_ID_4) * rightSign;
                
                // Clear encoder counters
                PLIB_TMR_Counter16BitClear(TMR_ID_3);
                PLIB_TMR_Counter16BitClear(TMR_ID_4);
                
                // Update position of rover
                orientation += (((((leftCount-rightCount)/2.0)/ticksPerCm)/circumference)*360);
                distance = (((leftCount+rightCount)/2.0)/ticksPerCm);
                x = distance*cos(orientation);
                y = distance*sin(orientation);
                
                obj.Update.Data.location.x = x;
                obj.Update.Data.location.y = y;
                obj.Update.Data.orientation = orientation;
                
                MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void MOTOR_CONTROLLER_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
    if(_queue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

void MOTOR_CONTROLLER_THREAD_ReadFromQueue(char* pvBuffer) {
    xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
}

void MOTOR_CONTROLLER_THREAD_SendToQueue(char buffer) {
    xQueueSend(_queue, &buffer, portMAX_DELAY);
}

void MOTOR_CONTROLLER_THREAD_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendFromISR(_queue, &buffer, pxHigherPriorityTaskWoken);
}

void MOTOR_CONTROLLER_THREAD_ON( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, 0, 1);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, 1, 1);
}

void MOTOR_CONTROLLER_THREAD_OFF( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, 0, 0);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, 1, 0);
}

void MOTOR_CONTROLLER_THREAD_FORWARD( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 0);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 1);
}

void MOTOR_CONTROLLER_THREAD_REVERSE( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 1);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 0);
}

void MOTOR_CONTROLLER_THREAD_LEFT( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 0);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 0);
}

void MOTOR_CONTROLLER_THREAD_RIGHT( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 1);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 0);
}

/*******************************************************************************
 End of File
 */
