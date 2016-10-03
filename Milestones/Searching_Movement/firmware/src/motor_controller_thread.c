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

#include "motor_controller_thread.h"

static QueueHandle_t _queue;

#define TYPEOFQUEUE char
#define SIZEOFQUEUE 200

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
    /* Place the App state machine in its initial state. */
    motor_controller_threadData.state = MOTOR_CONTROLLER_THREAD_STATE_INIT;

    MOTOR_CONTROLLER_THREAD_InitializeQueue();

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void MOTOR_CONTROLLER_THREAD_Tasks ( void )

  Remarks:
    See prototype in motor_controller_thread.h.
 */

void MOTOR_CONTROLLER_THREAD_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( motor_controller_threadData.state )
    {
        /* Application's initial state. */
        case MOTOR_CONTROLLER_THREAD_STATE_INIT:
        {
            bool appInitialized = true;
            if (appInitialized)
            {
                motor_controller_threadData.state = MOTOR_CONTROLLER_THREAD_STATE_SERVICE_TASKS;
            }
            break;
        }
        case MOTOR_CONTROLLER_THREAD_STATE_SERVICE_TASKS:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

void MOTOR_CONTROLLER_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
}

void MOTOR_CONTROLLER_THREAD_ReadFromQueue(MessageObj* pvBuffer) {
    xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
}

void MOTOR_CONTROLLER_THREAD_SendToQueue(MessageObj buffer) {
    xQueueSend(_queue, &buffer, portMAX_DELAY);
}

void MOTOR_CONTROLLER_THREAD_SendToQueueISR(MessageObj buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendFromISR(_queue, &buffer, pxHigherPriorityTaskWoken);
}

/*******************************************************************************
 End of File
 */
