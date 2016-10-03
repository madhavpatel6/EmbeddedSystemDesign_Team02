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
#define TYPEOFQUEUE TargetAlignment_Type


/*******************************************************************************
  Function:
    void ADC_THREAD_Initialize ( void )

  Remarks:
    See prototype in adc_thread.h.
 */

void ADC_THREAD_Initialize ( void )
{
    _queue = createAdcQ();
    DRV_TMR0_Start();
    DRV_ADC_Open();
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void ADC_APP_Tasks ( void )

  Remarks:
    See prototype in adc_app.h.
 */

/* Following functions are for milestone1 */
QueueHandle_t createAdcQ(){
    //8 b/c "team 2" //10 is size of Q
    return xQueueCreate(SIZEOFQUEUE,sizeof(TYPEOFQUEUE));
}

/* Sends value from adc to adc_app Queue */
int adc_app_SendValToMsgQ(float adcVal){
    return xQueueSend(_queue, &adcVal, portMAX_DELAY);
}

/* Sends value from adc ISR to adc_app Queue */
int adc_app_SendValToMsgQFromISR(TargetAlignment_Type adcVal, BaseType_t *pxHigherPriorityTaskWoken){
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

/* This converts the IR sensor values to cm for 3.42cm -~ 20cm       */
/* This uses Chris' equation :             1                         */
/*                            ---------------------------------      */
/*                            (AnalogVoltage - 0.2243)/(9.6762)      */
/* Analog voltage is found by converting the digital value to analog */
/* Inputs a uint32_t digital value from ADC                          */
/* Outputs converted digital value to cm                             */
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


/******************************************************************************
  Function:
    void ADC_THREAD_Tasks ( void )

  Remarks:
    See prototype in adc_thread.h.
 */

 void ADC_THREAD_Tasks ( void )
 {
     dbgOutputLoc(ENTER_TASK_ADC_APP);
     MessageObj obj;
     TargetAlignment_Type distanceIR;
     obj.Type = UPDATE;
     obj.Update.Type = ALIGNMENTDATA;
     dbgOutputLoc(BEFORE_WHILE_ADC_APP);
     while(1){
         dbgOutputLoc(BEFORE_RECEIVE_FROM_Q_ADC_APP);
         if(xQueueReceive(_queue, &distanceIR, portMAX_DELAY)){
             convertTocmIR(distanceIR.IR_0);
             convertTocmIR(distanceIR.IR_1);
             convertTocmIR(distanceIR.IR_2);
             // Storing the converted cm value in the message object
             obj.Update.Data.alignmentData.IR_0 = convertTocmIR(distanceIR.IR_0);
             obj.Update.Data.alignmentData.IR_1 = convertTocmIR(distanceIR.IR_1);
             obj.Update.Data.alignmentData.IR_2 = convertTocmIR(distanceIR.IR_2);
             // Sending to Tx Thread Q
             MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
         }
         dbgOutputLoc(AFTER_RECEIVE_FROM_Q_ADC_APP);
     }
 }



/*******************************************************************************
 End of File
 */
