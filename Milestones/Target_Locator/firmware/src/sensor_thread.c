/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    sensor_thread.c

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

#include "sensor_thread.h"
#include "system_config.h"
#include "system_definitions.h"
#include <math.h>
static QueueHandle_t _queue;

#define TYPEOFQUEUE SensorADC_t
#define SIZEOFQUEUE 20

/*******************************************************************************
  Function:
    void SENSOR_THREAD_Initialize ( void )

  Remarks:
    See prototype in sensor_thread.h.
 */

void SENSOR_THREAD_Initialize ( void )
{
    SENSOR_THREAD_InitializeQueue();
    DRV_TMR0_Start();
    DRV_TMR3_Start();
    DRV_ADC_Open();
}


/******************************************************************************
  Function:
    void SENSOR_THREAD_Tasks ( void )

  Remarks:
    See prototype in sensor_thread.h.
 */

void SENSOR_THREAD_Tasks ( void )
{
    SensorADC_t objRecv;
    MessageObj objSend;
    objSend.Type = UPDATE;
    objSend.Update.Type = SENSORDATA;
    UltrasonicContainer ultraDistances;
    memset(&ultraDistances, 0, sizeof(UltrasonicContainer));
    while(1) {
        memset(&objRecv, 0, sizeof(SensorADC_t));
        SENSOR_THREAD_ReadFromQueue(&objRecv);
        
        switch(objRecv.UpdateType) {
            case IRSENSORS: {
                ConvertDigitalToCM(objRecv.IRSensors, &objSend.Update.Data.sensordata.ir);
                break;
            }
            case ULTRASONICSENSORS: {
                HandleUltrasonicUpdate(objRecv.USSensors, &ultraDistances);
                objSend.Update.Data.sensordata.ultrasonic = ultraDistances.distance;
                break;
            }
        }
        MESSAGE_CONTROLLER_THREAD_SendToQueue(objSend);
    }
}

void SENSOR_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
}

void SENSOR_THREAD_ReadFromQueue(SensorADC_t* pvBuffer) {
    xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
}

void SENSOR_THREAD_SendToQueue(SensorADC_t buffer) {
    xQueueSend(_queue, &buffer, portMAX_DELAY);
}

void SENSOR_THREAD_SendToQueueISR(SensorADC_t buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendFromISR(_queue, &buffer, pxHigherPriorityTaskWoken);
}

void ConvertDigitalToCM(IRSensorsADC_t sensorData, IRSensorDistance_t* values) {
    memset(values, 0, sizeof(SensorDataType));
    ConvertTopLeftLongRangeIRToCM(&(values->leftFTSensor), sensorData.leftFTSensor);
//    ConvertGP2Y0A02YK0FToCM(&(values->middleFTSensor), sensorData.middleFTSensor);
    ConvertTopRightLongRangeIRToCM(&(values->rightFTSensor),  sensorData.rightFTSensor);
//    ConvertGP2Y0A02YK0FToCM(&(values->leftFBSensor),  sensorData.leftFBSensor);
//    ConvertGP2Y0A02YK0FToCM(&(values->middleFBSensor), sensorData.middleFBSensor);
//    ConvertGP2Y0A02YK0FToCM(&(values->rightFBSensor), sensorData.rightFBSensor);
//    
}

void ConvertTopLeftLongRangeIRToCM(float* distanceCM, uint32_t adcValue) {
    float avgAdcValue = adcValue;
    float voltage = avgAdcValue/(310.303);

    if(voltage <= 2.5 && voltage >=0.899) {
        *distanceCM = 114.097*(pow(0.484144,voltage));
    }
    else if(voltage >= 0.747 && voltage < 0.899) {
        *distanceCM = 64.47/(voltage+0.1776);
    }
    else {
        *distanceCM = -2;
    }

}

void ConvertTopRightLongRangeIRToCM(float* distanceCM, uint32_t adcValue) {
    float avgAdcValue = adcValue;
    float voltage = avgAdcValue/(310.303);
    
    if(voltage <= 2.5 && voltage >= 1.096) {
        *distanceCM = 119.191*(pow(0.4879,voltage));
    }
    else if(voltage >= 0.854 && voltage < 1.096){
        *distanceCM = 61.223/(voltage+0.0173); 
    }
    else {
        *distanceCM = -1;
    }
//    *distanceCM = voltage;
}

void HandleUltrasonicUpdate(USSensorADC_t sensorData, UltrasonicContainer* values) {
    switch(sensorData.location) {
        case LEFTFRONTULTRASONIC: { 
            ConvertUltrasonicToCM( &(values->distance.leftfront), sensorData.tickCount);
            values->isSet.leftfront = true;
            break;
        }
        case MIDDLEFRONTULTRASONIC: { 
            ConvertUltrasonicToCM( &(values->distance.middlefront), sensorData.tickCount);
            values->isSet.middlefront = true;
            break;
        }
        case RIGHTFRONTULTRASONIC: { 
            ConvertUltrasonicToCM( &(values->distance.rightfront), sensorData.tickCount);
            values->isSet.rightfront = true;
            break;
        }
        case LEFTSIDEULTRASONIC: { 
            ConvertUltrasonicToCM( &(values->distance.leftside), sensorData.tickCount);
            values->isSet.leftside = true;
            break;
        }
        case RIGHTSIDEULTRASONIC: { 
            ConvertUltrasonicToCM( &(values->distance.rightside), sensorData.tickCount);
            values->isSet.rightside = true;
            break;
        }
        default:
            break;
    }
}

void ConvertUltrasonicToCM(float* distanceCM, uint32_t tickCount) {
    *distanceCM = (tickCount)*(1/(80.0))*8/58.0;
}
/*******************************************************************************
 End of File
 */
