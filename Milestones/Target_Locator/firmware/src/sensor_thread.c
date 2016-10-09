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

#define TYPEOFQUEUE SensorADCType
#define SIZEOFQUEUE 10

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
    SensorADCType sensorData;
    MessageObj obj;
    obj.Type = UPDATE;
    obj.Update.Type = SENSORDATA;
    while(1) {
        memset(&sensorData, 0, sizeof(SensorADCType));
        SENSOR_THREAD_ReadFromQueue(&sensorData);
        ConvertDigitalToCM(sensorData, &obj.Update.Data.sensordata);
        MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);
    }
}

void SENSOR_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
}

void SENSOR_THREAD_ReadFromQueue(SensorADCType* pvBuffer) {
    xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
}

void SENSOR_THREAD_SendToQueue(SensorADCType buffer) {
    xQueueSend(_queue, &buffer, portMAX_DELAY);
}

void SENSOR_THREAD_SendToQueueISR(SensorADCType buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendFromISR(_queue, &buffer, pxHigherPriorityTaskWoken);
}

void ConvertDigitalToCM(SensorADCType sensorData, SensorDataType* values) {
    memset(values, 0, sizeof(SensorDataType));
    values->leftFTSensor = sensorData.leftFTSensor;
    values->middleFTSensor = sensorData.middleFTSensor;
    values->rightFTSensor = sensorData.rightFTSensor;
    values->leftFBSensor = sensorData.leftFBSensor;
    values->middleFBSensor = sensorData.middleFBSensor;
    values->rightFBSensor = sensorData.rightFBSensor;
    values->leftSDSensor = sensorData.leftSDSensor;
    values->rightSDSensor = sensorData.rightSDSensor;
//    ConvertGP2Y0A02YK0FToCM(&(values->leftFTSensor), sensorData.leftFTSensor);
//    ConvertGP2Y0A02YK0FToCM(&(values->middleFTSensor), sensorData.middleFTSensor);
//    ConvertGP2Y0A02YK0FToCM(&(values->rightFTSensor),  sensorData.rightFTSensor);
//    ConvertGP2Y0A02YK0FToCM(&(values->leftFBSensor),  sensorData.leftFBSensor);
//    ConvertGP2Y0A02YK0FToCM(&(values->middleFBSensor), sensorData.middleFBSensor);
//    ConvertGP2Y0A02YK0FToCM(&(values->rightFBSensor), sensorData.rightFBSensor);
    
}

void ConvertGP2Y0A02YK0FToCM(float* distanceCM, uint32_t adcValue) {
    float voltage = adcValue/(310.3030);
    if(voltage <= 2.51) {
        *distanceCM = (60.4949)*pow(voltage,-1.1904);
    }
    else {
        *distanceCM = -1;
    }
}
/*******************************************************************************
 End of File
 */
