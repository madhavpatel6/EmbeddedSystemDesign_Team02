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
#include "computationthread.h"
#include "grabberthread.h"
#include "debug.h"
#include "uartrxthread.h"
#include "uarttxthread.h"
#include "uarttxthread_public.h"
#include "uartrxthread_public.h"
#include "system_definitions.h"
#include "system_interrupt_public.h"
// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************

QueueHandle_t _usartqueue;
#define USARTTYPEOFQUEUE char
#define USARTSIZEOFQUEUE 600

void IntHandlerDrvAdc(void)
{
    int i = 0;
    float IR0_adcVal = 0.0;
    float IR1_adcVal = 0.0;
    float IR2_adcVal = 0.0;
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    dbgOutputLoc(ENTER_ADC_ISR);
    //Read data before clearing interrupt flag
    dbgOutputLoc(ADDING_VAL_ADC_ISR);
    for(i; i < 16; i+=3) {
        IR0_adcVal = IR0_adcVal + DRV_ADC_SamplesRead(i);
        IR1_adcVal = IR1_adcVal + DRV_ADC_SamplesRead(i+1);
        IR2_adcVal = IR2_adcVal + DRV_ADC_SamplesRead(i+2);
    }
    
    dbgOutputLoc(BEFORE_CONVERT_IR0_TO_CM_ADC_ISR);
    convertTocm(&IR0_adcVal);
    dbgOutputLoc(AFTER_CONVERT_IR0_TO_CM_ADC_ISR);
    
    dbgOutputLoc(BEFORE_CONVERT_IR1_TO_CM_ADC_ISR);
    convertTocm(&IR1_adcVal);
    dbgOutputLoc(AFTER_CONVERT_IR1_TO_CM_ADC_ISR);
    
    dbgOutputLoc(BEFORE_CONVERT_IR2_TO_CM_ADC_ISR);
    convertTocm(&IR2_adcVal);
    dbgOutputLoc(AFTER_CONVERT_IR2_TO_CM_ADC_ISR);
    
    dbgOutputLoc(BEFORE_SEND_IR0_TO_Q_ADC_ISR);
    computationthread_SendValToSensorQFromISR(IR0_adcVal, &pxHigherPriorityTaskWoken);
    dbgOutputLoc(AFTER_SEND_IR0_TO_Q_ADC_ISR);
    
    dbgOutputLoc(BEFORE_SEND_IR1_TO_Q_ADC_ISR);
    computationthread_SendValToSensorQFromISR(IR1_adcVal, &pxHigherPriorityTaskWoken);
    dbgOutputLoc(AFTER_SEND_IR1_TO_Q_ADC_ISR);
    
    dbgOutputLoc(BEFORE_SEND_IR2_TO_Q_ADC_ISR);
    computationthread_SendValToSensorQFromISR(IR2_adcVal, &pxHigherPriorityTaskWoken);
    dbgOutputLoc(AFTER_SEND_IR2_TO_Q_ADC_ISR);
    
    dbgOutputLoc(LEAVE_ADC_ISR);
    PLIB_ADC_SampleAutoStartEnable(ADC_ID_1);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    /* Clear ADC Interrupt Flag */
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_ADC_1);
}

void Usart0_InitializeQueue() {
    _usartqueue = xQueueCreate(USARTSIZEOFQUEUE, sizeof(USARTTYPEOFQUEUE));
    if(_usartqueue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

int Usart0_ReadFromQueue(void* pvBuffer, BaseType_t *pxHigherPriorityTaskWoken) {
    dbgOutputLoc(USART0_BEFORE_RECEIVE_FR_QUEUE);
    int ret = xQueueReceiveFromISR(_usartqueue, pvBuffer, pxHigherPriorityTaskWoken);
    dbgOutputLoc(USART0_AFTER_RECEIVE_FR_QUEUE);
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
    dbgOutputLoc(USART0_ENTER_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    if (SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_RECEIVE) && !DRV_USART0_ReceiverBufferIsEmpty())
    {
        dbgOutputLoc(USART0_BEFORE_SEND_TO_QUEUE);
//        UARTRXTHREAD_SendToQueueISR(DRV_USART0_ReadByte(), &pxHigherPriorityTaskWoken); // read received byte
        dbgOutputLoc(USART0_AFTER_SEND_TO_QUEUE);
    }
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_TRANSMIT) && !(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART0_TransferStatus()) )
    {
        char buf;
        dbgOutputLoc(USART0_BEFORE_RECEIVE_FR_QUEUE);
        if(Usart0_ReadFromQueue(&buf, &pxHigherPriorityTaskWoken)) {
            DRV_USART0_WriteByte(buf);
        }
        else {
            SYS_INT_SourceDisable(INT_SOURCE_USART_1_TRANSMIT);
        }
        dbgOutputLoc(USART0_AFTER_RECEIVE_FR_QUEUE);
    }
    //DRV_USART_TasksTransmit(sysObj.drvUsart0);
    DRV_USART_TasksReceive(sysObj.drvUsart0);
    DRV_USART_TasksError(sysObj.drvUsart0);
    dbgOutputLoc(USART0_LEAVE_ISR);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
}

void InitializeISRQueues() {
    Usart0_InitializeQueue();
}

 

 

 

 

 

 
  
/*******************************************************************************
 End of File
*/
