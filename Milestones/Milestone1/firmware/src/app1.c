/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app1.c

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

#include "app1.h"
#include <queue.h>
#include "debug.h"
#include "app1_public.h"

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP1_Initialize ( void )

  Remarks:
    See prototype in app1.h.
 */

void APP1_Initialize ( void )
{
    APP1_InitializeQueue();
}


/******************************************************************************
  Function:
    void APP1_Tasks ( void )

  Remarks:
    See prototype in app1.h.
 */
void APP1_Tasks ( void )
{
    dbgOutputLoc(APP1_ENTER_TASK);
    DRV_TMR0_Start();
    dbgOutputLoc(APP1_BEFORE_WHILELOOP);
    while(1) {
        char buffer;
        dbgOutputBlock(APP1_ReadFromQueue(&buffer));
        dbgOutputVal(buffer);
    }
}


/*******************************************************************************
 Queue(s)
*/
QueueHandle_t _queue;

/**
 * [APP1_InitializeQueue Initializes a queue for this task]
 */
void APP1_InitializeQueue() {
    _queue = xQueueCreate(10, sizeof(char));
    if(_queue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

/**
 * [APP1_ReadFromQueue Read from the queue]
 * @param  pvBuffer [a buffer that is filled with the value at the front of the queue]
 * @return          [error code; 0 if success, 1 otherwise]
 */
int APP1_ReadFromQueue(void* pvBuffer) {
    dbgOutputLoc(APP1_BEFORE_RECEIVE_FR_QUEUE);
    int ret = xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
    dbgOutputLoc(APP1_AFTER_RECEIVE_FR_QUEUE);
    return ret;
}

/**
 * [APP1_SendToQueue Function to send data to APP1's queue]
 * @param  buffer [value to send to the queue]
 * @return        [error code; 1 if success, 0 otherwise]
 */
int APP1_SendToQueue(char buffer) {
    return xQueueSendToBack(_queue, &buffer, portMAX_DELAY);
}

/**
 * [APP1_SendToQueueISR Function to send data to APP1's queue from an ISR]
 * @param  buffer [value to send to the queue]
 * @return        [error code; 1 if success, 0 otherwise]
 */
int APP1_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    return xQueueSendToBackFromISR(_queue, &buffer, pxHigherPriorityTaskWoken);
}
/*******************************************************************************
 End of File
 */
