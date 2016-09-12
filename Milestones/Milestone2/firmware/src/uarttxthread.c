/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    uarttxthread.c

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

#include "uarttxthread.h"
#include "uarttxthread_public.h"
#include "system_interrupt_public.h"
#include "debug.h"
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

QueueHandle_t _queue;

#define SIZEOFQUEUE 10
#define TYPEOFQUEUE char
/*******************************************************************************
  Function:
    void UARTTXTHREAD_Initialize ( void )

  Remarks:
    See prototype in uarttxthread.h.
 */

void UARTTXTHREAD_Initialize ( void )
{
    UARTTXTHREAD_InitializeQueue();
    /*Send a trash character to allow the ISR to fire initially*/
    DRV_USART0_WriteByte(NULL);
}

/******************************************************************************
  Function:
    void UARTTXTHREAD_Tasks ( void )

  Remarks:
    See prototype in uarttxthread.h.
 */

const char test[] = "test";
int x = 0;
void UARTTXTHREAD_Tasks ( void )
{
    dbgOutputLoc(UARTRXTHREAD_ENTER_TASK);
    dbgOutputLoc(UARTRXTHREAD_BEFORE_WHILELOOP);
    while(1){
        dbgOutputLoc(UARTRXTHREAD_BEFORE_SEND_TO_QUEUE);
        Usart0_SendToQueue(test[x]);
        dbgOutputLoc(UARTRXTHREAD_AFTER_SEND_TO_QUEUE);
        if(x == 3) {
            x = 0;
        }
        else {
            x++;
        }
    }
}

void UARTTXTHREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
    if(_queue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

void UARTTXTHREAD_ReadFromQueue(void* pvBuffer) {
    dbgOutputLoc(UARTTXTHREAD_BEFORE_RECEIVE_FR_QUEUE);
    dbgOutputBlock(xQueueReceive(_queue, pvBuffer, portMAX_DELAY));
    dbgOutputLoc(UARTTXTHREAD_AFTER_RECEIVE_FR_QUEUE);
}

void UARTTXTHREAD_SendToQueue(char buffer) {
    dbgOutputBlock(xQueueSendToBack(_queue, &buffer, portMAX_DELAY));
}

void UARTTXTHREAD_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    dbgOutputBlockISR(xQueueSendToBackFromISR(_queue, &buffer, pxHigherPriorityTaskWoken));
}

/*******************************************************************************
 End of File
 */
