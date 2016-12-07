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
static QueueHandle_t motorQueue;
#define USARTTYPEOFQUEUE char
#define USARTSIZEOFQUEUE 600
#define MOTORTYPEOFQUEUE char
#define MOTORSIZEOFQUEUE 300

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


static float desiredTicksR = 0;
static float outputR;
static unsigned short pwmCmdR = 0;
static float integralR = 0;
static float KpR = 6000;
static float KiR = 10000;
static int rightSign = 1;

static float desiredTicksL = 0;
static float outputL;
static unsigned short pwmCmdL = 0;
static float integralL = 0;
static float KpL = 6000;
static float KiL = 10000;
static int leftSign = 1;
static int time = 0;
static int timeOut = 0;

// for MS3 8000 5000
//         ?    ?
// Working state 
//    8000 500
//    8000 500


//working okay 8000 10000 for both

//now that we have ramps and profiles in place lets tune i
// 10,000 works well, 20,000 too jittery, 15000 still jittery, 12500 little jitter lets see how 10000 was, that feels pretty good
// lets try reducing my kp, its at 8k, lets try 7 felt like more overshoot but left == right seemed more true
// lets try 6k, overshot still got worse but right == left was still suprisingly good
// lets try 3k, right == left was not as good and it felt chunky
// lets see if 4.5k is a good medium -- naah left vs right was not as good lets stick with 6k for now

// i tuned some movement in turning now im coming back to smooth it out if necissary, lets turn back up to 8k
// no difference from 66k guess ill leave it at 6k
// yeah 6 is fine im happy

static int desiredTicks = 0;
static int ticksSoFar = 0;

static int cacheType = 0;
static int cacheDistance = 0;

static bool enable;

void enableTrue(){
    enable = true;
}
void enableFalse(){
    enable = false;
}


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
    if(time % 100 == 0){
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
    if(time % 20 == 0){
        obj.Request = PFtoTG;
        MESSAGE_CONTROLLER_THREAD_SendToQueueISR(obj, &pxHigherPriorityTaskWoken);
    }
    
    if(desiredTicks != 0){
        ticksSoFar += PLIB_TMR_Counter16BitGet(TMR_ID_3);
        PLIB_TMR_Counter16BitClear(TMR_ID_3);
        if(ticksSoFar >= desiredTicks){
            message_in_t buffer;
            buffer.type = update;
            buffer.cacheType = cacheType;
            buffer.cacheDistance = cacheDistance;
            BaseType_t *temp1;
            MOTOR_CONTROLLER_THREAD_SendToQueueISR(buffer, temp1);
            PLIB_OC_PulseWidth16BitSet(OC_ID_1, 0);
            PLIB_OC_PulseWidth16BitSet(OC_ID_2, 0);
            desiredTicks = 0;
        }
    }
    
    char newSP;
    if(time % 10 == 0){
        if( motor_ReadFromQueue(&newSP) == pdTRUE){
            desiredTicksL = (float) newSP;
            desiredTicksR = (float) newSP;
        }else{
            message_in_t buffer;
            buffer.type = update;
            buffer.cacheType = cacheType;
            buffer.cacheDistance = cacheDistance;
            BaseType_t *temp1;
            MOTOR_CONTROLLER_THREAD_SendToQueueISR(buffer, temp1);
            cacheDistance = 0;
        }
    }
    
    // && 0 to temporarily disable motors
    if(time >= 50  && enable == true){// && (desiredTicksL != 0 || desiredTicksR != 0)){
    
        // setDirectionForward();

        int rightCount = PLIB_TMR_Counter16BitGet(TMR_ID_3) * rightSign;
        int leftCount = PLIB_TMR_Counter16BitGet(TMR_ID_4) * leftSign;

        float errorR = desiredTicksR - (float) rightCount;
        integralR = integralR + errorR; // for now assume dt = 1 when really its 200ms
        // derivative = (error - previous_error)/dt
        outputR = KpR* errorR + KiR* integralR;
        if(outputR > 0){
//            SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 0); // right forward
            rightSign = 1;
        }else{
//            SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 1); // right back
              outputR = 0;
//            outputR *= -1;
//            rightSign = -1;
        }
        if(outputR > 65535){
            outputR = 65535;
        }
        pwmCmdR = (unsigned short) outputR;


        float errorL = desiredTicksL - (float) leftCount;
        integralL = integralL + errorL; // for now assume dt = 1 when really its 200ms
        // derivative = (error - previous_error)/dt
        outputL = KpL*errorL + KiL*integralL;
        if(outputL > 0){
//            SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 0); // left forward
            leftSign = 1;
        }else{
              outputL = 0;
//            SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 1); // left back
//            outputL *= -1;
//            leftSign = -1;
        }
        if(outputL > 65535){
            outputL = 65535;
        }
        pwmCmdL = (unsigned short) outputL;
        
        if(time % 1 == 0 && 0){
            Tx_Thead_Queue_DataType tx_thread_obj;
            memset(&tx_thread_obj, 0, sizeof(Tx_Thead_Queue_DataType));
            tx_thread_obj.Destination = TARGETGRABBER;
            //sprintf(tx_thread_obj.Data, " %4d, %4d, %6d, %6d \n", rightCount, leftCount, pwmCmdR, pwmCmdL);
            sprintf(tx_thread_obj.Data, "{\"type\": \"PID\", \"motor\": \"1\", \"vel\": \"%4d\", \"time\": \"%4d\"}", rightCount, time);
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
//        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 0);
//        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 0);
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
    motor_InitializeQueue();
}

void addMotorTask(int type, int distance){
    cacheType = type;
    cacheDistance = distance;
    
    ticksSoFar = 0;
    char peak = 5;
    int length;
    
    if(type == 0 && (distance == 3 || distance == -3)){
        // straight
        if(distance > 0){
            setDirectionForward();
        }else{
            setDirectionBack();
            distance *= -1;
        }
        length = 6;
//        desiredTicks = 250 * distance;
        int i;
        for(i = 0; i <= peak; i++){
            motor_SendToQueue(i);
        }
        for(i = 0; i < length; i++){
            motor_SendToQueue(peak);
        }
        for(i = peak; i >= 0; i--){
            motor_SendToQueue(i);
        }
    }else if (type == 1 && (distance == 90 || distance == -90)){
        // turn
        if(distance > 0){
            setDirectionLeft();
//            desiredTicks = distance * 7.7;
        }else{
            setDirectionRight();
            distance *= -1;
//            desiredTicks = distance * 7.7;
        }
        peak = 4;
        length = 7;
        int i, j;
        int rampLethargy = 2;
        for(i = 0; i <= peak; i++){
            for(j = 0; j < rampLethargy; j++){
                motor_SendToQueue(i);
            }
        }
        for(i = 0; i < length; i++){
            motor_SendToQueue(peak);
        }
        for(i = peak; i >= 0; i--){
            for(j = 0; j < rampLethargy; j++){
                motor_SendToQueue(i);
            }
        }
        // added lethargy, things seemed less stable but more controllable at two then 1 still has really good right == left
        // when time at peak was 9 and 10 we were rotating past 90 degrees
        // 8 is still too long
        // 7 feels spot on but its error still adds up, ill try 6 just to go under, 6 was under
        // lets fo crazy and pump up the peak from 4 to 5
        // im def overshooting which is fine, right == left still good, feels jerky
        // turning our length to zero made us not turn far enough
        // lets try lenth =2, thats a tad beyone 90
        // lets try lenght 1, nvm we are sacrificing our right == left ness when we get rid of length
        // my best guess for th enight is peak 4 len 7 leth 2
        
        
        
        
        // desiredTicks = 9 * distance;
    }else if (type == 2){
        motor_SendToQueue(0);
        motor_SendToQueue(2);
        motor_SendToQueue(2);
        motor_SendToQueue(4);// we need more than two fours but 6 is probably more than enough
        motor_SendToQueue(4);
        motor_SendToQueue(4);
        motor_SendToQueue(4);
        motor_SendToQueue(4);
        motor_SendToQueue(4);
        motor_SendToQueue(2);
        motor_SendToQueue(2);
        motor_SendToQueue(0);
        
    }
    
//    
//    PLIB_OC_PulseWidth16BitSet(OC_ID_1, 65535/2);
//    PLIB_OC_PulseWidth16BitSet(OC_ID_2, 65535/2);
    
}

void increaseSP(){
     motor_SendToQueue((char)desiredTicksL + 1);
//    desiredTicksR++;
//    desiredTicksL++;
}
void decreaseSP(){
    motor_SendToQueue((char)desiredTicksL - 1);
//    desiredTicksR--;
//    desiredTicksL--;
}

void motor_InitializeQueue() {
    motorQueue = xQueueCreate(MOTORSIZEOFQUEUE, sizeof(MOTORTYPEOFQUEUE));
    if(motorQueue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

int motor_ReadFromQueue(void* pvBuffer) {
    BaseType_t *pxHigherPriorityTaskWoken;
    int ret = xQueueReceiveFromISR(motorQueue, pvBuffer, pxHigherPriorityTaskWoken);
    return ret;
}

void motor_SendToQueue(char buffer) {
    xQueueSendToBack(motorQueue, &buffer, portMAX_DELAY);
}

void motor_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendToBackFromISR(motorQueue, &buffer, pxHigherPriorityTaskWoken);
}
  
/*******************************************************************************
 End of File
*/

