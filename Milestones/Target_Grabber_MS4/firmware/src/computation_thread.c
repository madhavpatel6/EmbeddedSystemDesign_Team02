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
#include "peripheral/oc/plib_oc.h"
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

#define SIZEOFQUEUE 36
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
    tempAdcVoltageConv = (tempAdcVal*3.0)/(1024.0);
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
        avgIRAdcVal = ((1.0*irDigitalVal)/(3.0)); // This is because of 5 sensors and 15 samples
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

/* This function will filter out data that does not concern the alignment subr  */
/* at the current time.  Ex. say that two objects are being seen but one is     */
/* behind the other.  Therefore I only care about the object in the front.      */
/* Inputs:  All Five IR values and booleans that determine if the         */
/* values are within the correct reading range of the sensors             */
/* Outputs: */
void filterIRData(bool toBefilterIR_0_bool, bool toBefilterIR_1_bool, 
                  bool toBefilterIR_2_bool, bool toBefilterIR_3_bool, 
                  bool toBefilterIR_4_bool, float toBefilterIR_0, 
                  float toBefilterIR_1, float toBefilterIR_2, float toBefilterIR_3, 
                  float toBefilterIR_4, bool *filterIR_0_bool, 
                  bool *filterIR_1_bool, bool *filterIR_2_bool, 
                  bool *filterIR_3_bool, bool *filterIR_4_bool, 
                  float *filterIR_0, float *filterIR_1, 
                  float *filterIR_2, float *filterIR_3, float *filterIR_4){
    float ir0_ir1_range;
    float ir1_ir2_range;
    float ir2_ir3_range;
    float ir3_ir4_range;
    float ir0_ir2_range;
    float ir0_ir3_range;
    float ir0_ir4_range;
    float ir1_ir3_range;
    float ir1_ir4_range;
    float ir2_ir4_range;
    ir0_ir1_range = fabs(toBefilterIR_0 - toBefilterIR_1);
    ir1_ir2_range = fabs(toBefilterIR_1 - toBefilterIR_2);
    ir2_ir3_range = fabs(toBefilterIR_2 - toBefilterIR_3);
    ir3_ir4_range = fabs(toBefilterIR_3 - toBefilterIR_4);
    ir0_ir2_range = fabs(toBefilterIR_0 - toBefilterIR_2);
    ir0_ir3_range = fabs(toBefilterIR_0 - toBefilterIR_3);
    ir0_ir4_range = fabs(toBefilterIR_0 - toBefilterIR_4);
    ir1_ir3_range = fabs(toBefilterIR_1 - toBefilterIR_3);
    ir1_ir4_range = fabs(toBefilterIR_1 - toBefilterIR_4);
    ir2_ir4_range = fabs(toBefilterIR_2 - toBefilterIR_4);
   
    // All are bools are false if there is a gap ex: 01010 in between sensors.
    // I know that scenario is not possible. therefore all bools are thrown out
    // Until next reading or if only two in the center are reading ex: 01100
    // If a reading is on then end and there is a gap the ending on the end
    // is taken as if it is the only true value
    switch((toBefilterIR_0_bool << 4) | (toBefilterIR_1_bool << 3) | (toBefilterIR_2_bool << 2) | (toBefilterIR_3_bool << 1) | (toBefilterIR_4_bool)){
        // 00000
        case 0:{
            *filterIR_0_bool = false;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = false;
            
            /* Zeros because boolean is false */
            *filterIR_0 = 0.0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = 0.0;
            break;
        }
        // 00001
        case 1:{
            *filterIR_0_bool = false;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = true;
            
            /* Zeros because boolean is false */
            *filterIR_0 = 0.0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = toBefilterIR_4;
            break;
        }
        // 00010
        case 2:{
            *filterIR_0_bool = false;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = false;
            
            /* Zeros because boolean is false */
            *filterIR_0 = 0.0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = 0.0;
            break;
        }
        // 00011
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s))
        case 3:{
            // This is the expected/wanted if statement we want to occur
            if(ir3_ir4_range < 2.6){
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = true;
                *filterIR_4_bool = true;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = toBefilterIR_3;
                *filterIR_4 = toBefilterIR_4;
            }
            else{
                // If this is true then keep IR_4 because it is the closer object
                if(toBefilterIR_3 > toBefilterIR_4){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = toBefilterIR_4;
                }
                // Else then keep IR_3 because it is the closer object
                // Since it is interior do not keep. get new readings
                else{
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
            }
            break;
        }
        // 00100
        case 4:{
            *filterIR_0_bool = false;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = false;
            
            /* Zeros because boolean is false */
            *filterIR_0 = 0.0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = 0.0;
            break;
        }
        // 00101
        case 5:{
            *filterIR_0_bool = false;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = true;
            
            /* Zeros because boolean is false */
            *filterIR_0 = 0.0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = toBefilterIR_4;
            break;
        }
        // 00110
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out values
        case 6:{
            // This is a valid target
            if(ir2_ir3_range < 2.6){
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = true;
                *filterIR_3_bool = true;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = toBefilterIR_2;
                *filterIR_3 = toBefilterIR_3;
                *filterIR_4 = 0.0;
            }
            else{
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            break;
        }
        // 00111
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 7:{
            // If true then ir2 is the closest to an object
            if((toBefilterIR_2 < toBefilterIR_3)&&(toBefilterIR_2 < toBefilterIR_4)){
                // This is the case we want
                if((ir2_ir3_range < 2.6)&&(ir2_ir4_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here something is out of range
                else{
                    // If true then IR3 is bad. IR2 is good
                    if(ir2_ir3_range > 2.6){
                        // If true then IR2 is good and IR3 & IR4 are bad
                        // Since interior point then throw out and get new readings 00100
                        if(ir2_ir4_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                        // If gets here then IR2 & IR4 are good. IR3 is bad
                        // Since IR2 is interior then only keep IR4 00101
                        else{
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = toBefilterIR_4;
                        }
                    }
                    // If gets here then IR2&IR3 are good. check IR4
                    else{
                        // If true then IR2&IR3 are good IR4 is bad.
                        // Throw out values because interior point 00110
                        if(ir2_ir4_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = 0.0;
                        }
                    }
                }
            }
            // If true then ir3 is the closest to an object
            else if((toBefilterIR_3 < toBefilterIR_2)&&(toBefilterIR_3 < toBefilterIR_4)){
                // This is the case we want
                if((ir2_ir3_range < 2.6)&&(ir3_ir4_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here something is out of range
                else{
                    // If true then IR2 is bad. IR3 is good
                    if(ir2_ir3_range > 2.6){
                        // If true then IR3 is good and IR2 & IR4 are bad
                        // Since interior point then throw out and get new readings 00010
                        if(ir3_ir4_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                        // If gets here then IR3 & IR4 are good. IR2 is bad
                        else{   
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = toBefilterIR_4;
                        }
                    }
                    // If gets here then IR2&IR3 are good. check IR4
                    else{
                        // If true then IR2&IR3 are good IR4 is bad.
                        // Throw out values because interior point 00110
                        if(ir3_ir4_range > 2.6){   
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = 0.0;
                        }
                    }
                }
            }
            // If true then ir4 is the closest to an object
            else if((toBefilterIR_4 < toBefilterIR_3)&&(toBefilterIR_4 < toBefilterIR_2)){
                // This is the case we want
                if((ir2_ir4_range < 2.6)&&(ir3_ir4_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here something is out of range
                else{
                    // If true then IR2 is bad. IR4 is good
                    if(ir2_ir4_range > 2.6){
                        // If true then IR4 is good and IR3 & IR2 are bad
                        if(ir3_ir4_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = toBefilterIR_4;
                        }
                        // If gets here then IR3 & IR4 are good. IR2 is bad
                        else{
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = toBefilterIR_4;
                        }
                    }
                    // If gets here then IR2&IR4 are good. check IR3
                    else{
                        // If true then IR2&IR4 are good IR3 is bad.
                        // Throw out values because interior point 00101
                        // Only keep IR4
                        if(ir3_ir4_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = toBefilterIR_4;
                        }
                    }
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 222.0;
//                *filterIR_1 = 222.0;
//                *filterIR_2 = 222.0;
//                *filterIR_3 = 222.0;
//                *filterIR_4 = 222.0;
//            }
//            break;
            else{
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = *filterIR_3_bool;
                *filterIR_4_bool = *filterIR_4_bool;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = *filterIR_3;
                *filterIR_4 = *filterIR_4;
            }
            break;
        }
        // 01000
        case 8:{
            *filterIR_0_bool = false;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = false;
            
            /* Zeros because boolean is false */
            *filterIR_0 = 0.0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = 0.0;
            break;
        }
        // 01001
        case 9:{
            *filterIR_0_bool = false;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = true;
            
            /* Zeros because boolean is false */
            *filterIR_0 = 0.0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = toBefilterIR_4;
            break;
        }
        // 01010
        case 10:{
            *filterIR_0_bool = false;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = false;
            
            /* Zeros because boolean is false */
            *filterIR_0 = 0.0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = 0.0;
            break;
        }
        // 01011
        case 11:{
            // This is the expected/wanted if statement we want to occur
            if(ir3_ir4_range < 2.6){
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = true;
                *filterIR_4_bool = true;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = toBefilterIR_3;
                *filterIR_4 = toBefilterIR_4;
            }
            else{
                // If this is true then keep IR_4 because it is the closer object
                if(toBefilterIR_3 > toBefilterIR_4){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = toBefilterIR_4;
                }
                // Else then keep IR_3 because it is the closer object
                // Since it is interior do not keep. get new readings
                else{
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
            }
            break;
        }
        // 01100
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out values
        case 12:{
            // This is a valid target
            if(ir1_ir2_range < 2.6){
                *filterIR_0_bool = false;
                *filterIR_1_bool = true;
                *filterIR_2_bool = true;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = toBefilterIR_1;
                *filterIR_2 = toBefilterIR_2;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            else{
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            break;
        }
        // 01101
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out values
        case 13:{           
            // If true then IR1 or IR2 is closest to object
            if(((toBefilterIR_1 < toBefilterIR_2)&&(toBefilterIR_1 < toBefilterIR_4)) || ((toBefilterIR_2 < toBefilterIR_1)&&(toBefilterIR_2 < toBefilterIR_4))){
                // This is a valid target
                if(ir1_ir2_range < 2.6){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // If gets here IR4 is good. throw out rest because interior points
                else if((ir1_ir4_range < 2.6) || (ir2_ir4_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here then throw out everything. interior points
                else{
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
            }
            // If true then IR4 is closest to object
            else if((toBefilterIR_4 < toBefilterIR_1)&&(toBefilterIR_4 < toBefilterIR_2)){
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = true;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = toBefilterIR_4;
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 333.0;
//                *filterIR_1 = 333.0;
//                *filterIR_2 = 333.0;
//                *filterIR_3 = 333.0;
//                *filterIR_4 = 333.0;
//            }
//            break;
            else{
                *filterIR_0_bool = false;
                *filterIR_1_bool = *filterIR_1_bool;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = false;
                *filterIR_4_bool = *filterIR_4_bool;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = *filterIR_1;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = 0.0;
                *filterIR_4 = *filterIR_4;
            }
            break;
        }
        // 01110
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 14:{
            // This returns that IR_1 is the smallest value (closest object)
            if((toBefilterIR_1 < toBefilterIR_2)&&(toBefilterIR_1 < toBefilterIR_3)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir1_ir2_range < 2.6)&&(ir1_ir3_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = 0.0;
                }
                // This happens because a value is outside of the range
                else{
                    // If true then IR1 & IR2 is good, IR3 bad
                    if(ir1_ir2_range < 2.6){
                        // Keep because interior point 01100
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = true;
                        *filterIR_2_bool = true;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = toBefilterIR_1;
                        *filterIR_2 = toBefilterIR_2;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    // If gets here i know that IR2 is bad IR1 & IR3 good. 
                    else{
                        // Throw out because interior point 01010
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                }
            }
            // This returns that IR_2 is the smallest value (closest object)
            else if((toBefilterIR_2 < toBefilterIR_1)&&(toBefilterIR_2 < toBefilterIR_3)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir1_ir2_range < 2.6)&&(ir2_ir3_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = 0.0;
                }
                // This happens because a value is outside of the range
                else{
                    // If true then IR2&IR1 is good; IR3 is bad
                    if(ir1_ir2_range < 2.6){
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = true;
                        *filterIR_2_bool = true;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = toBefilterIR_1;
                        *filterIR_2 = toBefilterIR_2;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    else if(ir2_ir3_range < 2.6){
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = true;
                        *filterIR_3_bool = true;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = toBefilterIR_2;
                        *filterIR_3 = toBefilterIR_3;
                        *filterIR_4 = 0.0;
                    }
                    else{
                        // If true then IR2 is good and IR1&IR3 are bad
                        // Since interior point throw out all values 00100
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                }
            }
            // This returns that IR_3 is the smallest value (closest object)
            else if((toBefilterIR_3 < toBefilterIR_2)&&(toBefilterIR_3 < toBefilterIR_1)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir2_ir3_range < 2.6)&&(ir1_ir3_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = 0.0;
                }
                // This happens because a value is outside of the range
                else{
                    // If true then IR3 & IR2 are good and IR1 is bad
                    if(ir2_ir3_range < 2.6){
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = true;
                        *filterIR_3_bool = true;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = toBefilterIR_2;
                        *filterIR_3 = toBefilterIR_3;
                        *filterIR_4 = 0.0;
                    }
                    else{
                        // If true then IR3 is good and IR1&IR2 are bad
                        // Since interior point throw out values 00010
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 200.0;
//                *filterIR_1 = 200.0;
//                *filterIR_2 = 200.0;
//                *filterIR_3 = 200.0;
//                *filterIR_4 = 200.0;
//            }
//            break;
            else{
                *filterIR_0_bool = false;
                *filterIR_1_bool = *filterIR_1_bool;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = *filterIR_3_bool;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = *filterIR_1;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = *filterIR_3;
                *filterIR_4 = 0.0;
            }
            break;
        }
        // 01111
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 15:{
            // This returns that IR_1 is the smallest value (closest object)
            if((toBefilterIR_1 < toBefilterIR_2)&&(toBefilterIR_1 < toBefilterIR_3)&&(toBefilterIR_1 < toBefilterIR_4)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir1_ir4_range < 2.6)&&(ir1_ir3_range < 2.6)&&(ir1_ir2_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // This happens because a value is outside of the range
                else{
                    // If true then at least IR4 is bad
                    if(ir1_ir4_range > 2.6){
                        // If true then at least IR4 and IR3 are bad
                        if(ir1_ir3_range > 2.6){
                            // If true then all values are bad except IR1, Get new readings
                            if(ir1_ir2_range > 2.6){
                                // But since it falls in an interior case: ex 01000 then its bad
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here IR1 and IR2 are good values, throw out the rest
                            else{
                                // But since it falls in an interior case: ex 01100 then its good
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                        }
                        // If gets here then I know IR4 is bad. IR3 & IR1 is good. Check IR2
                        else{
                            // If true then IR2&IR4 is bad; IR3 is good.
                            // But since it falls in an interior case: ex 01010 then its bad
                            if(ir1_ir2_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If get here then I know that IR4 is bad. IR2 & IR1 & IR3 are good
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                    // If true then at least IR4&IR1 is good and IR3 is bad
                    else if(ir1_ir3_range > 2.6){
                        // If true then IR4 & IR1 are good and IR3 & IR2 are bad
                        if(ir1_ir2_range > 2.6){
                            // But since IR1 is now interior case all are bad except IR4
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = toBefilterIR_4;
                        }
                        // If gets here then IR1 & IR2 good IR 4good. only keep IR2&4 because closer to object
                        else{
                            // But since IR1 is now interior case IR1&IR2 good
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                    // If here then we know IR4&IR3&IR1 are good and IR2 is bad reading
                    // But since IR1 are now interior case all are bad except IR4 & IR3 CHECK HERE EDIT
                    else{
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = true;
                        *filterIR_4_bool = true;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = toBefilterIR_3;
                        *filterIR_4 = toBefilterIR_4;
                    }
                }
            }
            // This returns that IR_2 is the smallest value (closest object)
            else if((toBefilterIR_2 < toBefilterIR_1)&&(toBefilterIR_2 < toBefilterIR_3)&&(toBefilterIR_2 < toBefilterIR_4)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir2_ir4_range < 2.6)&&(ir2_ir3_range < 2.6)&&(ir1_ir2_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // This happens because a value is outside of the range
                else{
                    // If true then at least IR4 is bad
                    if(ir2_ir4_range > 2.6){
                        // If true then at least IR4 and IR3 are bad
                        if(ir2_ir3_range > 2.6){
                            // If true then all values are bad except IR2, Get new readings
                            if(ir1_ir2_range > 2.6){
                                // But since it falls in an interior case: ex 00100 then its bad
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here keep IR1&IR2
                            else{
                                // But since it falls in an interior case: ex 01100 then its good
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                        }
                        // If gets here then I know IR4 is bad. IR3&IR2 are good. Check IR1
                        else{
                            // If true then IR1&IR4 is bad; IR3&IR2 is good.
                            // But since it falls in an interior case: ex 00110 then its good
                            if(ir1_ir2_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                            // If get here then I know that IR4 is bad. IR2 & IR1 & IR3 are good
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                    // If true then at least IR4 & IR2 is good and IR3 is bad
                    else if(ir2_ir3_range > 2.6){
                        // If true then IR4 is good and IR3 & IR1 are bad
                        if(ir1_ir2_range > 2.6){
                            // But since IR2 is now interior case all are bad except IR4 CHECK HERE EDIT
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = toBefilterIR_4;
                        }
                        // If gets here then I know IR1&IR2&IR4 good. only keep IR1&IR2 closer to object
                        else{
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                    // If here then we know IR4 & IR3 & IR2 are good and IR1 is bad reading
                    else{
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = true;
                        *filterIR_3_bool = true;
                        *filterIR_4_bool = true;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = toBefilterIR_2;
                        *filterIR_3 = toBefilterIR_3;
                        *filterIR_4 = toBefilterIR_4;
                    }
                }
            }
            // This returns that IR_3 is the smallest value (closest object)
            else if((toBefilterIR_3 < toBefilterIR_1)&&(toBefilterIR_3 < toBefilterIR_2)&&(toBefilterIR_3 < toBefilterIR_4)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir3_ir4_range < 2.6)&&(ir1_ir3_range < 2.6)&&(ir2_ir3_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // This happens because a value is outside of the range
                else{
                    // If true then at least IR4 is bad
                    if(ir3_ir4_range > 2.6){
                        // If true then at least IR4 and IR1 are bad
                        if(ir1_ir3_range > 2.6){
                            // If true then all values are bad except IR3, Get new readings
                            if(ir2_ir3_range > 2.6){
                                // But since it falls in an interior case: ex 00100 then its bad
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here IR2&IR3 are good
                            else{
                                // But since it falls in an interior case: ex 00100 then its bad
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                        // If gets here then I know IR4 is bad. IR3&IR1 are good. Check IR2
                        else{
                            // If true then IR2&IR4 are bad; IR3 & IR1 is good.
                            // But since it falls in an interior case: ex 01010 then its bad
                            if(ir2_ir3_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If get here then I know that IR4 is bad. IR2 & IR1 & IR3 are good
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                    // If true then at least IR3 & IR4 is good and IR1 is bad
                    else if(ir1_ir3_range > 2.6){
                        // If true then IR3&IR4 are good and IR1 & IR2 are bad
                        if(ir2_ir3_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = toBefilterIR_4;
                        }
                        // If gets here then i know that IR3&IR2&IR4 are good. IR1 is bad
                        else{
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = toBefilterIR_4;
                        }
                    }
                    // If here then we know IR4 & IR3 & IR1 are good and IR2 is bad reading
                    // But since IR1 are now interior case all are bad except IR4 & IR3 CHECK HERE EDIT
                    else{
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = true;
                        *filterIR_4_bool = true;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = toBefilterIR_3;
                        *filterIR_4 = toBefilterIR_4;
                    }
                }
            }
            // This returns that IR_4 is the smallest value (closest object)
            else if((toBefilterIR_4 < toBefilterIR_1)&&(toBefilterIR_4 < toBefilterIR_2)&&(toBefilterIR_4 < toBefilterIR_3)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir1_ir4_range < 2.6)&&(ir3_ir4_range < 2.6)&&(ir2_ir4_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // This happens because a value is outside of the range
                else{
                    // If true then at least IR1 is bad
                    if(ir1_ir4_range > 2.6){
                        // If true then at least IR1 and IR3 are bad
                        if(ir3_ir4_range > 2.6){
                            // If true then all values are bad except IR4, Get new readings
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = toBefilterIR_4;
                        }
                        // If gets here then I know IR1 is bad. IR3 & IR4 is good. Check IR2
                        else{
                            // If true then IR2&IR1 is bad; IR3 & IR4 is good.
                            if(ir2_ir4_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                            // If get here then I know that IR1 is bad. IR2 & IR4 & IR3 are good
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                    }
                    // If true then at least IR1 & IR4 are good and IR3 is bad
                    else if(ir3_ir4_range > 2.6){
                        // If true then IR1 & IR4 are good and IR3 & IR2 are bad
                        // But since IR1 is now interior case all are bad except IR4 CHECK HERE EDIT
                        // Should not go inside this case typically
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = true;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = toBefilterIR_4;
                    }
                    // If here then we know IR4 & IR1 & IR3 are good and IR2 is bad reading
                    // But since IR1 are now interior case all are bad except IR4 & IR3
                    else{
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = true;
                        *filterIR_4_bool = true;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = toBefilterIR_3;
                        *filterIR_4 = toBefilterIR_4;
                    }
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 200.0;
//                *filterIR_1 = 200.0;
//                *filterIR_2 = 200.0;
//                *filterIR_3 = 200.0;
//                *filterIR_4 = 200.0;
//            }
//            break;
            else{
                *filterIR_0_bool = false;
                *filterIR_1_bool = *filterIR_1_bool;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = *filterIR_3_bool;
                *filterIR_4_bool = *filterIR_4_bool;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = *filterIR_1;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = *filterIR_3;
                *filterIR_4 = *filterIR_4;
            }
            break;
        }      
        // 10000
        case 16:{
            *filterIR_0_bool = true;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = false;
            
            /* Zeros because boolean is false */
            *filterIR_0 = toBefilterIR_0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = 0.0;
            break;
        }
        // 10001
        case 17:{
            // If true then I know IR0 is the smallest(Closest to an object)
            if(toBefilterIR_0 < toBefilterIR_4){
                *filterIR_0_bool = true;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = toBefilterIR_0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            // If gets here then I know that IR4 is closest to an object
            else{
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = true;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = toBefilterIR_4;
            }
            break;
        }
        // 10010
        case 18:{
            *filterIR_0_bool = true;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = false;
            
            /* Zeros because boolean is false */
            *filterIR_0 = toBefilterIR_0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = 0.0;
            break;
        }
        // 10011
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 19:{
            // If true then IR0 is the closest to an object
            // Therefore ignore the other two readings.
            if((toBefilterIR_0 < toBefilterIR_3) && (toBefilterIR_0 < toBefilterIR_4)){
                *filterIR_0_bool = true;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = toBefilterIR_0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            // If true then IR3 is the closest to an object
            else if((toBefilterIR_3 < toBefilterIR_0) && (toBefilterIR_3 < toBefilterIR_4)){
                // If true then IR4 is also in range
                if(ir3_ir4_range < 2.6){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // IR3 good. IR4 bad
                else{
                    // If true thenIR3 good. IR4&IR0 are bad. Throw out everything because interior points 00010
                    if(ir0_ir3_range > 2.6){
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    // If gets here keep IR0 because interior points 10010
                    else{
                        *filterIR_0_bool = true;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = toBefilterIR_0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    
                }
            }
            // If true then IR4 is the closest to an object
            else if((toBefilterIR_4 < toBefilterIR_3) && (toBefilterIR_4 < toBefilterIR_0)){
                // If true then IR3 is also in range
                if(ir3_ir4_range < 2.6){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // IR3 is bad. IR0 is not good because it is farther away than IR4.
                // Therefore only IR4 is returned good
                else{
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = toBefilterIR_4;
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 666.0;
//                *filterIR_1 = 666.0;
//                *filterIR_2 = 666.0;
//                *filterIR_3 = 666.0;
//                *filterIR_4 = 666.0;
//            }
//            break;
            else{
                *filterIR_0_bool = *filterIR_0_bool;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = *filterIR_3_bool;
                *filterIR_4_bool = *filterIR_4_bool;

                /* Zeros because boolean is false */
                *filterIR_0 = *filterIR_0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = *filterIR_3;
                *filterIR_4 = *filterIR_4;
            }
            break;
        }
        // 10100
        case 20:{
            *filterIR_0_bool = true;
            *filterIR_1_bool = false;
            *filterIR_2_bool = false;
            *filterIR_3_bool = false;
            *filterIR_4_bool = false;
            
            /* Zeros because boolean is false */
            *filterIR_0 = toBefilterIR_0;
            *filterIR_1 = 0.0;
            *filterIR_2 = 0.0;
            *filterIR_3 = 0.0;
            *filterIR_4 = 0.0;
            break;
        }
        // 10101
        case 21:{
            // If true then I know IR0 is the smallest(Closest to an object)
            // Throw out IR2 because its interior points 10101
            if(toBefilterIR_0 < toBefilterIR_4){
                *filterIR_0_bool = true;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = toBefilterIR_0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            // If gets here then I know that IR4 is closest to an object
            // Throw out IR2 because its interior points 10101
            else{
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = true;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = toBefilterIR_4;
            }
            break;
        }
        // 10110
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 22:{
            // If IR0 is closest to object
            if((toBefilterIR_0 < toBefilterIR_2)&&(toBefilterIR_0 < toBefilterIR_3)){
                *filterIR_0_bool = true;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = toBefilterIR_0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
                
            }
            // If IR2 or IR3 is closest to object
            else if(((toBefilterIR_2 < toBefilterIR_0)&&(toBefilterIR_2 < toBefilterIR_3)) || ((toBefilterIR_3 < toBefilterIR_0)&&(toBefilterIR_3 < toBefilterIR_2))){
                // This is a valid target 00110
                if(ir2_ir3_range < 2.6){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = 0.0;
                }
                // If gets here then throw out values
                else{
                    // If true then throw out values
                    if((ir0_ir2_range > 2.6)||(ir0_ir3_range > 2.6)){
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    // If gets here keep only IR0 because interior points
                    else{
                        *filterIR_0_bool = true;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = toBefilterIR_0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 777.0;
//                *filterIR_1 = 777.0;
//                *filterIR_2 = 777.0;
//                *filterIR_3 = 777.0;
//                *filterIR_4 = 777.0;
//            }
//            break;
            else{
                *filterIR_0_bool = *filterIR_0_bool;
                *filterIR_1_bool = false;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = *filterIR_3_bool;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = *filterIR_0;
                *filterIR_1 = 0.0;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = *filterIR_3;
                *filterIR_4 = 0.0;
            }
            break;
        }
        // 10111
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 23:{
            // If true then IR0 is the closest to an object. Throw out other readings
            if((toBefilterIR_0 < toBefilterIR_2)&&(toBefilterIR_0 < toBefilterIR_3)&&(toBefilterIR_0 < toBefilterIR_4)){
                *filterIR_0_bool = true;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = toBefilterIR_0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            // If true then IR2 is the closest to an object.
            else if((toBefilterIR_2 < toBefilterIR_0)&&(toBefilterIR_2 < toBefilterIR_3)&&(toBefilterIR_2 < toBefilterIR_4)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir2_ir4_range < 2.6)&&(ir2_ir3_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here then I know something needs to be thrown out
                else{
                    // If true then I know that IR2 is good and IR4 is bad
                    if(ir2_ir4_range > 2.6){
                        // If true then IR2 is good and IR4 & IR3 are bad
                        if(ir2_ir3_range > 2.6){
                            // If true then IR2 is good and IR4&IR3&IR0 are bad
                            // Since IR2 is interior all values are thrown out.
                            if(ir0_ir2_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then Keep IR0 because interior points 10100
                            // IR2&IR0 good. Only keep IR0
                            else{
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                        }
                        // If gets here then
                        // Keep IR2&IR3 throw out the rest 00110
                        else{
                            // Since IR2 is interior all values are thrown out. CHECK
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = 0.0;
                        }
                    }
                    // If get here then IR2 & IR4 are good. check IR3
                    else{
                        // If true then IR2&IR4 are good and IR3 is bad
                        // Since IR2 is interior throw it out. ex 00101
                        // Keep only IR4
                        if(ir2_ir3_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = toBefilterIR_4;
                        }
                    }
                }
            }
            // If true then IR3 is the closest to an object.
            else if((toBefilterIR_3 < toBefilterIR_0)&&(toBefilterIR_3 < toBefilterIR_2)&&(toBefilterIR_3 < toBefilterIR_4)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir3_ir4_range < 2.6)&&(ir2_ir3_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here then I know something needs to be thrown out
                else{
                    // If true then I know that IR3 is good and IR4 is bad
                    if(ir3_ir4_range > 2.6){
                        // If true then IR3 is good and IR4 & IR2 are bad
                        if(ir2_ir3_range > 2.6){
                            // If true the IR3 good. IR4&IR2&IR0 bad
                            // Since IR3 is interior all values are thrown out.
                            if(ir0_ir3_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here keep IR0 only. IR3&IR0 good. interior points 10010
                            else{
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                        }
                        // If gets here then IR2&IR3 are good 00110
                        else{
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = 0.0;
                        }
                    }
                    // If get here then IR3 & IR4 are good. check IR2
                    else{
                        // If true then IR3&IR4 are good and IR2 is bad
                        if(ir2_ir3_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = toBefilterIR_4;
                        }
                    }
                }
            }
            // If true then IR4 is the closest to an object.
            // Throw out IR0
            else if((toBefilterIR_4 < toBefilterIR_0)&&(toBefilterIR_4 < toBefilterIR_3)&&(toBefilterIR_4 < toBefilterIR_2)){
                // This is the if statement that we want to occur. Everything is within range
                if((ir3_ir4_range < 2.6)&&(ir2_ir4_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here then I know something needs to be thrown out
                else{
                    // If true then I know that IR4 is good and IR3 is bad
                    if(ir3_ir4_range > 2.6){
                        // If true then IR4 is good and IR3 & IR2 are bad
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = true;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = toBefilterIR_4;
                    }
                    // If get here then IR3 & IR4 are good. check IR2
                    else{
                        // If true then IR3&IR4 are good and IR2 is bad
                        if(ir2_ir4_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = toBefilterIR_4;
                        }
                    }
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 888.0;
//                *filterIR_1 = 888.0;
//                *filterIR_2 = 888.0;
//                *filterIR_3 = 888.0;
//                *filterIR_4 = 888.0;
//            }
//            break;
            else{
                *filterIR_0_bool = *filterIR_0_bool;
                *filterIR_1_bool = false;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = *filterIR_3_bool;
                *filterIR_4_bool = *filterIR_4_bool;

                /* Zeros because boolean is false */
                *filterIR_0 = *filterIR_0;
                *filterIR_1 = 0.0;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = *filterIR_3;
                *filterIR_4 = *filterIR_4;
            }
            break;
        }
        // 11000
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 24:{
            // This is the expected/wanted if statement we want to occur
            if(ir0_ir1_range < 2.6){
                *filterIR_0_bool = true;
                *filterIR_1_bool = true;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = toBefilterIR_0;
                *filterIR_1 = toBefilterIR_1;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            else{
                // If this is true then keep IR_0 because it is the closer object
                if(toBefilterIR_0 < toBefilterIR_1){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // Else then keep IR_1 because it is the closer object
                // Get new data because interior point 01000
                else{
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
            }
            break;
        }
        // 11001
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 25:{
            // If true then IR0 is closest to an object
            // Throw out IR4
            if((toBefilterIR_0 < toBefilterIR_1)&&(toBefilterIR_0 < toBefilterIR_4)){
                // If true then IR1&IR4 is bad IR0 is good
                if(ir0_ir1_range > 2.6){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // If gets here then IR0&IR1 are good. IR4 is bad
                else{
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
            }
            // If true then IR1 is closest to an object
            else if((toBefilterIR_1 < toBefilterIR_0)&&(toBefilterIR_1 < toBefilterIR_4)){
                // If true then IR0 is bad.
                if(ir0_ir1_range > 2.6){
                    // If true then IR0&IR4 is bad.
                    // Since interior point throw out values 01000
                    if(ir1_ir4_range > 2.6){
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    // If gets here keep IR4 because interior points 01001
                    else{
                        *filterIR_0_bool = false;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = true;

                        /* Zeros because boolean is false */
                        *filterIR_0 = 0.0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = toBefilterIR_4;
                    }
                }
                // If gets here then IR0&IR1 are good.
                else{
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
            }
            // If true then IR4 is closest to an object
            // Throw out other two readings if true
            else if((toBefilterIR_4 < toBefilterIR_1)&&(toBefilterIR_4 < toBefilterIR_0)){
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = true;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = toBefilterIR_4;
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 999.0;
//                *filterIR_1 = 999.0;
//                *filterIR_2 = 999.0;
//                *filterIR_3 = 999.0;
//                *filterIR_4 = 999.0;
//            }
//            break;
            else{
                *filterIR_0_bool = *filterIR_0_bool;
                *filterIR_1_bool = *filterIR_1_bool;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = *filterIR_4_bool;

                /* Zeros because boolean is false */
                *filterIR_0 = *filterIR_0;
                *filterIR_1 = *filterIR_1;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = *filterIR_4;
            }
            break;
        }
        // 11010
        case 26:{
            // This is the expected/wanted if statement we want to occur
            // Throw out IR3 because interior points 11010
            if(ir0_ir1_range < 2.6){
                *filterIR_0_bool = true;
                *filterIR_1_bool = true;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = toBefilterIR_0;
                *filterIR_1 = toBefilterIR_1;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            else{
                // If this is true then keep IR_0 because it is the closer object
                // Throw out IR3 because interior points 11010
                if(toBefilterIR_0 < toBefilterIR_1){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // Else then keep IR_1 because it is the closer object
                // Get new data because interior points 01010
                else{
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
            }
            break;
        }
        // 11011
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 27:{
            // If true then IR0 is the closest to object
            if((toBefilterIR_0 < toBefilterIR_1)&&(toBefilterIR_0 < toBefilterIR_3)&&(toBefilterIR_0 < toBefilterIR_4)){
                // This is the if statement that we want to occur. Everything is within range
                // EDIT: Might need to change this. will need to test
                // Before test only keeping IR0 and IR1 since they are closer
                if((ir0_ir1_range < 2.6)&&(ir0_ir3_range < 2.6)&&(ir0_ir4_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // This occurs because something is out of range
                else{
                    // If true then IR0 is good and IR1 is bad
                    if(ir0_ir1_range > 2.6){
                        // If true then IR0 is good and IR1&IR3 are bad
                        // If true then IR0 is good and IR1&IR3&IR4 are bad
                        // Only keep IR0
                        *filterIR_0_bool = true;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = toBefilterIR_0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    // If gets here IR1&IR0 are good. Check IR3&IR4
                    else{
                        // If true then IR0&IR1 are good. IR3 is bad
                        // If true then IR1&IR0 are good. IR4&IR3 are bad.
                        *filterIR_0_bool = true;
                        *filterIR_1_bool = true;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = toBefilterIR_0;
                        *filterIR_1 = toBefilterIR_1;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                }
            }
            // If true then IR1 is the closest to object
            else if((toBefilterIR_1 < toBefilterIR_0)&&(toBefilterIR_1 < toBefilterIR_3)&&(toBefilterIR_1 < toBefilterIR_4)){
                // This is the if statement that we want to occur. Everything is within range
                // EDIT: Might need to change this. will need to test
                // Before test only keeping IR0 and IR1 since they are closer
                if((ir0_ir1_range < 2.6)&&(ir1_ir3_range < 2.6)&&(ir1_ir4_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // This occurs because something is out of range
                else{
                    // If true then IR1 is good and IR0 is bad
                    if(ir0_ir1_range > 2.6){
                        // If true then IR1 is good and IR0&IR3 are bad
                        if(ir1_ir3_range > 2.6){
                            // If true then IR1 is good and IR0&IR3&IR4 are bad
                            // Since interior points throw out values 01000
                            if(ir1_ir4_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR1&IR4 are good. IR3&IR0 are bad
                            // Only Keep IR4 since IR1 is interior point
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                        // If gets here then IR1&IR3 are good IR0 is bad. Check IR4
                        else{
                            // If true then IR1&IR3 are good. IR0&IR4 are bad.
                            // Since interior points throw out data.
                            if(ir1_ir4_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR1&IR3&IR4 are good. IR0 is bad.
                            // Keep only IR3&IR4 and throw out the rest
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                    }
                    // If gets here IR1&IR0 are good. Check IR3&IR4
                    else{
                        // If true then IR0&IR1 are good. IR3 is bad
                        // If true then IR1&IR0 are good. IR4&IR3 are bad.
                        *filterIR_0_bool = true;
                        *filterIR_1_bool = true;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = toBefilterIR_0;
                        *filterIR_1 = toBefilterIR_1;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                }
            }
            // If true then IR3 is the closest to object
            else if((toBefilterIR_3 < toBefilterIR_1)&&(toBefilterIR_3 < toBefilterIR_0)&&(toBefilterIR_3 < toBefilterIR_4)){
                // This is the if statement that we want to occur. Everything is within range
                // EDIT: Might need to change this. will need to test
                // Before test only keeping IR3 and IR4 since they are closer
                if((ir0_ir3_range < 2.6)&&(ir1_ir3_range < 2.6)&&(ir3_ir4_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // This occurs because something is out of range
                else{
                    // If true then IR3 is good and IR0 is bad
                    if(ir0_ir3_range > 2.6){
                        // If true then IR3 is good and IR0&IR1 are bad
                        if(ir1_ir3_range > 2.6){
                            // If true then IR3 is good and IR0&IR1&IR4 are bad
                            // Since interior points throw out values 00010
                            if(ir3_ir4_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR3&IR4 are good. IR1&IR0 are bad
                            // Only Keep IR3&IR4
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                        // If gets here then IR1&IR3 are good IR0 is bad. Check IR4
                        else{
                            // If true then IR1&IR3 are good. IR0&IR4 are bad.
                            // Since interior points throw out data.
                            if(ir3_ir4_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR1&IR3&IR4 are good. IR0 is bad.
                            // Keep only IR3&IR4 and throw out the rest
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                    }
                    // If gets here IR3&IR0 are good. Check IR1&IR4
                    else{
                        // If true then IR0&IR3 are good. IR1 is bad
                        if(ir1_ir3_range > 2.6){
                            // If true then IR3&IR0 are good. IR4&IR1 are bad.
                            // Since interior points only keep IR0. Throw out the rest
                            if(ir3_ir4_range > 2.6){
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR0&IR4&IR3 are good. 
                            // Only Keep IR3&IR4 because they are closer to object
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                        // If gets here then IR3&IR1&IR0 are good. check IR4
                        else{
                            // If true then IR3&IR1&IR0 are good. IR4 is bad.
                            // Keep only IR1&IR0
                            if(ir3_ir4_range > 2.6){
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                }
            }
            // If true then IR4 is the closest to object
            else if((toBefilterIR_4 < toBefilterIR_1)&&(toBefilterIR_4 < toBefilterIR_3)&&(toBefilterIR_4 < toBefilterIR_0)){
                // This is the if statement that we want to occur. Everything is within range
                // EDIT: Might need to change this. will need to test
                // Before test only keeping IR3 and IR4 since they are closer
                if((ir0_ir4_range < 2.6)&&(ir1_ir4_range < 2.6)&&(ir3_ir4_range < 2.6)){
                    *filterIR_0_bool = false;
                    *filterIR_1_bool = false;
                    *filterIR_2_bool = false;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* Zeros because boolean is false */
                    *filterIR_0 = 0.0;
                    *filterIR_1 = 0.0;
                    *filterIR_2 = 0.0;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // This occurs because something is out of range
                else{
                    // If true then IR4 is good and IR0 is bad
                    if(ir0_ir4_range > 2.6){
                        // If true then IR4 is good and IR0&IR1 are bad
                        // If true then IR4 is good and IR0&IR1&IR3 are bad
                        // Keep only IR4
                        if(ir3_ir4_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = toBefilterIR_4;
                        }
                        // If gets here then IR3&IR4 are good. IR1&IR0 are bad
                        // Only Keep IR3&IR4
                        else{
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = true;
                            *filterIR_4_bool = true;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = toBefilterIR_3;
                            *filterIR_4 = toBefilterIR_4;
                        }
                    }
                    // If gets here IR4&IR0 are good. Check IR1&IR3
                    else{
                        // If true then IR0&IR4 are good. IR1 is bad
                        if(ir1_ir4_range > 2.6){
                            // If true then IR4&IR0 are good. IR3&IR1 are bad.
                            // Only keep IR4 because it is closest to object
                            if(ir3_ir4_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = toBefilterIR_4;
                            }
                            // If gets here then IR0&IR4&IR3 are good. 
                            // Only Keep IR3&IR4 because they are closer to object
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                        // If gets here then IR4&IR1&IR0 are good. check IR3
                        else{
                            // If true then IR4&IR1&IR0 are good. IR3 is bad.
                            // Keep only IR4
                            if(ir3_ir4_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                    }
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 1111.0;
//                *filterIR_1 = 1111.0;
//                *filterIR_2 = 1111.0;
//                *filterIR_3 = 1111.0;
//                *filterIR_4 = 1111.0;
//            }
//            break;
            else{
                *filterIR_0_bool = *filterIR_0_bool;
                *filterIR_1_bool = *filterIR_1_bool;
                *filterIR_2_bool = false;
                *filterIR_3_bool = *filterIR_3_bool;
                *filterIR_4_bool = *filterIR_4_bool;

                /* Zeros because boolean is false */
                *filterIR_0 = *filterIR_0;
                *filterIR_1 = *filterIR_1;
                *filterIR_2 = 0.0;
                *filterIR_3 = *filterIR_3;
                *filterIR_4 = *filterIR_4;
            }
            break;
        }
        // 11100
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 28:{
            // If true then IR0 is the closest to the object
            if((toBefilterIR_0 < toBefilterIR_1)&&(toBefilterIR_0 < toBefilterIR_2)){
                // If true then all are good values. This is the case we want
                if((ir0_ir1_range < 2.6)&&(ir0_ir2_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // Something is out of range
                else{
                    // If true then IR1 is bad. IR0 is good
                    if(ir0_ir1_range > 2.6){
                        // If true then IR1&IR2 are bad. IR0 is good.
                        // Only keep IR0
                        *filterIR_0_bool = true;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = toBefilterIR_0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    // If gets here then IR0&IR1 are good. check IR2
                    else{
                        // If true then IR0&IR1 are good. IR2 is bad.
                        if(ir0_ir2_range > 2.6){
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                }
            }
            // If true then IR1 is the closest to the object
            else if((toBefilterIR_1 < toBefilterIR_0)&&(toBefilterIR_1 < toBefilterIR_2)){
                // If true then all are good values. This is the case we want
                if((ir0_ir1_range < 2.6)&&(ir1_ir2_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // Something is out of range
                else{
                    // If true then IR0 is bad. IR1 is good
                    if(ir0_ir1_range > 2.6){
                        // If true then IR0&IR2 are bad. IR1 is good.
                        if(ir1_ir2_range > 2.6){
                            // Throw out everything because of interior point 01000
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                        // If gets here then IR1 &IR2 are good 01100
                        else{
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                    // If gets here then IR0&IR1 are good. check IR2
                    else{
                        // If true then IR0&IR1 are good. IR2 is bad.
                        if(ir1_ir2_range > 2.6){
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                }
            }
            // If true then IR2 is the closest to the object
            else if((toBefilterIR_2 < toBefilterIR_1)&&(toBefilterIR_2 < toBefilterIR_0)){
                // If true then all are good values. This is the case we want
                if((ir0_ir2_range < 2.6)&&(ir1_ir2_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // Something is out of range
                else{
                    // If true then IR0 is bad. IR2 is good
                    if(ir0_ir2_range > 2.6){
                        // If true then IR0&IR1 are bad. IR2 is good.
                        if(ir1_ir2_range > 2.6){
                            // Throw out everything because interior point 00100
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                        // If gets here then IR1&IR2 good 01100
                        else{
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                    // If gets here then IR0&IR2 are good. check IR1
                    else{
                        // If true then IR0&IR2 are good. IR1 is bad.
                        // Only keep IR0 because IR2 is interior point
                        if(ir1_ir2_range > 2.6){
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 200.0;
//                *filterIR_1 = 200.0;
//                *filterIR_2 = 200.0;
//                *filterIR_3 = 200.0;
//                *filterIR_4 = 200.0;
//            }
//            break;
            else{
                *filterIR_0_bool = *filterIR_0_bool;
                *filterIR_1_bool = *filterIR_1_bool;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = false;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = *filterIR_0;
                *filterIR_1 = *filterIR_1;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = 0.0;
                *filterIR_4 = 0.0;
            }
            break;
        }
        // 11101
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 29:{
            // If this is true then IR0 is the closest to object
            // If true then throw out IR4
            if((toBefilterIR_0 < toBefilterIR_1)&&(toBefilterIR_0 < toBefilterIR_2)&&(toBefilterIR_0 < toBefilterIR_4)){
                // This is the case we want to happen.
                if((ir0_ir1_range < 2.6)&&(ir0_ir2_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // If gets here something is out of range
                else{
                    // If true then IR0 is good. IR1 is bad
                    if(ir0_ir1_range > 2.6){
                        // If true then IR0 good. IR1&IR2 bad
                        // Only Keep IR0
                        *filterIR_0_bool = true;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = toBefilterIR_0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    // If gets here then IR0&IR1 good. check IR2
                    else{
                        // If true then IR0&IR1 good. IR2 bad
                        if(ir0_ir2_range > 2.6){
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                }
            }
            // If this is true then IR1 is the closest to object
            else if((toBefilterIR_1 < toBefilterIR_0)&&(toBefilterIR_1 < toBefilterIR_2)&&(toBefilterIR_1 < toBefilterIR_4)){
                // This is the case we want to happen.
                if((ir0_ir1_range < 2.6)&&(ir1_ir2_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // If gets here something is out of range
                else{
                    // If true then IR1 is good. IR0 is bad
                    if(ir0_ir1_range > 2.6){
                        // If true then IR1 good. IR0&IR2 bad
                        if(ir1_ir2_range > 2.6){
                            // If true then IR1 good. IR0&IR2&IR4 bad
                            if(ir1_ir4_range > 2.6){
                                // Throw out everything because interior points 01000
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then only keep IR4 because interior points 01001
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                        // If gets here then IR1&IR2 are good 01100
                        else{
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                    // If gets here then IR0&IR1 good. check IR2
                    else{
                        // If true then IR0&IR1 good. IR2 bad
                        if(ir0_ir2_range > 2.6){
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                }
            }
            // If this is true then IR2 is the closest to object
            else if((toBefilterIR_2 < toBefilterIR_1)&&(toBefilterIR_2 < toBefilterIR_0)&&(toBefilterIR_2 < toBefilterIR_4)){
                // This is the case we want to happen.
                if((ir1_ir2_range < 2.6)&&(ir0_ir2_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = false;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = 0.0;
                    *filterIR_4 = 0.0;
                }
                // If gets here something is out of range
                else{
                    // If true then IR2 is good. IR1 is bad
                    if(ir1_ir2_range > 2.6){
                        // If true then IR2 good. IR1&IR0 bad
                        if(ir0_ir2_range > 2.6){
                            // If true then IR2 good, IR1&IR0&IR4 bad. 
                            // Throw out everything because interior points 00100
                            if(ir2_ir4_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here only keep IR4 because interior points 00101
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                        // If gets here only keep IR0 because IR2 is interior point 10100
                        else{
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                    // If gets here then IR2&IR1 good. check IR0
                    else{
                        // If true then IR2&IR1 good. IR0 bad
                        // Throw out everything due to interior points 01100
                        if(ir0_ir2_range > 2.6){
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = true;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = toBefilterIR_2;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                    }
                }
            }
            // If this is true then IR4 is the closest to object
            // If this is true then throw out the other values
            else if((toBefilterIR_4 < toBefilterIR_1)&&(toBefilterIR_4 < toBefilterIR_2)&&(toBefilterIR_4 < toBefilterIR_0)){
                *filterIR_0_bool = false;
                *filterIR_1_bool = false;
                *filterIR_2_bool = false;
                *filterIR_3_bool = false;
                *filterIR_4_bool = true;

                /* Zeros because boolean is false */
                *filterIR_0 = 0.0;
                *filterIR_1 = 0.0;
                *filterIR_2 = 0.0;
                *filterIR_3 = 0.0;
                *filterIR_4 = toBefilterIR_4;
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 3000.0;
//                *filterIR_1 = 3000.0;
//                *filterIR_2 = 3000.0;
//                *filterIR_3 = 3000.0;
//                *filterIR_4 = 3000.0;
//            }
//            break;
            else{
                *filterIR_0_bool = *filterIR_0_bool;
                *filterIR_1_bool = *filterIR_1_bool;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = false;
                *filterIR_4_bool = *filterIR_4_bool;

                /* Zeros because boolean is false */
                *filterIR_0 = *filterIR_0;
                *filterIR_1 = *filterIR_1;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = 0.0;
                *filterIR_4 = *filterIR_4;
            }
            break;
        }
        // 11110
        // 2.6 range is chosen because it is the distance from nearest most corner
        // to outermost corner
        // If its not in that range throw out the furthest value(s)
        case 30:{
            // If true then IR0 is the closest to the object
            if((toBefilterIR_0 < toBefilterIR_1)&&(toBefilterIR_0 < toBefilterIR_2)&&(toBefilterIR_0 < toBefilterIR_3)){
                // This is the case we want to happen.
                if((ir0_ir1_range < 2.6)&&(ir0_ir2_range < 2.6)&&(ir0_ir3_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = 0.0;
                }
                // If get here then something is out of range
                else{
                    // If true then IR0 good. IR1 is bad
                    if(ir0_ir1_range > 2.6){
                        // If true then IR0 good and IR1&IR2 bad
                        *filterIR_0_bool = true;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* Zeros because boolean is false */
                        *filterIR_0 = toBefilterIR_0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    // If gets here then IR0&IR1 are good. Check IR2&IR3
                    else{
                        // If true then IR0&IR1 are good. IR2 is bad.
                        if(ir0_ir2_range > 2.6){
                            // If true then IR0&IR1 are good. IR2&IR3 are bad
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                        // If gets here then IR0&IR1&IR2 are good. Check IR 3
                        else{
                            // If true then IR0&IR1&IR2 are good. IR3 is bad
                            if(ir0_ir3_range > 2.6){
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                }
            }
            // If true then IR1 is the closest to the object
            else if((toBefilterIR_1 < toBefilterIR_0)&&(toBefilterIR_1 < toBefilterIR_2)&&(toBefilterIR_1 < toBefilterIR_3)){
                // This is the case we want to happen.
                if((ir0_ir1_range < 2.6)&&(ir1_ir2_range < 2.6)&&(ir1_ir3_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = 0.0;
                }
                // If get here then something is out of range
                else{
                    // If true then IR1 good. IR0 is bad
                    if(ir0_ir1_range > 2.6){
                        // If true then IR1 good and IR0&IR2 bad
                        if(ir1_ir2_range > 2.6){
                            // If true then IR1 good. IR0&IR2&IR3 bad
                            // Throw out everything because interior points 01000
                            *filterIR_0_bool = false;
                            *filterIR_1_bool = false;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = 0.0;
                            *filterIR_1 = 0.0;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                        // If gets here then IR1&IR2 good. IR0 is bad. Check IR3
                        else{
                            // If true then IR1&IR2 good. IR0&IR3 bad
                            // Keep IR1&IR2 interior points 01100
                            if(ir1_ir3_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR1&IR2&IR3 good. IR0 bad
                            // Keep only IR1&IR2&IR3
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                    // If gets here then IR0&IR1 are good. Check IR2&IR3
                    else{
                        // If true then IR0&IR1 are good. IR2 is bad.
                        if(ir1_ir2_range > 2.6){
                            // If true then IR0&IR1 are good. IR2&IR3 are bad
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                        // If gets here then IR0&IR1&IR2 are good. Check IR 3
                        else{
                            // If true then IR0&IR1&IR2 are good. IR3 is bad
                            if(ir1_ir3_range > 2.6){
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                }
            }
            // If true then IR2 is the closest to the object
            else if((toBefilterIR_2 < toBefilterIR_1)&&(toBefilterIR_2 < toBefilterIR_0)&&(toBefilterIR_2 < toBefilterIR_3)){
                // This is the case we want to happen.
                if((ir1_ir2_range < 2.6)&&(ir0_ir2_range < 2.6)&&(ir2_ir3_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = 0.0;
                }
                // If get here then something is out of range
                else{
                    // If true then IR2 good. IR0 is bad
                    if(ir0_ir2_range > 2.6){
                        // If true then IR2 good and IR1&IR2 bad
                        if(ir1_ir2_range > 2.6){
                            // If true then IR2 good. IR1&IR0&IR3 bad
                            if(ir2_ir3_range > 2.6){
                                // Throw out everything due to interior points 00100
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here IR2&IR3 good 00110
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                        // If gets here then IR1&IR2 good. IR0 is bad. Check IR3
                        else{
                            // If true then IR1&IR2 good. IR0&IR3 bad
                            // Keep IR1&IR2 because interior points 01100
                            if(ir2_ir3_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR1&IR2&IR3 good. IR0 bad
                            // Keep only IR1&IR2&IR3 good. IR0 bad.
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                    // If gets here then IR0&IR2 are good. Check IR1&IR3
                    else{
                        // If true then IR0&IR2 are good. IR1 is bad.
                        if(ir1_ir2_range > 2.6){
                            // If true then IR0&IR2 are good. IR1&IR3 are bad
                            if(ir2_ir3_range > 2.6){
                                // Keep only IR0 because interior points with IR2 10100
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR2&IR3&IR0 are good. 10110
                            // Only Keep IR2&IR3 because closer to object
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = toBefilterIR_0;
                                *filterIR_3 = toBefilterIR_0;
                                *filterIR_4 = 0.0;
                            }
                        }
                        // If gets here then IR0&IR1&IR2 are good. Check IR 3
                        else{
                            // If true then IR0&IR1&IR2 are good. IR3 is bad
                            if(ir2_ir3_range > 2.6){
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                }
            }
            // If true then IR3 is the closest to the object
            else if((toBefilterIR_3 < toBefilterIR_1)&&(toBefilterIR_3 < toBefilterIR_2)&&(toBefilterIR_3 < toBefilterIR_0)){
                // This is the case we want to happen.
                if((ir1_ir3_range < 2.6)&&(ir2_ir3_range < 2.6)&&(ir0_ir3_range < 2.6)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = false;

                    /* Zeros because boolean is false */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = 0.0;
                }
                // If get here then something is out of range
                else{
                    // If true then IR3 good. IR1 is bad
                    if(ir1_ir3_range > 2.6){
                        // If true then IR3 good. IR1&IR0 bad
                        if(ir0_ir3_range > 2.6){
                            // Throw out everything because interior points 00010
                            if(ir2_ir3_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here IR2&IR3 are good. 00110
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                        // If gets here IR0&IR3 good. IR1 bad. Check IR2
                        else{
                            // If true IR0&IR3 are good. IR1&IR2 bad
                            if(ir2_ir3_range > 2.6){
                                // Only keep IR0 because interior point 10010
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR0&IR3&IR2 are good. 10110
                            // Only keep IR2&IR3
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                    // If gets here then IR3&IR1 are good. Check IR2&IR0
                    else{
                        // If true then IR3&IR1 are good. IR2 is bad.
                        if(ir2_ir3_range > 2.6){
                            // If true then IR3&IR1 are good. IR2&IR0 are bad
                            // Throw out everything because interior points 01010
                            if(ir0_ir3_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR0&IR1&IR3 are good. IR2 is bad.
                            // Only keep IR0&IR1 because IR3 is interior points 11010
                            else{
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                        }
                        // If gets here then IR3&IR1&IR2 are good. Check IR0
                        else{
                            // If true then IR3&IR1&IR2 are good. IR0 is bad
                            if(ir0_ir3_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = 0.0;
                            }
                        }
                    }
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 4000.0;
//                *filterIR_1 = 4000.0;
//                *filterIR_2 = 4000.0;
//                *filterIR_3 = 4000.0;
//                *filterIR_4 = 4000.0;
//            }
//            break;
            else{
                *filterIR_0_bool = *filterIR_0_bool;
                *filterIR_1_bool = *filterIR_1_bool;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = *filterIR_3_bool;
                *filterIR_4_bool = false;

                /* Zeros because boolean is false */
                *filterIR_0 = *filterIR_0;
                *filterIR_1 = *filterIR_1;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = *filterIR_3;
                *filterIR_4 = 0.0;
            }
            break;
        }
        // 11111
        case 31:{
            // If true then IR0 is the closest to the object
            if((toBefilterIR_0 < toBefilterIR_1)&&(toBefilterIR_0 < toBefilterIR_2)&&(toBefilterIR_0 < toBefilterIR_3)&&(toBefilterIR_0 < toBefilterIR_4)){
                // This is the case if its an obstacle
                if((ir0_ir1_range < 5.4)&&(ir0_ir2_range < 5.4)&&(ir0_ir3_range < 5.4)&&(ir0_ir4_range < 5.4)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* All are true therefore should be equal to corresponding IR values */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here something is out of range
                else{
                    // If true then IR0 good. IR1 bad
                    if(ir0_ir1_range > 2.6){
                        // If true then IR0 good. IR1&IR2 bad
                        *filterIR_0_bool = true;
                        *filterIR_1_bool = false;
                        *filterIR_2_bool = false;
                        *filterIR_3_bool = false;
                        *filterIR_4_bool = false;

                        /* All are true therefore should be equal to corresponding IR values */
                        *filterIR_0 = toBefilterIR_0;
                        *filterIR_1 = 0.0;
                        *filterIR_2 = 0.0;
                        *filterIR_3 = 0.0;
                        *filterIR_4 = 0.0;
                    }
                    // If gets here then IR0&IR1 are good. Check IR2&IR3&IR4
                    else{
                        // If true then IR0&IR1 are good. IR2 is bad.
                        if(ir0_ir2_range > 2.6){
                            // If true then IR0&IR1 good. IR2&IR3 bad
                            // If true then IR0&IR1 good. IR2&IR3&IR4 bad.
                            // Keep only IR0&IR1
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* All are true therefore should be equal to corresponding IR values */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                        // If gets here then IR0&IR1&IR2 good. Check IR3&IR4
                        else{
                            // If true then IR0&IR1&IR2 good. IR3 bad
                            if(ir0_ir3_range > 2.6){
                                // If true then IR0&IR1&IR2 good. IR3&IR4 bad.
                                // Keep IR0&IR1&IR2
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* All are true therefore should be equal to corresponding IR values */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR0&IR1&IR2&IR3 good. Check IR4
                            else{
                                // If true then IR0&IR1&IR2&IR3 good. IR4 bad
                                if(ir0_ir4_range > 2.6){
                                    *filterIR_0_bool = true;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* All are true therefore should be equal to corresponding IR values */
                                    *filterIR_0 = toBefilterIR_0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                            }
                        }
                    }
                }
            }
            // If true then IR1 is the closest to the object
            else if((toBefilterIR_1 < toBefilterIR_0)&&(toBefilterIR_1 < toBefilterIR_2)&&(toBefilterIR_1 < toBefilterIR_3)&&(toBefilterIR_1 < toBefilterIR_4)){
                // This is the case if its an obstacle
                if((ir0_ir1_range < 5.4)&&(ir1_ir2_range < 5.4)&&(ir1_ir3_range < 5.4)&&(ir1_ir4_range < 5.4)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* All are true therefore should be equal to corresponding IR values */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here something is out of range
                else{
                    // If true then IR1 good. IR0 bad
                    if(ir0_ir1_range > 2.6){
                        // If true then IR1 good. IR0&IR2 bad
                        if(ir1_ir2_range > 2.6){
                            // If true then IR1 good. IR0&IR2&IR3 bad
                            if(ir1_ir3_range > 2.6){
                                // If true then IR1 good. IR0&IR2&IR3&IR4 bad.
                                // Throw out everything because interior points 01000
                                if(ir1_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = false;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = 0.0;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR1&IR4 good.
                                // Keep only IR4 because interior points 01001 CHECK HERE EDIT
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = false;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = 0.0;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                            // If gets here then IR1&IR3 good. IR0&IR2 bad. Check IR4
                            else{
                                // If true then IR1&IR3 good. IR0&IR2&IR4 bad
                                // Throw out everything because interior points 01010
                                if(ir1_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = false;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = 0.0;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR1&IR3&IR4 good. IR0&IR2 bad.
                                // Keep only IR3&IR4 because interior points 01011 CHECK HERE EDIT
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                        }
                        // If gets here then IR1&IR2 good. IR0 bad. Check IR3&IR4
                        else{
                            // If true then IR1&IR2 good. IR3&IR0 bad
                            if(ir1_ir3_range > 2.6){
                                // If true then IR1&IR2 good. IR0&IR3&IR4 bad
                                // Keep only IR2&IR1 because interior points 01100
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR1&IR2&IR3 good. IR0 bad. Check IR4
                            else{
                                // If true then IR1&IR2&IR3 good. IR0&IR4 bad.
                                // Keep only IR1&IR2&IR3
                                if(ir1_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR1&IR2&IR3&IR4 good. IR0 bad
                                // Only keep IR1&IR2&IR3&IR4
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                        }
                    }
                    // If gets here then IR0&IR1 are good. Check IR2&IR3&IR4
                    else{
                        // If true then IR0&IR1 are good. IR2 is bad.
                        if(ir1_ir2_range > 2.6){
                            // If true then IR0&IR1 good. IR2&IR3 bad
                            *filterIR_0_bool = true;
                            *filterIR_1_bool = true;
                            *filterIR_2_bool = false;
                            *filterIR_3_bool = false;
                            *filterIR_4_bool = false;

                            /* Zeros because boolean is false */
                            *filterIR_0 = toBefilterIR_0;
                            *filterIR_1 = toBefilterIR_1;
                            *filterIR_2 = 0.0;
                            *filterIR_3 = 0.0;
                            *filterIR_4 = 0.0;
                        }
                        // If gets here then IR0&IR1&IR2 good. Check IR3&IR4
                        else{
                            // If true then IR0&IR1&IR2 good. IR3 bad
                            if(ir1_ir3_range > 2.6){
                                // If true then IR0&IR1&IR2 good. IR3&IR4 bad.
                                // Keep IR0&IR1&IR2
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR0&IR1&IR2&IR3 good. Check IR4
                            else{
                                // If true then IR0&IR1&IR2&IR3 good. IR4 bad
                                if(ir1_ir4_range > 2.6){
                                    *filterIR_0_bool = true;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = toBefilterIR_0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                            }
                        }
                    }
                }
            }
            // If true then IR2 is the closest to the object
            else if((toBefilterIR_2 < toBefilterIR_1)&&(toBefilterIR_2 < toBefilterIR_0)&&(toBefilterIR_2 < toBefilterIR_3)&&(toBefilterIR_2 < toBefilterIR_4)){
                // This is the case if its an obstacle
                if((ir0_ir2_range < 5.4)&&(ir1_ir2_range < 5.4)&&(ir2_ir3_range < 5.4)&&(ir2_ir4_range < 5.4)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* All are true therefore should be equal to corresponding IR values */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here something is out of range
                else{
                    // If true then IR2 good. IR0 bad
                    if(ir0_ir2_range > 2.6){
                        // If true then IR2 good. IR1&IR0 bad
                        if(ir1_ir2_range > 2.6){
                            // If true then IR2 good. IR1&IR0&IR3 bad
                            if(ir2_ir3_range > 2.6){
                                // If true then IR2 good. IR1&IR0&IR3&IR4 bad.
                                // Throw out everything because interior points 00100
                                if(ir2_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = false;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = 0.0;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR2&IR4 good.
                                // Keep only IR4 because interior points 00101 CHECK HERE EDIT
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = false;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = 0.0;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                            // If gets here then IR2&IR3 good. IR1&IR0 bad. Check IR4
                            else{
                                // If true then IR2&IR3 good. IR1&IR0&IR4 bad
                                // Keep only IR2&IR3 because interior points 00110
                                if(ir2_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR2&IR3&IR4 good. IR1&IR0 bad.
                                // Keep only IR2&IR3&IR4 because it is closer
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                        }
                        // If gets here then IR1&IR2 good. IR0 bad. Check IR3&IR4
                        else{
                            // If true then IR1&IR2 good. IR3&IR0 bad
                            if(ir2_ir3_range > 2.6){
                                // If true then IR1&IR2 good. IR0&IR3&IR4 bad
                                // Keep only IR1&IR2 because interior points 01100 and are closer to object
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR1&IR2&IR3 good. IR0 bad. Check IR4
                            else{
                                // If true then IR1&IR2&IR3 good. IR0&IR4 bad.
                                // Keep only IR1&IR2&IR3
                                if(ir2_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR1&IR2&IR3&IR4 good. IR0 bad
                                // Only keep IR1&IR2&IR3&IR4
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                        }
                    }
                    // If gets here then IR0&IR2 are good. Check IR1&IR3&IR4
                    else{
                        // If true then IR0&IR2 are good. IR1 is bad.
                        if(ir1_ir2_range > 2.6){
                            // If true then IR0&IR2 good. IR0&IR3 bad
                            if(ir2_ir3_range > 2.6){
                                // If true then IR0&IR2 good. IR1&IR3&IR4 bad.
                                // Keep only IR0 because interior points 10100
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR0&IR2&IR3 good. IR1 bad. Check IR4
                            else{
                                // If true then IR0&IR2&IR3 good. IR1&IR4 bad.
                                // Only keep IR2&IR3 because interior points 10110 and are closer to object
                                if(ir2_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR0&IR2&IR3&IR4 good. IR1 bad.
                                // Only Keep IR2&IR3&IR4 because closest to object
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                        }
                        // If gets here then IR0&IR1&IR2 good. Check IR3&IR4
                        else{
                            // If true then IR0&IR1&IR2 good. IR3 bad
                            if(ir2_ir3_range > 2.6){
                                // If true then IR0&IR1&IR2 good. IR3&IR4 bad.
                                // Keep IR0&IR1&IR2
                                *filterIR_0_bool = true;
                                *filterIR_1_bool = true;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = toBefilterIR_0;
                                *filterIR_1 = toBefilterIR_1;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR0&IR1&IR2&IR3 good. Check IR4
                            else{
                                // If true then IR0&IR1&IR2&IR3 good. IR4 bad
                                if(ir2_ir4_range > 2.6){
                                    *filterIR_0_bool = true;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = toBefilterIR_0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                            }
                        }
                    }
                }
            }
            // If true then IR3 is the closest to the object
            else if((toBefilterIR_3 < toBefilterIR_1)&&(toBefilterIR_3 < toBefilterIR_2)&&(toBefilterIR_3 < toBefilterIR_0)&&(toBefilterIR_3 < toBefilterIR_4)){
                // This is the case if its an obstacle
                if((ir1_ir3_range < 2.6)&&(ir2_ir3_range < 5.4)&&(ir0_ir3_range < 5.4)&&(ir3_ir4_range < 5.4)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* All are true therefore should be equal to corresponding IR values */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here something is out of range
                else{
                    // If true then IR3 good. IR0 bad
                    if(ir0_ir3_range > 2.6){
                        // If true then IR3 good. IR0&IR2 bad
                        if(ir2_ir3_range > 2.6){
                            // If true then IR3 good. IR1&IR2&IR3&IR4 bad.
                            // Throw out everything because interior points 00010
                            if(ir3_ir4_range > 2.6){
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = false;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = 0.0;
                            }
                            // If gets here then IR3&IR4 good.
                            // Keep only IR3&IR4 because it is closer to object
                            else{
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                        // If gets here then IR3&IR2 good. IR0 bad. Check IR1&IR4
                        else{
                            // If true then IR3&IR2 good. IR1&IR0 bad
                            if(ir1_ir3_range > 2.6){
                                // If true then IR3&IR2 good. IR1&IR0&IR4 bad
                                // Keep only IR2&IR3 because interior points 00110 and are closer to object
                                if(ir3_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR3&IR2&IR4 good.
                                // Keep only IR2&IR3&IR4 because it is closest to object
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                            // If gets here then IR1&IR2&IR3 good. IR0 bad. Check IR4
                            else{
                                // If true then IR1&IR2&IR3 good. IR0&IR4 bad.
                                // Keep only IR1&IR2&IR3 because its closer to object
                                if(ir3_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR1&IR2&IR3&IR4 good. IR0 bad
                                // Only keep IR1&IR2&IR3&IR4 because it is closest to object
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                        }
                    }
                    // If gets here then IR0&IR3 are good. Check IR2&IR1&IR4
                    else{
                        // If true then IR0&IR3 are good. IR2 is bad.
                        if(ir2_ir3_range > 2.6){
                            // If true then IR0&IR3 good. IR2&IR1 bad
                            if(ir1_ir3_range > 2.6){
                                // If true then IR0&IR3 good. IR2&IR1&IR4 bad.
                                // Keep only IR0 because interior points 10010 CHECK HERE EDIT
                                if(ir3_ir4_range > 2.6){
                                    *filterIR_0_bool = true;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = false;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = toBefilterIR_0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = 0.0;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR0&IR3&IR4 are good.
                                // Keep only IR3&IR4 because they are closest to object
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                            // If gets here then IR0&IR1&IR3 good. IR2 bad. Check IR4
                            else{
                                // If true then IR0&IR1&IR3 good. IR2&IR4 bad.
                                // Only keep IR0&IR1 because interior points 11010
                                if(ir3_ir4_range > 2.6){
                                    *filterIR_0_bool = true;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = false;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = toBefilterIR_0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = 0.0;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here then IR0&IR1&IR3&IR4 good. IR2 bad.
                                // Only Keep IR3&IR4 because closest to object
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = false;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = 0.0;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                        }
                        // If gets here then IR0&IR3&IR2 good. Check IR1&IR4
                        else{
                            // If true then IR0&IR3&IR2 good. IR1 bad
                            if(ir1_ir3_range > 2.6){
                                // If true then IR0&IR3&IR2 good. IR1&IR4 bad.
                                // Keep IR2&IR3 because interior points 10110 and closer to object
                                if(ir3_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                                // If gets here IR0&IR3&IR2&IR4 good. IR1 bad.
                                // Only keep IR2&IR3&IR4 because they are closer to object
                                else{
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = false;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = 0.0;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                            // If gets here then IR0&IR1&IR2&IR3 good. Check IR4
                            else{
                                // If true then IR0&IR1&IR2&IR3 good. IR4 bad
                                if(ir3_ir4_range > 2.6){
                                    *filterIR_0_bool = true;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = false;

                                    /* All are true therefore should be equal to corresponding IR values */
                                    *filterIR_0 = toBefilterIR_0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = 0.0;
                                }
                            }
                        }
                    }
                }
            }
            // If true then IR4 is the closest to the object
            else if((toBefilterIR_4 < toBefilterIR_1)&&(toBefilterIR_4 < toBefilterIR_2)&&(toBefilterIR_4 < toBefilterIR_3)&&(toBefilterIR_4 < toBefilterIR_0)){
                // This is the case if its an obstacle
                if((ir0_ir4_range < 5.4)&&(ir2_ir4_range < 5.4)&&(ir3_ir4_range < 5.4)&&(ir1_ir4_range < 5.4)){
                    *filterIR_0_bool = true;
                    *filterIR_1_bool = true;
                    *filterIR_2_bool = true;
                    *filterIR_3_bool = true;
                    *filterIR_4_bool = true;

                    /* All are true therefore should be equal to corresponding IR values */
                    *filterIR_0 = toBefilterIR_0;
                    *filterIR_1 = toBefilterIR_1;
                    *filterIR_2 = toBefilterIR_2;
                    *filterIR_3 = toBefilterIR_3;
                    *filterIR_4 = toBefilterIR_4;
                }
                // If gets here something is out of range
                else{
                    // If true then IR4 good. IR1 bad
                    if(ir1_ir4_range > 2.6){
                        // If true then IR4 good. IR1&IR2 bad
                        if(ir2_ir4_range > 2.6){
                            // If true then IR4 good. IR1&IR2&IR3 bad
                            if(ir3_ir4_range > 2.6){
                                // If true then IR4 good. IR1&IR2&IR3&IR0 bad.
                                // Keep only IR4
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = toBefilterIR_4;
                            }
                            // If gets here then IR3&IR4 good. IR1&IR2 bad. Check IR0
                            else{
                                // If true then IR3&IR4 good. IR1&IR2&IR0 bad
                                // Only keep IR3&IR4 because they are closest to object
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                        // If gets here then IR4&IR2 good. IR1 bad. Check IR3&IR0
                        else{
                            // If true then IR4&IR2 good. IR3&IR1 bad
                            if(ir3_ir4_range > 2.6){
                                // If true then IR4&IR2 good. IR1&IR3&IR0 bad
                                // Keep only IR4 because interior points 00101
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = toBefilterIR_4;
                            }
                            // If gets here then IR4&IR2&IR3 good. IR1 bad. Check IR0
                            else{
                                // If true then IR4&IR2&IR3 good. IR1&IR0 bad.
                                // Keep only IR2&IR3&IR4
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = true;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = toBefilterIR_2;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                    }
                    // If gets here then IR4&IR1 are good. Check IR2&IR3&IR0
                    else{
                        // If true then IR4&IR1 are good. IR2 is bad.
                        if(ir2_ir4_range > 2.6){
                            // If true then IR4&IR1 good. IR2&IR3 bad
                            if(ir3_ir4_range > 2.6){
                                // If true then IR4&IR1 good. IR2&IR3&IR0 bad.
                                // Keep only IR4 because interior points 01001
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = toBefilterIR_4;
                            }
                            // If gets here then IR4&IR1&IR3 good. IR2 bad. Check IR0
                            else{
                                // If true then IR4&IR1&IR3 good. IR2&IR0 bad.
                                // Only keep IR3&IR4 because interior points 01011
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = true;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = toBefilterIR_3;
                                *filterIR_4 = toBefilterIR_4;
                            }
                        }
                        // If gets here then IR4&IR1&IR2 good. Check IR3&IR0
                        else{
                            // If true then IR4&IR1&IR2 good. IR3 bad
                            if(ir3_ir4_range > 2.6){
                                // If true then IR4&IR1&IR2 good. IR3&IR0 bad.
                                // Keep IR4 because interior points 01101
                                *filterIR_0_bool = false;
                                *filterIR_1_bool = false;
                                *filterIR_2_bool = false;
                                *filterIR_3_bool = false;
                                *filterIR_4_bool = true;

                                /* Zeros because boolean is false */
                                *filterIR_0 = 0.0;
                                *filterIR_1 = 0.0;
                                *filterIR_2 = 0.0;
                                *filterIR_3 = 0.0;
                                *filterIR_4 = toBefilterIR_4;
                            }
                            // If gets here then IR4&IR1&IR2&IR3 good. Check IR0
                            else{
                                // If true then IR1&IR2&IR3&IR4 good. IR0 bad
                                if(ir0_ir4_range > 2.6){
                                    *filterIR_0_bool = false;
                                    *filterIR_1_bool = true;
                                    *filterIR_2_bool = true;
                                    *filterIR_3_bool = true;
                                    *filterIR_4_bool = true;

                                    /* Zeros because boolean is false */
                                    *filterIR_0 = 0.0;
                                    *filterIR_1 = toBefilterIR_1;
                                    *filterIR_2 = toBefilterIR_2;
                                    *filterIR_3 = toBefilterIR_3;
                                    *filterIR_4 = toBefilterIR_4;
                                }
                            }
                        }
                    }
                }
            }
//            /* This is for ERROR CHECKING <<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>*/
//            else{
//                *filterIR_0_bool = true;
//                *filterIR_1_bool = true;
//                *filterIR_2_bool = true;
//                *filterIR_3_bool = true;
//                *filterIR_4_bool = true;
//
//                /* Zeros because boolean is false */
//                *filterIR_0 = 5000.0;
//                *filterIR_1 = 5000.0;
//                *filterIR_2 = 5000.0;
//                *filterIR_3 = 5000.0;
//                *filterIR_4 = 5000.0;
//            }
//            break;
            else{
                *filterIR_0_bool = *filterIR_0_bool;
                *filterIR_1_bool = *filterIR_1_bool;
                *filterIR_2_bool = *filterIR_2_bool;
                *filterIR_3_bool = *filterIR_3_bool;
                *filterIR_4_bool = *filterIR_4_bool;

                /* Zeros because boolean is false */
                *filterIR_0 = *filterIR_0;
                *filterIR_1 = *filterIR_1;
                *filterIR_2 = *filterIR_2;
                *filterIR_3 = *filterIR_3;
                *filterIR_4 = *filterIR_4;
            }
            break;
        }
        default:{
            *filterIR_0_bool = true;
            *filterIR_1_bool = true;
            *filterIR_2_bool = true;
            *filterIR_3_bool = true;
            *filterIR_4_bool = true;
            
            /* For Testing */
            *filterIR_0 = 100.0;
            *filterIR_1 = 100.0;
            *filterIR_2 = 100.0;
            *filterIR_3 = 100.0;
            *filterIR_4 = 100.0;
            break;
        }
    }
}


/* This subroutine handles the target alignment data             */
/* Which pertains to scanning AN0, AN1, AN2 on the ADC           */
/* The ADC is in scan mode with autosampling but it is enabled   */
/* Only when TMR2 fires which is every 50ms                      */
/* TMR2 is turned on at end of previous state as well as opening */
/* the ADC for operation.                                        */
/* Inputs:  N/A                                                  */
/* Outputs: Boolean that returns true once the target is aligned */
/*          Updates the Message Controller Thread Queue to let   */
/* PathFinder have updated values of the location of the target  */
bool targetAlignmentSubr(){
    MessageObj obj;
    TargetAlignment_Type distanceIR;
    float angle;
    float distance_away;
    
    float toBefilterIR_0, toBefilterIR_1, toBefilterIR_2, toBefilterIR_3, toBefilterIR_4;
    bool toBefilterIR_0_bool, toBefilterIR_1_bool, toBefilterIR_2_bool, toBefilterIR_3_bool, toBefilterIR_4_bool;
    
    float filterIR_0, filterIR_1, filterIR_2, filterIR_3, filterIR_4;
    bool filterIR_0_bool, filterIR_1_bool, filterIR_2_bool, filterIR_3_bool, filterIR_4_bool;
    memset(&distanceIR, 0, sizeof(TargetAlignment_Type));
    memset(&obj, 0, sizeof(MessageObj));
    obj.Type = UPDATE;
    obj.Update.Type = ALIGNMENTDATA;
    dbgOutputLoc(BEFORE_WHILE_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
    while(1){
        dbgOutputLoc(BEFORE_RECEIVE_FROM_Q_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
        if(xQueueReceive(_queue, &distanceIR, portMAX_DELAY)){
            // Storing the converted cm value in the to be filtered variables
            toBefilterIR_0_bool = cmValChecker(convertTocmIR(distanceIR.IR_0));
            toBefilterIR_1_bool = cmValChecker(convertTocmIR(distanceIR.IR_1));
            toBefilterIR_2_bool = cmValChecker(convertTocmIR(distanceIR.IR_2));
            toBefilterIR_3_bool = cmValChecker(convertTocmIR(distanceIR.IR_3));
            toBefilterIR_4_bool = cmValChecker(convertTocmIR(distanceIR.IR_4));
            
            if(toBefilterIR_0_bool == true){
                toBefilterIR_0 = convertTocmIR(distanceIR.IR_0);
            }
            if(toBefilterIR_1_bool == true){
                toBefilterIR_1 = convertTocmIR(distanceIR.IR_1);
            }
            if(toBefilterIR_2_bool == true){
                toBefilterIR_2 = convertTocmIR(distanceIR.IR_2);
            }
            if(toBefilterIR_3_bool == true){
                toBefilterIR_3 = convertTocmIR(distanceIR.IR_3);
            }
            if(toBefilterIR_4_bool == true){
                toBefilterIR_4 = convertTocmIR(distanceIR.IR_4);
            }
            filterIRData(toBefilterIR_0_bool, toBefilterIR_1_bool, toBefilterIR_2_bool, 
                         toBefilterIR_3_bool, toBefilterIR_4_bool, toBefilterIR_0, 
                         toBefilterIR_1, toBefilterIR_2, toBefilterIR_3, toBefilterIR_4,
                         &filterIR_0_bool, &filterIR_1_bool, &filterIR_2_bool, 
                         &filterIR_3_bool, &filterIR_4_bool, &filterIR_0, 
                         &filterIR_1, &filterIR_2, &filterIR_3, &filterIR_4);
            obj.Update.Data.alignmentData.IR_0_bool = filterIR_0_bool;
            obj.Update.Data.alignmentData.IR_1_bool = filterIR_1_bool;
            obj.Update.Data.alignmentData.IR_2_bool = filterIR_2_bool;
            obj.Update.Data.alignmentData.IR_3_bool = filterIR_3_bool;
            obj.Update.Data.alignmentData.IR_4_bool = filterIR_4_bool;
            
            if(obj.Update.Data.alignmentData.IR_0_bool == true){
                obj.Update.Data.alignmentData.IR_0 = filterIR_0;
            }
            if(obj.Update.Data.alignmentData.IR_1_bool == true){
                obj.Update.Data.alignmentData.IR_1 = filterIR_1;
            }
            if(obj.Update.Data.alignmentData.IR_2_bool == true){
                obj.Update.Data.alignmentData.IR_2 = filterIR_2;
            }
            if(obj.Update.Data.alignmentData.IR_3_bool == true){
                obj.Update.Data.alignmentData.IR_3 = filterIR_3;
            }
            if(obj.Update.Data.alignmentData.IR_4_bool == true){
                obj.Update.Data.alignmentData.IR_4 = filterIR_4;
            }
            
            /* NEW CODE <END>*/
            computation_threadData.alignmentResult = determineAlignment(obj.Update.Data.alignmentData.IR_0_bool,
                                                                        obj.Update.Data.alignmentData.IR_1_bool,
                                                                        obj.Update.Data.alignmentData.IR_2_bool,
                                                                        obj.Update.Data.alignmentData.IR_3_bool,
                                                                        obj.Update.Data.alignmentData.IR_4_bool,
                                                                        obj.Update.Data.alignmentData.IR_0,
                                                                        obj.Update.Data.alignmentData.IR_1,
                                                                        obj.Update.Data.alignmentData.IR_2,
                                                                        obj.Update.Data.alignmentData.IR_3,
                                                                        obj.Update.Data.alignmentData.IR_4,
                                                                        &distance_away, &angle);
            switch(computation_threadData.alignmentResult)
            {
                case NOT_ALIGNED:{
                    obj.Update.Data.alignmentData.internalAlignment = NOT_ALIGNED;
                    obj.Update.Data.alignmentData.Target_Angle = angle;
                    obj.Update.Data.alignmentData.Target_Distance = distance_away;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                }
                case SLIGHT_RIGHT:{
                    obj.Update.Data.alignmentData.internalAlignment = SLIGHT_RIGHT;
                    obj.Update.Data.alignmentData.Target_Angle = angle;
                    obj.Update.Data.alignmentData.Target_Distance = distance_away;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                }
                case SLIGHT_LEFT:{
                    obj.Update.Data.alignmentData.internalAlignment = SLIGHT_LEFT;
                    obj.Update.Data.alignmentData.Target_Angle = angle;
                    obj.Update.Data.alignmentData.Target_Distance = distance_away;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                }
                case MAJOR_RIGHT:{
                    obj.Update.Data.alignmentData.internalAlignment = MAJOR_RIGHT;
                    obj.Update.Data.alignmentData.Target_Angle = angle;
                    obj.Update.Data.alignmentData.Target_Distance = distance_away;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                }
                case MAJOR_LEFT:{
                    obj.Update.Data.alignmentData.internalAlignment = MAJOR_LEFT;
                    obj.Update.Data.alignmentData.Target_Angle = angle;
                    obj.Update.Data.alignmentData.Target_Distance = distance_away;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                }
                case ALIGNED_NOT_CLOSE:{
                    obj.Update.Data.alignmentData.internalAlignment = ALIGNED_NOT_CLOSE;
                    obj.Update.Data.alignmentData.Target_Angle = angle;
                    obj.Update.Data.alignmentData.Target_Distance = distance_away;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                }
                case ALIGNED:{
                    obj.Update.Data.alignmentData.internalAlignment = ALIGNED;
                    obj.Update.Data.alignmentData.Target_Angle = angle;
                    obj.Update.Data.alignmentData.Target_Distance = distance_away;
                    obj.Update.Data.alignmentData.Grabber_Aligned = true;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                }
                case OBSTACLE:{
                    obj.Update.Data.alignmentData.internalAlignment = OBSTACLE;
                    obj.Update.Data.alignmentData.Target_Angle = angle;
                    obj.Update.Data.alignmentData.Target_Distance = distance_away;
                    obj.Update.Data.alignmentData.Grabber_Aligned = false;
                    // Sending to Tx Thread Q
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
                    break;
                }
                default:
                    break;
            }
        }
        dbgOutputLoc(AFTER_RECEIVE_FROM_Q_TARGET_ALIGNMENT_SUBR_COMPUTATION_THREAD);
        return obj.Update.Data.alignmentData.Grabber_Aligned; // returning if its aligned
    }
}

/* This subroutine determines if the alignment values are within range    */
/* to acquire the target and proceed to the next state                    */
/* Inputs:  All Five IR values and booleans that determine if the         */
/* values are within the correct reading range of the sensors             */
/* Also inputs hypotenuseDistance and thetaDegree variables to store data */
/* to send to the PathFinder                                              */
/* Outputs: Returns the correct value in degree as well as the distance   */
/* away the target is.  It also returns a string that tells if it is a    */
/* obstacle or a general idea of which way to rotate.                     */
int determineAlignment(bool ir_0_bool, bool ir_1_bool, bool ir_2_bool, bool ir_3_bool, bool ir_4_bool,
                       float ir_0, float ir_1, float ir_2, float ir_3, float ir_4, float *hypotenuseDistance,
                       float *thetaDegree){
    float thetaRadian = 0.0;
    float temphyp = 0.0;
    float topMidwayRatio = 0.0;
    float midwayRatio = 0.0;
    float adjacentSide = 0.0;
    float oppositeSide = 0.0;
    float ir0_ir1_range;
    float ir1_ir2_range;
    float ir2_ir3_range;
    float ir3_ir4_range;
    float ir0_ir2_range;
    float ir0_ir3_range;
    float ir0_ir4_range;
    float ir1_ir3_range;
    float ir1_ir4_range;
    float ir2_ir4_range;
    ir0_ir1_range = fabs(ir_0 - ir_1);
    ir1_ir2_range = fabs(ir_1 - ir_2);
    ir2_ir3_range = fabs(ir_2 - ir_3);
    ir3_ir4_range = fabs(ir_3 - ir_4);
    ir0_ir2_range = fabs(ir_0 - ir_2);
    ir0_ir3_range = fabs(ir_0 - ir_3);
    ir0_ir4_range = fabs(ir_0 - ir_4);
    ir1_ir3_range = fabs(ir_1 - ir_3);
    ir1_ir4_range = fabs(ir_1 - ir_4);
    ir2_ir4_range = fabs(ir_2 - ir_4);

    if(ir_0_bool && ir_1_bool && ir_2_bool && ir_3_bool && ir_4_bool){
        adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
        oppositeSide = 0.0;
        temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
        *hypotenuseDistance = temphyp;
        *thetaDegree = 0.0;
        return 7; // OBSTACLE
    }
    // This case is potentially a corner reading from a target or its an obstacle
    // Need to handle both of these cases
    else if(ir_0_bool && ir_1_bool && ir_2_bool && ir_3_bool){
        // Get the closest reading and align to it
        // If true them IR0 is closest to object
        if((ir_0 < ir_1)&&(ir_0 < ir_2)&&(ir_0 < ir_3)){
            midwayRatio = (ir_1 + ir_2) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 3; // MAJOR RIGHT
        }
        // If true them IR1 or IR3 is closest to object
        else if(((ir_1 < ir_0)&&(ir_1 < ir_2)&&(ir_1 < ir_3)) || ((ir_3 < ir_1)&&(ir_3 < ir_2)&&(ir_3 < ir_0))){
            midwayRatio = (ir_1 + ir_2) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 1; // SLIGHT RIGHT
        }
        // If true them IR2 is closest to object
        else if((ir_2 < ir_1)&&(ir_2 < ir_0)&&(ir_2 < ir_3)){
            if((ir_2 < 4.0)&&(ir1_ir2_range < 2.0)&&(ir2_ir3_range < 2.0)&&(ir0_ir2_range < 2.0)){ // ALIGNED
                adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                oppositeSide = 0.0;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                *thetaDegree = 0.0;
                return 6; // ALIGNED
            }
            else if(ir_2 < 4.0){
                adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                oppositeSide = 0.0;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                *thetaDegree = 0.0;
                return 5; // ALIGNED NOT CLOSE
            }
            else{
                midwayRatio = (ir_1 + ir_2) / 2.0;
                adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
                oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                thetaRadian = sin(oppositeSide / temphyp);
                *thetaDegree = thetaRadian * (180.0/M_PI);
                return 1; // SLIGHT RIGHT
            }
        }
        // This is for Error Checking <<<<<<<<<<<<<>>>>>>>>>>>>>>>>>
        else{
            *hypotenuseDistance = 600.0;
            *thetaDegree = 600.0;
            return 7; // OBSTACLE
        }
    }
    // This case is potentially a corner reading from a target or its an obstacle
    // Need to handle both of these cases
    else if(ir_1_bool && ir_2_bool && ir_3_bool && ir_4_bool){
        // Get the closest reading and align to it
        // If true them IR1 is closest to object
        if((ir_2 < ir_1)&&(ir_2 < ir_3)&&(ir_2 < ir_4)){
            if((ir_2 < 4.0)&&(ir1_ir2_range < 2.0)&&(ir2_ir3_range < 2.0)&&(ir2_ir4_range < 2.0)){ // ALIGNED
                adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                oppositeSide = 0.0;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                *thetaDegree = 0.0;
                return 6; // ALIGNED
            }
            else if(ir_2 < 4.0){
                adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                oppositeSide = 0.0;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                *thetaDegree = 0.0;
                return 5; // ALIGNED NOT CLOSE
            }
            else{
                midwayRatio = (ir_3 + ir_2) / 2.0;
                adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
                oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                thetaRadian = sin(oppositeSide / temphyp);
                thetaRadian = -1.0 * thetaRadian;
                *thetaDegree = thetaRadian * (180.0/M_PI);
                return 2; // SLIGHT LEFT
            }
        }
        // If true them IR3 or IR1 is closest to object
        else if(((ir_3 < ir_1)&&(ir_3 < ir_2)&&(ir_3 < ir_4)) || ((ir_1 < ir_2)&&(ir_1 < ir_3)&&(ir_1 < ir_4))){
            midwayRatio = (ir_3 + ir_2) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            thetaRadian = -1.0 * thetaRadian;
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 2; // SLIGHT LEFT
        }
        // If true them IR4 is closest to object
        else if((ir_4 < ir_1)&&(ir_4 < ir_2)&&(ir_4 < ir_3)){
            midwayRatio = (ir_3 + ir_2) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            thetaRadian = -1.0 * thetaRadian;
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 4; // MAJOR LEFT
        }
        // This is for Error Checking <<<<<<<<<<<<<>>>>>>>>>>>>>>>>>
        else{
            *hypotenuseDistance = 600.0;
            *thetaDegree = 600.0;
            return 7; // OBSTACLE
        }
    }
    else if(ir_0_bool && ir_1_bool && ir_2_bool){
        // Get the closest reading and align to it
        // If true them IR0 is closest to object
        if((ir_0 < ir_1)&&(ir_0 < ir_2)){
            adjacentSide = SENSOR_MOUNT_DISTANCE + ir_1;
            oppositeSide = SENSOR_MOUNT_INNER;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 3; // MAJOR RIGHT
        }
        // If true them IR1 or IR2 is closest to object
        else if(((ir_1 < ir_0)&&(ir_1 < ir_2)) || ((ir_2 < ir_1)&&(ir_2 < ir_0))){
            adjacentSide = SENSOR_MOUNT_DISTANCE + ir_1;
            oppositeSide = SENSOR_MOUNT_INNER;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 1; // SLIGHT RIGHT
        }
        // This is for Error Checking <<<<<<<<<<<<<>>>>>>>>>>>>>>>>>
        else{
            *hypotenuseDistance = 600.0;
            *thetaDegree = 600.0;
            return 7; // OBSTACLE
        }
    }
    else if(ir_1_bool && ir_2_bool && ir_3_bool){
        // Get the closest reading and align to it
        // If true them IR1 is closest to object
        if((ir_1 < ir_2)&&(ir_1 < ir_3)){
            midwayRatio = (ir_1 + ir_2) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 1; // SLIGHT RIGHT
        }
        // If true them IR2 is closest to object
        else if((ir_2 < ir_1)&&(ir_2 < ir_3)){
            if((ir_2 < 4.0)&&(ir1_ir2_range < 2.0)&&(ir2_ir3_range < 2.0)){ // ALIGNED
                adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                oppositeSide = 0.0;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                *thetaDegree = 0.0;
                return 6; // ALIGNED
            }
            else{
                adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                oppositeSide = 0.0;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                *thetaDegree = 0.0;
                return 5; // ALIGNED NOT CLOSE
            }
        }
        // If true them IR3 is closest to object
        else if((ir_3 < ir_1)&&(ir_3 < ir_2)){
            midwayRatio = (ir_3 + ir_2) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            thetaRadian = -1.0 * thetaRadian;
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 2; // SLIGHT LEFT
        }
        // This is for Error Checking <<<<<<<<<<<<<>>>>>>>>>>>>>>>>>
        else{
            *hypotenuseDistance = 600.0;
            *thetaDegree = 600.0;
            return 7; // OBSTACLE
        }
    }
    // We would want to center by angling towards IR3
    else if(ir_2_bool && ir_3_bool && ir_4_bool){
        // Get the closest reading and align to it
        // If true them IR2 or IR3 is closest to object
        if(((ir_2 < ir_3)&&(ir_2 < ir_4)) || ((ir_3 < ir_2)&&(ir_3 < ir_4))){
            adjacentSide = SENSOR_MOUNT_DISTANCE + ir_3;
            oppositeSide = SENSOR_MOUNT_INNER;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            thetaRadian = -1.0 * thetaRadian;
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 2; // SLIGHT LEFT
        }
        // If true them IR4 is closest to object
        else if((ir_4 < ir_2)&&(ir_4 < ir_3)){
            adjacentSide = SENSOR_MOUNT_DISTANCE + ir_3;
            oppositeSide = SENSOR_MOUNT_INNER;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            thetaRadian = -1.0 * thetaRadian;
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 4; // MAJOR LEFT
        }
        // This is for Error Checking <<<<<<<<<<<<<>>>>>>>>>>>>>>>>>
        else{
            *hypotenuseDistance = 600.0;
            *thetaDegree = 600.0;
            return 7; // OBSTACLE
        }
    }
    else if(ir_0_bool && ir_1_bool){
        // If true then IR0 is closest to object
        if(ir_0 < ir_1){
            midwayRatio = (ir_0 + ir_1) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_OUTTER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 3; // MAJOR RIGHT
        }
        // If gets here IR1 is closest
        else{
            midwayRatio = (ir_0 + ir_1) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_OUTTER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 1; // SLIGHT RIGHT
        }
    }
    else if(ir_1_bool && ir_2_bool){
        // If true then IR1 is closest to object
        if(ir_1 < ir_2){
            midwayRatio = (ir_1 + ir_2) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 1; // SLIGHT RIGHT
        }
        // If gets here IR2 is closest
        else{
            if((ir_2 < 4.0)&&(ir1_ir2_range < 2.6)){
                adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                oppositeSide = 0.0;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                *thetaDegree = 0.0;
                return 6; // ALIGNED
            }
            else{
                // check alignment distance
                if(ir_2 > 4.0){
                    adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                    oppositeSide = 0.0;
                    temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                    *hypotenuseDistance = temphyp;
                    *thetaDegree = 0.0;
                    return 5; // ALIGNED NOT CLOSE
                }
                // If gets here not quite aligned
                else{
                    midwayRatio = (ir_0 + ir_1) / 2.0;
                    adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
                    oppositeSide = SENSOR_MOUNT_MIDWAY_OUTTER_EXACT;
                    temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                    *hypotenuseDistance = temphyp;
                    thetaRadian = sin(oppositeSide / temphyp);
                    *thetaDegree = thetaRadian * (180.0/M_PI);
                    return 1; // SLIGHT RIGHT
                }
            }
        }
    }
    else if(ir_2_bool && ir_3_bool){
        // If true then IR3 is closest to object
        if(ir_3 < ir_2){
            midwayRatio = (ir_3 + ir_2) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            thetaRadian = -1.0 * thetaRadian;
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 2; // SLIGHT LEFT
        }
        // If gets here IR2 is closest
        else{
            if((ir_2 < 4.0)&&(ir2_ir3_range < 2.6)){
                adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                oppositeSide = 0.0;
                temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                *hypotenuseDistance = temphyp;
                *thetaDegree = 0.0;
                return 6; // ALIGNED
            }
            else{
                // check alignment distance
                if(ir_2 > 4.0){
                    adjacentSide = SENSOR_MOUNT_DISTANCE + ir_2;
                    oppositeSide = 0.0;
                    temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                    *hypotenuseDistance = temphyp;
                    *thetaDegree = 0.0;
                    return 5; // ALIGNED NOT CLOSE
                }
                // If gets here not quite aligned
                else{
                    midwayRatio = (ir_3 + ir_2) / 2.0;
                    adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
                    oppositeSide = SENSOR_MOUNT_MIDWAY_INNER_EXACT;
                    temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
                    *hypotenuseDistance = temphyp;
                    thetaRadian = sin(oppositeSide / temphyp);
                    thetaRadian = -1.0 * thetaRadian;
                    *thetaDegree = thetaRadian * (180.0/M_PI);
                    return 2; // SLIGHT LEFT
                }
            }
        }
    }
    else if(ir_3_bool && ir_4_bool){
        // If returns true then IR3 is closest to object
        if(ir_3 < ir_4){
            midwayRatio = (ir_3 + ir_4) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_OUTTER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            thetaRadian = -1.0 * thetaRadian;
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 2; // SLIGHT LEFT
        }
        // If gets here then IR4 is closest to object
        else{
            midwayRatio = (ir_3 + ir_4) / 2.0;
            adjacentSide = SENSOR_MOUNT_DISTANCE + midwayRatio;
            oppositeSide = SENSOR_MOUNT_MIDWAY_OUTTER_EXACT;
            temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
            *hypotenuseDistance = temphyp;
            thetaRadian = sin(oppositeSide / temphyp);
            thetaRadian = -1.0 * thetaRadian;
            *thetaDegree = thetaRadian * (180.0/M_PI);
            return 4; // MAJOR LEFT
        }
    }
    else if(ir_0_bool){
        adjacentSide = SENSOR_MOUNT_DISTANCE + ir_0;
        oppositeSide = SENSOR_MOUNT_OUTTER + 0.788;
        temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
        *hypotenuseDistance = temphyp;
        thetaRadian = sin(oppositeSide / temphyp);
        *thetaDegree = thetaRadian * (180.0/M_PI);
        return 3; // MAJOR RIGHT
    }
    else if(ir_4_bool){
        adjacentSide = SENSOR_MOUNT_DISTANCE + ir_4;
        oppositeSide = SENSOR_MOUNT_OUTTER + 0.788;
        temphyp = sqrt(powf(adjacentSide, 2.0) + powf(oppositeSide, 2.0)); 
        *hypotenuseDistance = temphyp;
        thetaRadian = sin(oppositeSide / temphyp);
        thetaRadian = -1.0 * thetaRadian;
        *thetaDegree = thetaRadian * (180.0/M_PI);
        return 4; // MAJOR LEFT
    }
    // This is for Error Checking <<<<<<<<<<<>>>>>>>>>>>>>>>>>>
    else{
        *hypotenuseDistance = 500.0;
        *thetaDegree = 500.0;
        return 0; // NOT ALIGNED latest values were not good, not aligned
    }
}

/* This subroutine will progress the state machine to target alignment state    */
/* It reads the computation thread queue for the proximity distance value       */
/* from the PathFinder                                                          */
/* Inputs: N/A                                                                  */
/* Outputs: This function returns true if the PathFinder sends a value of       */
/* 6 inches or lower                                                            */
bool targetProximitySubr(){
    TargetAlignment_Type pathFinderProximity;
    memset(&pathFinderProximity, 0, sizeof(TargetAlignment_Type));
    while(1){
        if(xQueueReceive(_queue, &pathFinderProximity, portMAX_DELAY)){
            if((pathFinderProximity.targetProximityInt <= 6) && (pathFinderProximity.targetProximityInt > 2)){
                return true;
            }
            else{
                return false;
            }
        }
    }
}
    

/* This subroutine will progress the state machine to target acquire   state    */
/* It reads the computation thread queue for the movement stopped value         */
/* from the PathFinder                                                          */
/* Inputs: N/A                                                                  */
/* Outputs: This function returns true if the PathFinder sends a value of       */
/* true                                                                         */
bool movementStoppedSubr(){
    TargetAlignment_Type pathFinderStopped;
    memset(&pathFinderStopped, 0, sizeof(TargetAlignment_Type));
    while(1){
        if(xQueueReceive(_queue, &pathFinderStopped, portMAX_DELAY)){
            if(pathFinderStopped.movementStoppedBool == true){
                return true;
            }
            else{
                return false;
            }
        }
    }
}

/* This function turns the grabber on to grab or drop the target                */
/* Inputs: Char that determines the action of the grabber. GRAB or DROP         */
/* Outputs: Boolean to advance the state machine                                */
bool grabberActionSubr(char action){
    bool grabComplete = false;
    bool dropComplete = false;
    MessageObj objAction;
    memset(&objAction, 0, sizeof(MessageObj));
    objAction.Type = UPDATE;
    objAction.Update.Type = TARGETACQUIRED;
    if(action == 'G'){
        // GRAB
        grabComplete = targetServo();
        if(grabComplete == true){
            objAction.Update.Data.targetAcquiredBool = true;
            // Sending to Tx Thread Q
            MESSAGE_CONTROLLER_THREAD_SendToQueue(objAction);
            return true;
        }
        else{
            objAction.Update.Data.targetAcquiredBool = false;
            // Sending to Tx Thread Q
            MESSAGE_CONTROLLER_THREAD_SendToQueue(objAction);
            return false;
        }
    }
    else if(action == 'D'){
        // DROP
        dropComplete = openServo();
        if(dropComplete == true){
            objAction.Update.Data.targetAcquiredBool = false;
            // Sending to Tx Thread Q
            MESSAGE_CONTROLLER_THREAD_SendToQueue(objAction);
            return true;
        }
        else{
            objAction.Update.Data.targetAcquiredBool = true;
            // Sending to Tx Thread Q
            MESSAGE_CONTROLLER_THREAD_SendToQueue(objAction);
            return false;           
        }
    }
    else{
        return false;
    }
}

/* This subroutine will progress the state machine to target drop state         */
/* It reads the computation thread queue for the outside arena value            */
/* from the PathFinder                                                          */
/* Inputs: N/A                                                                  */
/* Outputs: This function returns true if the PathFinder sends a value of       */
/* true                                                                         */
bool outsideArenaSubr(){
    TargetAlignment_Type pathFinderOutsideArena;
    memset(&pathFinderOutsideArena, 0, sizeof(TargetAlignment_Type));
    while(1){
        if(xQueueReceive(_queue, &pathFinderOutsideArena, portMAX_DELAY)){
            if(pathFinderOutsideArena.outsideArenaBool == true){
                return true;
            }
            else{
                return false;
            }
        }
    }
}

/* This function will close the grabber enough to grab a target                 */
/* 430 was chosen due to the target size and for the grabber to close enough    */
/* to hold onto the target.                                                     */
/* Inputs: N/A                                                                  */
/* Outputs: Boolean to let caller function know its task is complete            */
bool targetServo(){
    uint16_t RESET_MOTOR = 332;
    PLIB_OC_PulseWidth16BitSet(OC_ID_1, RESET_MOTOR); // 470 is 7.5% of 20ms timer period 6276 (1.5ms)
    return true;
}

/* This function will open the servo motor completely                           */
/* 500 was chosen because due to physical restraints of the grabber 500 opens   */
/* the grabber to its fullest.                                                  */
/* Inputs: N/A                                                                  */
/* Outputs: Boolean to let caller function know its task is complete            */
bool openServo(){
    uint16_t FULL_MOTOR = 500;
    PLIB_OC_PulseWidth16BitSet(OC_ID_1, FULL_MOTOR); // 628 is 10% of 20ms timer period 6276 (2ms)
    return true;
}

/* This function will power the servo to close the grabber all the way          */
/* Inputs: N/A                                                                  */
/* Outputs: Boolean to let caller function know its task is complete            */
bool closeServo(){
    uint16_t TARGET_MOTOR = 314;
    PLIB_OC_PulseWidth16BitSet(OC_ID_1, TARGET_MOTOR); // 314 is 5% of 20ms timer period 6276 (1ms)
    return true;
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
    DRV_TMR2_Start(); // Starting the 20ms timer for the servo
    DRV_OC0_Start(); // Starting the OC for the servo
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
            /* Going to next State: COMPUTATION_THREAD_STATE_REQUEST_TARGET_PROXIMITY */
            computation_threadData.state = COMPUTATION_THREAD_STATE_REQUEST_TARGET_PROXIMITY;
            break;
        }
        /* This case determines if I can start scanning for targets */
        case COMPUTATION_THREAD_STATE_REQUEST_TARGET_PROXIMITY:
        {
            // SEND REQUEST FOR TARGET PROXIMITY
            bool alignmentState = false;
            alignmentState = targetProximitySubr();
            if(alignmentState == true){
                /* Going to next State: COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT */
                /* Starting the ADC TMR_0 and opening the ADC for scanning        */
                DRV_TMR0_Start();
                DRV_ADC_Open();
                computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT;
            }
            else{
                /* Staying in the same State: COMPUTATION_THREAD_STATE_REQUEST_TARGET_PROXIMITY */
                computation_threadData.state = COMPUTATION_THREAD_STATE_REQUEST_TARGET_PROXIMITY;
            }
            break;
        }
 
        /* This case determines if the target is aligned */
        case COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT:
        {
            bool movementState = false;
            movementState = targetAlignmentSubr();
            if(movementState == true){
                /* Going to next State: COMPUTATION_THREAD_STATE_TARGET_ACQUIRE */
                /* Stops the ADC TMR_0 and closes the ADC from scanning b/c we  */
                /* Do not need to scan any longer. We know we are aligned.      */
                DRV_TMR0_Stop();
                DRV_ADC_Close();
                computation_threadData.state = COMPUTATION_THREAD_STATE_MOVEMENT_STOPPED;
            }
            else{
                /* Keeping State here: COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT */
                computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT;
            }
            break;
        }
        
        /* This case tells the grabber to acquire(grab) the target */
        case COMPUTATION_THREAD_STATE_MOVEMENT_STOPPED:
        {
            bool acquireState = false;
            acquireState = movementStoppedSubr();
            if(acquireState == true){
                /* Going to next State: COMPUTATION_THREAD_STATE_TARGET_ACQUIRE */
                //DRV_TMR2_Start(); // Starting the 20ms timer for the servo
                //DRV_OC0_Start(); // Starting the OC for the servo
                computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_ACQUIRE;
            }
            else{
                /* Keeping State here: COMPUTATION_THREAD_STATE_MOVEMENT_STOPPED */
                computation_threadData.state = COMPUTATION_THREAD_STATE_MOVEMENT_STOPPED;
            }
            break;
        }
        
        /* This case tells the grabber to acquire(grab) the target */
        case COMPUTATION_THREAD_STATE_TARGET_ACQUIRE:
        {
            bool acquireState = false;
            acquireState = grabberActionSubr('G'); // GRAB TARGET
            if(acquireState == true){
                /* Going to next State: COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA */
                //DRV_TMR2_Stop(); // Stopping the 20ms timer for the servo
                //DRV_OC0_Stop(); // Stopping the OC for the servo
                computation_threadData.state = COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA;
            }
            else{
                /* Staying in current State: COMPUTATION_THREAD_STATE_TARGET_ACQUIRE */
                computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_ACQUIRE;
            }
            break;
        }
        // Possibly checks to ensure the target is still there, if not go back
        // to target alignment
        /* This case keeps requesting to see if it can drop the target yet */
        case COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA:
        {
            bool dropState = false;
            dropState = outsideArenaSubr(); // SEND REQUEST FOR ARENA
            if(dropState == true){
                /* Going to next State: COMPUTATION_THREAD_STATE_TARGET_DROP */
                //DRV_TMR2_Start(); // Starting the 20ms timer for the servo
                //DRV_OC0_Start(); // Starting the OC for the servo
                computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_DROP;
            }
            else{
                /* Staying in current State: COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA */
                computation_threadData.state = COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA;
            }
            break;
        }
        /* This case tells the grabber to drop the target */
        case COMPUTATION_THREAD_STATE_TARGET_DROP:
        {
            bool initState = false;
            initState = grabberActionSubr('D'); // DROP TARGET
            if(initState == true){
                /* Going to next State: COMPUTATION_THREAD_STATE_INIT */
                //DRV_TMR2_Stop(); // Stopping the 20ms timer for the servo
                //DRV_OC0_Stop(); // Stopping the OC for the servo
                computation_threadData.state = COMPUTATION_THREAD_STATE_INIT;
            }
            else{
                /* Staying in current State: COMPUTATION_THREAD_STATE_TARGET_DROP */
                computation_threadData.state = COMPUTATION_THREAD_STATE_TARGET_DROP;
            }
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
