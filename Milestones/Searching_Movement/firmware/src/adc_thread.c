/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    adc_thread.c

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

#include "adc_thread.h"
#include "adc_thread_public.h"
#include "system_config.h"
#include "system_definitions.h"
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
static QueueHandle_t _queue;

#define SIZEOFQUEUE 10
#define TYPEOFQUEUE LineObj


/*******************************************************************************
  Function:
    void ADC_THREAD_Initialize ( void )

  Remarks:
    See prototype in adc_thread.h.
 */

void ADC_THREAD_Initialize ( void )
{
    ADC_THREAD_InitializeQueue();
    DRV_TMR0_Start();
    DRV_ADC_Open();
    DRV_ADC_Start();
}

/******************************************************************************
  Function:
    void ADC_THREAD_Tasks ( void )

  Remarks:
    See prototype in adc_thread.h.
 */

 void ADC_THREAD_Tasks ( void )
 {
    dbgOutputLoc(ENTER_TASK_ADC_THREAD);
    MessageObj messageObj;
    LineObj lineObj;
    MotorObj motorObj;

    dbgOutputLoc(BEFORE_WHILE_ADC_THREAD);
    while(1){
        memset(&messageObj, 0, sizeof(MessageObj));
        memset(&lineObj, 0, sizeof(LineObj));
        memset(&motorObj, 0, sizeof(MotorObj));
        messageObj.Type = UPDATE;
        messageObj.Update.Type = LINELOCATION;
        
        dbgOutputLoc(BEFORE_RECEIVE_FROM_Q_ADC_THREAD);
        ADC_THREAD_ReadFromQueue(&lineObj);
        if (ANALOG) {
//            averageData(&lineObj);
        }
        messageObj.Update.Data.lineLocation = lineObj;
        // Sending to message controller queue for an update
        MESSAGE_CONTROLLER_THREAD_SendToQueue(messageObj);
        dbgOutputLoc(AFTER_RECEIVE_FROM_Q_ADC_THREAD);
    }
 }

 /* This averages the data coming from the line sensor*/
 void averageData(LineObj* lineObj) {
     (*lineObj).IR_0 = ((*lineObj).IR_0/2.0);
     (*lineObj).IR_1 = ((*lineObj).IR_1/2.0);
     (*lineObj).IR_2 = ((*lineObj).IR_2/2.0);
     (*lineObj).IR_3 = ((*lineObj).IR_3/2.0);
     (*lineObj).IR_4 = ((*lineObj).IR_4/2.0);
     (*lineObj).IR_5 = ((*lineObj).IR_5/2.0);
     (*lineObj).IR_6 = ((*lineObj).IR_6/2.0);
     (*lineObj).IR_7 = ((*lineObj).IR_7/2.0);
 }
 
void ADC_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
    if(_queue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

int ADC_THREAD_ReadFromQueue(LineObj* lineObj) {
    return xQueueReceive(_queue, lineObj, portMAX_DELAY);
}

/* Sends value from ADC to ADC_THREAD Queue */
int ADC_THREAD_SendToQueue(LineObj lineObj){
    return xQueueSend(_queue, &lineObj, portMAX_DELAY);
}

/* Sends value from ADC ISR to ADC_THREAD Queue */
int ADC_THREAD_SendToQueueISR(LineObj lineObj, BaseType_t *pxHigherPriorityTaskWoken){
    return xQueueSendFromISR(_queue, &lineObj, pxHigherPriorityTaskWoken);
}

/*******************************************************************************
 End of File
 */
