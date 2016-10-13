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
#include "peripheral/oc/plib_oc.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************

static QueueHandle_t _usartqueue;
#define USARTTYPEOFQUEUE char
#define USARTSIZEOFQUEUE 600

void IntHandlerDrvAdc(void)
{
    dbgOutputLoc(ENTER_ADC_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    int i = 0;
    float adcValToQ = 0.0;
    DRV_ADC_Stop();
    //Read data before clearing interrupt flag
    dbgOutputLoc(ADDING_ADC_VAL_ISR);
    adcValToQ = adcValToQ + PLIB_ADC_ResultGetByIndex(ADC_ID_1, 0);
    dbgOutputLoc(BEFORE_SEND_TO_Q_ISR);
    adc_app_SendValToMsgQFromISR(adcValToQ, &pxHigherPriorityTaskWoken);
    dbgOutputLoc(AFTER_SEND_TO_Q_ISR);
    dbgOutputLoc(LEAVE_ADC_ISR);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    /* Clear ADC Interrupt Flag */
    PLIB_INT_SourceDisable(INT_ID_0, INT_SOURCE_ADC_1);
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_ADC_1);
}

/* This timer is for the ADC to fire every 50 ms */
// Timer 2
void IntHandlerDrvTmrInstance0(void)
{
    //dbgOutputLoc(ENTER_TMR_INSTANCE_0_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    //DRV_ADC_Start();
    //dbgOutputLoc(SET_ADC_FLAG_TMR_INSTANCE_0_ISR);
    //PLIB_INT_SourceFlagSet(INT_ID_0, INT_SOURCE_ADC_1);
    //PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_ADC_1);
    //dbgOutputLoc(LEAVE_TMR_INSTANCE_0_ISR);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
}
//
//int bufCount = 0;
//int ISRcount = 0;
//char buf[22] = "FRLB";
//uint16_t timerCount = 0;


static float desiredTicksR = 5;
static float outputR;
static unsigned short pwmCmdR = 0;
static float integralR = 0;
static float KpR = 7790;// 500;//2900;
static float KiR = 800; // 14;
static int rightSign = 1;

static float desiredTicksL = 5;
static float outputL;
static unsigned short pwmCmdL = 0;
static float integralL = 0;
static float KpL = 8000;// ;// 2700;
static float KiL = 500;// 8;
static int leftSign = 1;
static int time = 0;
static int timeOut = 0;


/* This timer is for the TX to fire every 200 ms */
// Timer 5
void IntHandlerDrvTmrInstance1(void)
{
    dbgOutputLoc(ENTER_TMR_INSTANCE_1_ISR);
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    MessageObj obj;
    obj.Type = SEND_REQUEST;
    dbgOutputLoc(BEFORE_SEND_TO_Q_TMR_INSTANCE_1_ISR);
    // this is necissary cuz this goes of at 20ms now
    if(time % 10 == 0){
        switch(MYMODULE){
            case SEARCHERMOVER:
                obj.Request = SMtoTL;
                // MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                break;
            case TARGETLOCATOR:
                obj.Request = TLtoSM;
                MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                obj.Request = TLtoPF;
                MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                break;
            case PATHFINDER:
                obj.Request = PFtoTL;
                // MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                obj.Request = PFtoTG;
                // MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                break;
            case TARGETGRABBER:
                obj.Request = TGtoPF;
                MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
                break;
        }
    }  
//    if (ISRcount == 16) {
//        MOTOR_CONTROLLER_THREAD_SendToQueueISR(buf[bufCount], &pxHigherPriorityTaskWoken);
//        if (bufCount < strlen(buf)-1) {
//            bufCount++;
//        } else {
//            bufCount = 0;
//        }
//        ISRcount = 0;
//    } else {
//        ISRcount++;
//    }
    
    if(time >= 50){
    
        setDirectionForward();

        int rightCount = PLIB_TMR_Counter16BitGet(TMR_ID_3) * rightSign;
        int leftCount = PLIB_TMR_Counter16BitGet(TMR_ID_4) * leftSign;

        float errorR = desiredTicksR - (float) rightCount;
        integralR = integralR + errorR; // for now assume dt = 1 when really its 200ms
        // derivative = (error - previous_error)/dt
        outputR = KpR* errorR + KiR* integralR;
        if(outputR > 0){
            setDirectionForward();
            rightSign = 1;
        }else{
            setDirectionBack();
            outputR *= -1;
            rightSign = -1;
        }
        if(outputR > 65535){
            outputR = 65535;
        }
        pwmCmdR = (unsigned short) outputR;
//        if(pwmCmdL)

        float errorL = desiredTicksL - (float) leftCount;
        integralL = integralL + errorL; // for now assume dt = 1 when really its 200ms
        // derivative = (error - previous_error)/dt
        outputL = KpL*errorL + KiL*integralL;
        if(outputL > 0){
            setDirectionForward();
            leftSign = 1;
        }else{
            setDirectionBack();
            outputL *= -1;
            leftSign = -1;
        }
        if(outputL > 65535){
            outputL = 65535;
        }
        pwmCmdL = (unsigned short) outputL;
        
        if(time % 3 == 0){
            Tx_Thead_Queue_DataType tx_thread_obj;
            memset(&tx_thread_obj, 0, sizeof(Tx_Thead_Queue_DataType));
            tx_thread_obj.Destination = PATHFINDER;
            //sprintf(tx_thread_obj.Data, " %4d, %4d, %6d, %6d \n", rightCount, leftCount, pwmCmdR, pwmCmdL);
            sprintf(tx_thread_obj.Data, "{\"type\": \"PID\", \"motor\": \"1\", \"vel\": \"%4d\", \"time\": \"%4d\", \"output\": \"%4f\", \"pwm\": \"%4d\"}", rightCount, timeOut, outputR, pwmCmdR);
            BaseType_t *ptr;
            TX_THREAD_SendToQueueISR(tx_thread_obj, ptr);
            sprintf(tx_thread_obj.Data, "{\"type\": \"PID\", \"motor\": \"2\", \"vel\": \"%4d\", \"time\": \"%4d\"}", leftCount, time);
            TX_THREAD_SendToQueueISR(tx_thread_obj, ptr);
        }

        PLIB_OC_PulseWidth16BitSet(OC_ID_1, pwmCmdR);
        PLIB_OC_PulseWidth16BitSet(OC_ID_2, pwmCmdL);

        // Clear encoder counters
        PLIB_TMR_Counter16BitClear(TMR_ID_3);
        PLIB_TMR_Counter16BitClear(TMR_ID_4);

    }else{
        disableMotors();
    }
    timeOut++;
    time++;
    
    dbgOutputLoc(AFTER_SEND_TO_Q_TMR_INSTANCE_1_ISR);
    incrementSystemClock();
    dbgOutputLoc(LEAVE_TMR_INSTANCE_1_ISR);
    portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_5);
}

// Timer 3
void IntHandlerDrvTmrInstance2(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_3);
}

// Timer 4
void IntHandlerDrvTmrInstance3(void)
{
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

