/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    computation_thread.c

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

#include "computation_thread.h"
#include "system_config.h"
#include "system_definitions.h"
#include "debug.h"
#include <math.h>

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

#define SIZEOFQUEUE 200
#define TYPEOFQUEUE TargetAlignment_Type

COMPUTATION_THREAD_DATA computation_threadData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

/* Following functions are for milestone1 */
QueueHandle_t createComputationQ(){
    //8 b/c "team 2" //10 is size of Q
    return xQueueCreate(SIZEOFQUEUE,sizeof(TYPEOFQUEUE));
}

/* Sends values from message controller thread to computation thread Queue */
int computation_thread_SendValToMsgQ(TargetAlignment_Type pathFinderResponse){
    return xQueueSend(_queue, &pathFinderResponse, portMAX_DELAY);
}

/* Sends value from adc ISR to adc_app Queue */
int computation_thread_SendValToMsgQFromISR(TargetAlignment_Type adcVal, BaseType_t *pxHigherPriorityTaskWoken){
    return xQueueSendFromISR(_queue, &adcVal, pxHigherPriorityTaskWoken);
}

/* This converts the ultrasonic sensor values to cm for 10cm - 80cm */
void convertTocmUltra(float *ultraDigitalVal){
    float tempAdcVal = 0.0;
    float tempAdcVoltageConv = 0.0;
    tempAdcVal = ((*ultraDigitalVal)/(1.0))*(1.0);
    tempAdcVoltageConv = (tempAdcVal*5.0)/(1024.0);
    *ultraDigitalVal = (tempAdcVoltageConv / 0.009766) * (2.54);
}

/* This converts the IR sensor values to cm for ~3.42cm -~ 20cm       */
/* This uses Chris' equation :             1                          */
/*                            ---------------------------------       */
/*                            (AnalogVoltage - 0.2243)/(9.6762)       */
/* Analog voltage is found by converting the digital value to analog  */
/* Inputs a uint32_t digital value from ADC                           */
/* Outputs float converted digital value to cm                        */
float convertTocmIR(uint32_t irDigitalVal){
    if((irDigitalVal == 0.0) || (irDigitalVal < 0.0)){
        irDigitalVal = 0.0;
    }
    else{
        float tempIRAdcVal;
        float tempIRAdcVoltageConv;
        float cmIRVal;
        float avgIRAdcVal;
        float constantVal;
        constantVal = (3.3)/(1024.0);
        avgIRAdcVal = ((1.0*irDigitalVal)/(5.0)); // This is because of 3 sensors and 15 samples
        tempIRAdcVoltageConv = (avgIRAdcVal)*(constantVal);
        tempIRAdcVal = (tempIRAdcVoltageConv-0.2243)/(9.6762);
        cmIRVal = ((1.0)/tempIRAdcVal);
        return cmIRVal;
    }
}

/* Determines if the converted cm value is reasonable                   */
/* Checks to make sure the value is within reading via Chris equation   */
/* and datasheet.  Also makes sure the distance is within 6" range      */
/* Inputs: float converted cm value                                     */
/* Outputs: true or false based on cm value                             */
bool cmValChecker(float cmVal){
    if((cmVal < 3.5) || (cmVal > 15.24)){
        return false;
    }
    else{
        return true;
    }
}

/* This subroutine handles the target alignment data             */
/* Which pertains to scanning AN0, AN1, AN2 on the ADC           */
/* The ADC is in scan mode with autosampling but it is enabled   */
/* Only when TMR2 fires which is every 50ms                      */
/* Inputs:  N/A                                                  */
/* Outputs: Boolean that returns true once the target is aligned */
/*          Updates the Message Controller Thread Queue to let   */
/* PathFinder have updated values of the location of the target  */
bool targetAlignmentSubr(){
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
// I probably want to enable the TMR2 interrupt here so that it isnt 
// Running the ADC all the time
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    MessageObj obj;
    TargetAlignment_Type distanceIR;
    memset(&distanceIR, 0, sizeof(TargetAlignment_Type));
    memset(&obj, 0, sizeof(MessageObj));
    obj.Type = UPDATE;
    obj.Update.Type = ALIGNMENTDATA;
    dbgOutputLoc(BEFORE_WHILE_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
    while(1){
        dbgOutputLoc(BEFORE_RECEIVE_FROM_Q_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
        if(xQueueReceive(_queue, &distanceIR, portMAX_DELAY)){
            // Storing the converted cm value in the message object
            obj.Update.Data.alignmentData.IR_0_bool = cmValChecker(convertTocmIR(distanceIR.IR_0));
            obj.Update.Data.alignmentData.IR_1_bool = cmValChecker(convertTocmIR(distanceIR.IR_1));
            obj.Update.Data.alignmentData.IR_2_bool = cmValChecker(convertTocmIR(distanceIR.IR_2));
            dbgOutputLoc(AFTER_CMVALCHECKER_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
            
            if(obj.Update.Data.alignmentData.IR_0_bool == true){
                obj.Update.Data.alignmentData.IR_0 = convertTocmIR(distanceIR.IR_0);
            }
            else{
                obj.Update.Data.alignmentData.IR_0 = 0.0;
            }
            if(obj.Update.Data.alignmentData.IR_1_bool == true){
                obj.Update.Data.alignmentData.IR_1 = convertTocmIR(distanceIR.IR_1);
            }
            else{
                obj.Update.Data.alignmentData.IR_1 = 0.0;
            }
            if(obj.Update.Data.alignmentData.IR_2_bool == true){
                obj.Update.Data.alignmentData.IR_2 = convertTocmIR(distanceIR.IR_2);
            }
            else{
                obj.Update.Data.alignmentData.IR_2 = 0.0;
            }
            dbgOutputLoc(AFTER_BOOLEAN_IFS_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
            computation_threadData.alignmentResult = determineAlignment(obj.Update.Data.alignmentData.IR_0_bool,
                                                                        obj.Update.Data.alignmentData.IR_1_bool,
                                                                        obj.Update.Data.alignmentData.IR_2_bool,
                                                                        obj.Update.Data.alignmentData.IR_0,
                                                                        obj.Update.Data.alignmentData.IR_1,
                                                                        obj.Update.Data.alignmentData.IR_2);
            dbgOutputLoc(BEFORE_SWITCH_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
            switch(computation_threadData.alignmentResult)
            {
                case NOT_ALIGNED:
                    obj.Update.Data.alignmentData.internalAlignment = NOT_ALIGNED;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                case SLIGHT_RIGHT:
                    obj.Update.Data.alignmentData.internalAlignment = SLIGHT_RIGHT;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                case SLIGHT_LEFT:
                    obj.Update.Data.alignmentData.internalAlignment = SLIGHT_LEFT;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                case MAJOR_RIGHT:
                    obj.Update.Data.alignmentData.internalAlignment = MAJOR_RIGHT;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                case MAJOR_LEFT:
                    obj.Update.Data.alignmentData.internalAlignment = MAJOR_LEFT;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                case ALIGNED_NOT_CLOSE:
                    obj.Update.Data.alignmentData.internalAlignment = ALIGNED_NOT_CLOSE;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                case ALIGNED:
                    obj.Update.Data.alignmentData.internalAlignment = ALIGNED;
                    obj.Update.Data.alignmentData.Grabber_Aligned = true;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                default:
                    break;
            }
            dbgOutputLoc(BEFORE_SWITCH_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
        }
        dbgOutputLoc(AFTER_RECEIVE_FROM_Q_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
        return obj.Update.Data.alignmentData.Grabber_Aligned; // returning if its aligned
    }
}

/* This subroutine determines if the alignment values are within range    */
/* to acquire the target and proceed to the next state                    */
/* Inputs:  All three IR values and booleans that determine if the       */
/* values are within the correct reading range of the sensors             */
/* Outputs: 2 if the target is aligned and close enough to grab           */
/*          1 if the target is not close enough                           */
/*          0 if the target is not aligned                                */
int determineAlignment(bool ir_0_bool, bool ir_1_bool, bool ir_2_bool, float ir_0, float ir_1, float ir_2){
    dbgOutputLoc(ENTER_DETERMINEALIGNMENT_COMPUTATION_THREAD);
    float ir0_ir1_range;
    float ir1_ir2_range;
    float ir0_ir2_range;
    ir0_ir1_range = fabs(ir_0 - ir_1);
    ir1_ir2_range = fabs(ir_1 - ir_2);
    ir0_ir2_range = fabs(ir_0 - ir_2);
    if((ir_0_bool == true) && (ir_1_bool == true) && (ir_2_bool == true)){
        if((ir0_ir1_range < 2.0) && (ir1_ir2_range < 2.0) && (ir0_ir2_range < 2.0)){ // This will need some error tolerance
            if(ir_1 < 4.0){ // tells that theyre close enough to grab
                return 6; // ALIGNED
            }
            else{
                return 5; // ALIGNED NOT CLOSE
            }
        }
        else{ 
            return 1; // ALIGNED NOT CLOSE
        }
    }
    else if(ir_0_bool == true){
        if((ir0_ir1_range < 2.0)){ // && (ir1_ir2_range > 2.0)){
            return 1; // SLIGHT RIGHT
        }
        else if((ir0_ir1_range > 2.0) || (ir_2_bool == false)){ // && (ir1_ir2_range > 2.0)){
            return 3; // MAJOR RIGHT
        }
        else{
            return 1; // SLIGHT RIGHT bc we need minor adjustment; not sure how much to move
        }
    }
    else if(ir_2_bool == true){
        if((ir1_ir2_range < 2.0)){ // && (ir0_ir1_range > 2.0)){
            return 2; // SLIGHT LEFT
        }
        else if((ir1_ir2_range > 2.0) || (ir_0_bool == false)){ // && (ir0_ir1_range > 2.0)){
            return 4; // MAJOR LEFT
        }
        else{
            return 2; // SLIGHT LEFT bc we need minor adjustment; not sure how much to move
        }
    }
    else {
        return 0; // NOT ALIGNED latest values were not good, not aligned
    }
    dbgOutputLoc(LEAVE_DETERMINEALIGNMENT_COMPUTATION_THREAD);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void COMPUTATION_THREAD_Initialize ( void )

  Remarks:
    See prototype in computation_thread.h.
 */

void COMPUTATION_THREAD_Initialize ( void )
{
    /* Start necessary hardware and queues before threads start */
    _queue = createComputationQ();
    DRV_TMR0_Start();
    DRV_ADC_Open();
    /* Place the App state machine in its initial state. */
    computation_threadData.state = COMPUTATION_THREAD_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void COMPUTATION_THREAD_Tasks ( void )

  Remarks:
    See prototype in computation_thread.h.
 */

void COMPUTATION_THREAD_Tasks ( void )
{
    dbgOutputLoc(ENTER_COMPUTATION_THREAD);
    /* Check the application's current state. */
    switch ( computation_threadData.state )
    {
        /* Application's initial state. */
        case COMPUTATION_THREAD_STATE_INIT:
        {
            dbgOutputLoc(INIT_STATE_COMPUTATION_THREAD);
            /* Going to next State: COMPUTATION_THREAD_STATE_REQUEST_TARGET_PROXIMITY */
            computation_threadData.state = COMPUTATION_THREAD_STATE_REQUEST_TARGET_PROXIMITY;
            break;
        }
        /* This case determines if I can start scanning for targets */
        case COMPUTATION_THREAD_STATE_REQUEST_TARGET_PROXIMITY:
        {
            dbgOutputLoc(REQUEST_TARGET_PROXIMITY_STATE_COMPUTATION_THREAD);
            // SEND REQUEST FOR TARGET PROXIMITY
            /* Going to next State: COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT */
            computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT;
            break;
        }
 
        /* This case determines if the target is aligned */
        case COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT:
        {
            dbgOutputLoc(TARGET_ALIGNMENT_STATE_COMPUTATION_THREAD);
            while(1){
                bool nextState = false;
                nextState = targetAlignmentSubr();
            }
//            if(nextState == true){
//                /* Going to next State: COMPUTATION_THREAD_STATE_TARGET_ACQUIRE */
//                //computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_ACQUIRE;
//                /* Keeping State here: COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT */
//                computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT;
//            }
//            else{
//                /* Keeping State here: COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT */
//                computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT;
//            }
            break;
        }
        /* This case tells the grabber to acquire(grab) the target */
        case COMPUTATION_THREAD_STATE_TARGET_ACQUIRE:
        {
            dbgOutputLoc(TARGET_ACQUIRE_STATE_COMPUTATION_THREAD);
            // GRAB TARGET
            /* Going to next State: COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA */
            computation_threadData.state = COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA;
            break;
        }
        // Possibly checks to ensure the target is still there, if not go back
        // to target alignment
        /* This case keeps requesting to see if it can drop the target yet */
        case COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA:
        {
            dbgOutputLoc(REQUEST_OUTSIDE_OF_ARENA_STATE_COMPUTATION_THREAD);
            // SEND REQUEST FOR ARENA
            /* Going to next State: COMPUTATION_THREAD_STATE_TARGET_DROP */
            computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_DROP;
            break;
        }
        /* This case tells the grabber to drop the target */
        case COMPUTATION_THREAD_STATE_TARGET_DROP:
        {
            dbgOutputLoc(TARGET_DROP_STATE_COMPUTATION_THREAD);
            // DROP TARGET
            /* Going to next State: COMPUTATION_THREAD_STATE_INIT */
            computation_threadData.state = COMPUTATION_THREAD_STATE_INIT;
            break;
        }
        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
