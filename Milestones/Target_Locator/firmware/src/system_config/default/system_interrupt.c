/*******************************************************************************
 System Interrupts File

  File Name:
    system_interrupt.c

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the
    interrupt sub-system.

  Summary:
    This file contains source code for the interrupt vector functions in the
    system.

  Description:
    This file contains source code for the interrupt vector functions in the
    system.  It implements the system and part specific vector "stub" functions
    from which the individual "Tasks" functions are called for any modules
    executing interrupt-driven in the MPLAB Harmony system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    interrupt-driven in the system.  These handles are passed into the individual
    module "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

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

#include <xc.h>
#include <sys/attribs.h>
#include "debug.h"
#include "tx_thread.h"
#include "rx_thread.h"
#include "rx_thread_public.h"
#include "message_controller_thread.h"
#include "system_definitions.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************

static QueueHandle_t _usartqueue;
#define USARTTYPEOFQUEUE char
#define USARTSIZEOFQUEUE 400


void IntHandlerDrvAdc(void)
{
    dbgOutputLoc(ENTER_ADC_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    TL_Queue_t obj;
    obj.type = SENSORADC;
    memset(&obj, 0, sizeof(TL_Queue_t));
    
    int i = 0;
    dbgOutputLoc(ADDING_ADC_VAL_ISR);
    for(i; i < 5; i=i+5) {
        obj.contents.sensors.IRSensors.leftFBSensor += DRV_ADC_SamplesRead(i);
        obj.contents.sensors.IRSensors.middleFBSensor += DRV_ADC_SamplesRead(i + 1);
        obj.contents.sensors.IRSensors.rightFBSensor += DRV_ADC_SamplesRead(i + 2);
        obj.contents.sensors.IRSensors.leftFTSensor += DRV_ADC_SamplesRead(i + 3);
        obj.contents.sensors.IRSensors.rightFTSensor += DRV_ADC_SamplesRead(i + 4);
    }
    dbgOutputLoc(BEFORE_SEND_TO_Q_ISR);
    SENSOR_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
    dbgOutputLoc(AFTER_SEND_TO_Q_ISR);
    dbgOutputLoc(LEAVE_ADC_ISR);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    PLIB_INT_SourceDisable(INT_ID_0, INT_SOURCE_ADC_1);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_ADC_1);
}


/* This timer is to fire the ADC */
void IntHandlerDrvTmrInstance0(void)
{
    dbgOutputLoc(ENTER_TMR_INSTANCE_0_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    if(PLIB_INT_SourceIsEnabled(INT_ID_0, INT_SOURCE_ADC_1) != true){
        PLIB_ADC_SampleAutoStartEnable(ADC_ID_1);
        PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_ADC_1);
        portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
        PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
    }
    else {
        portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
        PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
    }
}

/* This timer is to send request every ~200ms */
void IntHandlerDrvTmrInstance1(void)
{
    dbgOutputLoc(ENTER_TMR_INSTANCE_1_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    MessageObj obj;
    obj.type = SEND_REQUEST;
    dbgOutputLoc(BEFORE_SEND_TO_Q_TMR_INSTANCE_1_ISR);
    switch(MYMODULE){
        case SEARCHERMOVER:
            obj.message.Request = SMtoTL;
            MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
            break;
        case TARGETLOCATOR:
            obj.message.Request = TLtoSM;
            MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
            obj.message.Request = TLtoPF;
//            MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
            break;
        case PATHFINDER:
            obj.message.Request = PFtoTL;
            MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
            obj.message.Request = PFtoTG;
            MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
            break;
        case TARGETGRABBER:
            obj.message.Request = TGtoPF;
            MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
            break;
    }
    dbgOutputLoc(AFTER_SEND_TO_Q_TMR_INSTANCE_1_ISR);
    incrementSystemClock();
    dbgOutputLoc(LEAVE_TMR_INSTANCE_1_ISR);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_4);
}


void Usart0_InitializeQueue() {
    _usartqueue = xQueueCreate(USARTSIZEOFQUEUE, sizeof(USARTTYPEOFQUEUE));
    if(_usartqueue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

int Usart0_ReadFromQueue(void* pvBuffer, BaseType_t *pxHigherPriorityTaskWoken) {
    dbgOutputLoc(BEFORE_RECEIVE_FR_QUEUE_USART0_ISR);
    int ret = xQueueReceiveFromISR(_usartqueue, pvBuffer, pxHigherPriorityTaskWoken);
    dbgOutputLoc(AFTER_RECEIVE_FR_QUEUE_USART0_ISR);
    return ret;
}

void Usart0_SendToQueue(char buffer) {
    xQueueSendToBack(_usartqueue, &buffer, portMAX_DELAY);
}

void Usart0_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendToBackFromISR(_usartqueue, &buffer, pxHigherPriorityTaskWoken);
}

void IntHandlerDrvUsartInstance0(void)
{
    dbgOutputLoc(ENTER_USART0_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    if (SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_RECEIVE) && !DRV_USART0_ReceiverBufferIsEmpty())
    {
        dbgOutputLoc(BEFORE_SEND_TO_QUEUE_USART0_ISR);
        RX_THREAD_SendToQueueISR(DRV_USART0_ReadByte(), &pxHigherPriorityTaskWoken); // read received byte
        dbgOutputLoc(AFTER_SEND_TO_QUEUE_USART0_ISR);
    }
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_TRANSMIT) && !(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART0_TransferStatus()) )
    {
        char buf;
        dbgOutputLoc(BEFORE_RECEIVE_FR_QUEUE_USART0_ISR);
        if(Usart0_ReadFromQueue(&buf, &pxHigherPriorityTaskWoken)) {
            DRV_USART0_WriteByte(buf);
        }
        else {
            SYS_INT_SourceDisable(INT_SOURCE_USART_1_TRANSMIT);
        }
        dbgOutputLoc(AFTER_RECEIVE_FR_QUEUE_USART0_ISR);
    }
    //DRV_USART_TasksTransmit(sysObj.drvUsart0);
    DRV_USART_TasksReceive(sysObj.drvUsart0);
    DRV_USART_TasksError(sysObj.drvUsart0);
    dbgOutputLoc(LEAVE_USART0_ISR);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
}

void InitializeISRQueues() {
    Usart0_InitializeQueue();
}

 

 

 

 

 

 
  
/*******************************************************************************
 End of File
*/

