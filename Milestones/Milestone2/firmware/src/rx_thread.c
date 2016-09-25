/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    rx_thread.c

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

#include "rx_thread.h"
#include "rx_thread_public.h"
#include "debug.h"
#include "communication/messages.h"
#include "message_controller_thread.h"
#include <stdbool.h>
static QueueHandle_t _queue;

#define TYPEOFQUEUE char
#define SIZEOFQUEUE 10

/*******************************************************************************
  Function:
    void RX_THREAD_Initialize ( void )

  Remarks:
    See prototype in rx_thread.h.
 */

void RX_THREAD_Initialize ( void )
{
    RX_THREAD_InitializeQueue();
    SYS_INT_SourceEnable(INT_SOURCE_USART_1_RECEIVE);
}


/******************************************************************************
  Function:
    void RX_THREAD_Tasks ( void )

  Remarks:
    See prototype in rx_thread.h.
 */

void RX_THREAD_Tasks ( void )
{
    dbgOutputLoc(ENTER_RXTHREAD);
    dbgOutputLoc(BEFORE_WHILELOOP_RXTHREAD);
    MessageObj obj;
    obj.Type = EXTERNAL_REQUEST_RESPONSE;
    char c;
    while(1){
        dbgOutputLoc(BEFORE_RECEIVE_FR_QUEUE_RXTHREAD);
        RX_THREAD_ReadFromQueue(&c);
        dbgOutputLoc(AFTER_RECEIVE_FR_QUEUE_RXTHREAD);
        bool isComplete = ParseMessage(c, obj.External.Data, &obj.External.Source, &obj.External.MessageCount, &obj.External.Error);
        if(isComplete) {
            /*Since we returned true we assume the message is valid*/
            MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
        }
        else if(obj.External.Error) {
            MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
        }
    }
    dbgOutputLoc(LEAVE_RXTHREAD);
}

void RX_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
    if(_queue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

void RX_THREAD_ReadFromQueue(void* pvBuffer) {
    dbgOutputLoc(BEFORE_RECEIVE_FR_QUEUE_READFROMQUEUE_RXTHREAD);
    xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
    dbgOutputLoc(AFTER_RECEIVE_FR_QUEUE_READFROMQUEUE_RXTHREAD);
}

void RX_THREAD_SendToQueue(char buffer) {
    xQueueSendToBack(_queue, &buffer, portMAX_DELAY);
}

void RX_THREAD_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendToBackFromISR(_queue, &buffer, pxHigherPriorityTaskWoken);
}


/*******************************************************************************
 End of File
 */
