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
#include "rx_thread.h"
#include "tx_thread.h"
#include "adc_thread.h"
#include "adc_thread_public.h"
#include "debug.h"
#include "rx_thread_public.h"
#include "message_controller_thread.h"
#include "motor_controller_thread.h"
#include "system_definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************

static QueueHandle_t _usartqueue;
#define USARTTYPEOFQUEUE char
#define USARTSIZEOFQUEUE 600

// ADC interrupt
// Either scans through ADC channels or performs digital pin reads
void IntHandlerDrvAdc(void)
{
    dbgOutputLoc(ENTER_ADC_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    LineObj lineObj;
    
    memset(&lineObj, 0, (size_t) sizeof(LineObj));
    
    int i;
    
    dbgOutputLoc(ADDING_ADC_VAL_ISR);
    
    // Sum ADC sample readings
    if (ANALOG) {
        for (i = 0; i < 16; i += 8) {
            lineObj.IR_0 += DRV_ADC_SamplesRead(i);
            lineObj.IR_1 += DRV_ADC_SamplesRead(i+1);
            lineObj.IR_2 += DRV_ADC_SamplesRead(i+2);
            lineObj.IR_3 += DRV_ADC_SamplesRead(i+3);
            lineObj.IR_4 += DRV_ADC_SamplesRead(i+4);
            lineObj.IR_5 += DRV_ADC_SamplesRead(i+5);
            lineObj.IR_6 += DRV_ADC_SamplesRead(i+6);
            lineObj.IR_7 += DRV_ADC_SamplesRead(i+7);
        }
    }
    // Read digital pin values if ANALOG is not asserted
    else {
        lineObj.IR_0 = SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_F, 13);
        lineObj.IR_1 = SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_F, 12);
        lineObj.IR_2 = SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_F, 5);
        lineObj.IR_3 = SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_F, 4);
        lineObj.IR_4 = SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_D, 15);
        lineObj.IR_5 = SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_D, 14);
        lineObj.IR_6 = SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_A, 15);
        lineObj.IR_7 = SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_A, 14);
    }
    
    dbgOutputLoc(BEFORE_SEND_TO_Q_ISR);

    // Send the line location to the motor_controller thread
    ADC_THREAD_SendToQueueISR(lineObj, &pxHigherPriorityTaskWoken);
    dbgOutputLoc(AFTER_SEND_TO_Q_ISR);
    dbgOutputLoc(LEAVE_ADC_ISR);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    PLIB_INT_SourceDisable(INT_ID_0, INT_SOURCE_ADC_1);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_ADC_1);
}

/* This timer is for PWM, fires every 2 ms */
// Timer 2
void IntHandlerDrvTmrInstance0(void)
{
    dbgOutputLoc(ENTER_TMR_INSTANCE_0_ISR);
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
}

uint16_t timerCount = 0;

/* This timer is for the ADC, PID, and requests - fires every 20 ms */
// Timer 5
void IntHandlerDrvTmrInstance1(void)
{
    dbgOutputLoc(ENTER_TMR_INSTANCE_1_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    MessageObj obj;
    obj.Type = SEND_REQUEST;
    
    // Run PID algorithm
    MOTOR_CONTROLLER_THREAD_CorrectSpeed(timerCount);
    
    // Enable ADC auto sampling every 200 ms
    if (timerCount % 10 == 0) {
        if(!PLIB_INT_SourceIsEnabled(INT_ID_0, INT_SOURCE_ADC_1)){
            PLIB_ADC_SampleAutoStartEnable(ADC_ID_1);
            PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_ADC_1);
        }
    }
    
    // Issue requests every 300 ms
    if (timerCount % 15 == 0) {
        dbgOutputLoc(BEFORE_SEND_TO_Q_TMR_INSTANCE_1_ISR);
        switch(MYMODULE){
            case SEARCHERMOVER:
                obj.Request = SMtoTL;
                MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                break;
            case TARGETLOCATOR:
                obj.Request = TLtoSM;
                MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                obj.Request = TLtoPF;
                MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                break;
            case PATHFINDER:
                obj.Request = PFtoTL;
                MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                obj.Request = PFtoTG;
                MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                break;
            case TARGETGRABBER:
                obj.Request = TGtoPF;
                MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                break;
        }
    }
    
    dbgOutputLoc(AFTER_SEND_TO_Q_TMR_INSTANCE_1_ISR);
    timerCount++;
    incrementSystemClock();
    dbgOutputLoc(LEAVE_TMR_INSTANCE_1_ISR);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_5);
}

// Timer 3 - Motor 1 - right encoder
void IntHandlerDrvTmrInstance2(void)
{
    // Increment right encoder count
    MOTOR_CONTROLLER_THREAD_IncrementRight();
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_3);
}

// Timer 4 - Motor 2 - left encoder
void IntHandlerDrvTmrInstance3(void)
{
    // Increment right encoder count
    MOTOR_CONTROLLER_THREAD_IncrementLeft();
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

