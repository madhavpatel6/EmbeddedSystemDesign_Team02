/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    tx_thread.c

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

#include "tx_thread.h"
#include "tx_thread_public.h"
#include "system_interrupt_public.h"
#include "debug.h"
#include "communication/messages.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

static QueueHandle_t _queue;

#define SIZEOFQUEUE 10
#define TYPEOFQUEUE char[MAXMESSAGESIZE]


/*******************************************************************************
  Function:
    void TX_THREAD_Initialize ( void )

  Remarks:
    See prototype in tx_thread.h.
 */

void TX_THREAD_Initialize ( void )
{
    TX_THREAD_InitializeQueue();
}


/******************************************************************************
  Function:
    void TX_THREAD_Tasks ( void )

  Remarks:
    See prototype in tx_thread.h.
 */

void TX_THREAD_Tasks ( void )
{
    dbgOutputLoc(ENTER_TXTHREAD);
    char messageData[MAXMESSAGESIZE];
    char packedMessage[MAXMESSAGESIZE];
    dbgOutputLoc(BEFORE_WHILELOOP_RXTHREAD);
    while(1){
        //receive from our local queue
        TX_THREAD_ReadFromQueue(messageData);
        
        //ConvertCommObjectToString(readObj, messageData);
        
        int length = CreateMessage(packedMessage, messageData, PATHFINDER);
        
        int i = 0;
        for(i = 0; i < length; i++) {
            Usart0_SendToQueue(packedMessage[i]);
        }
        dbgOutputLoc(BEFORE_SEND_TO_QUEUE_RXTHREAD);
   
        dbgOutputLoc(AFTER_SEND_TO_QUEUE_RXTHREAD);
        SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
        //after we finish sending packet; disable tx isr
    }
}

void TX_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
    if(_queue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

void TX_THREAD_ReadFromQueue(char pvBuffer[]) {
    dbgOutputLoc(BEFORE_RECEIVE_FR_QUEUE_TXTHREAD);
    xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
    dbgOutputLoc(AFTER_RECEIVE_FR_QUEUE_TXTHREAD);
}

void TX_THREAD_SendToQueue(char buffer[]) {
    xQueueSendToBack(_queue, buffer, portMAX_DELAY);
}

void TX_THREAD_SendToQueueISR(char buffer[], BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendToBackFromISR(_queue, buffer, pxHigherPriorityTaskWoken);
}

 

/*******************************************************************************
 End of File
 */
