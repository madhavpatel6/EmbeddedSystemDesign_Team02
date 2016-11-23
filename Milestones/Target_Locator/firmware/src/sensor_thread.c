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

#include "stddef.h"
#include "stdbool.h"
#include "myqueue.h"
#include "system_config.h"
#include "system_definitions.h"
#include <math.h>
#include "sensor_thread.h"

static QueueHandle_t _queue;

#define TYPEOFQUEUE TL_Queue_t
#define SIZEOFQUEUE 10

#define M_PI 3.14159265358979323846

/*******************************************************************************
  Function:
    void SENSOR_THREAD_Initialize ( void )

  Remarks:
    See prototype in sensor_thread.h.
 */
static int x;
void SENSOR_THREAD_Initialize ( void )
{
    SENSOR_THREAD_InitializeQueue();
    DRV_TMR0_Start();

    DRV_ADC_Open();
}
static GridType grid;
static Queue sensorDataQ;
/******************************************************************************
  Function:
    void SENSOR_THREAD_Tasks ( void )

  Remarks:
    See prototype in sensor_thread.h.
 */

void SENSOR_THREAD_Tasks ( void )
{
    initializeQueue(&sensorDataQ);
    TL_Queue_t objRecv;
    MessageObj objSend;
    objSend.type = UPDATE;
    objSend.message.Update.Type = SENSORDATA;
    SensorDataContainerType sensorInformation;
    sensorInformation.middleFrontSensor.minimumMeasuringDistance = 20;
    sensorInformation.rightFrontSensor.maximumMeasuringDistance = 70;
    sensorInformation.rightFrontSensor.minimumMeasuringDistance = 20;
    sensorInformation.rightFrontSensor.orientation = 0;
    sensorInformation.leftFrontSensor.maximumMeasuringDistance = 70;
    sensorInformation.leftFrontSensor.minimumMeasuringDistance = 20;
    sensorInformation.leftFrontSensor.orientation = 0;
    sensorInformation.middleFrontSensor.maximumMeasuringDistance = 30;
    sensorInformation.middleFrontSensor.minimumMeasuringDistance = 7;
    sensorInformation.middleFrontSensor.orientation = 0;

    initializeGrid(grid);
    Movement_t previousAction;
    bool previousActionIsSet = false;
    while(1) {
        memset(&objRecv, 0, sizeof(TL_Queue_t));
        SENSOR_THREAD_ReadFromQueue(&objRecv);
        switch(objRecv.type) {
            case SENSORADC: {
                ConvertSensorADCToDistance(&objSend.message.Update.Data.sensordata, objRecv.contents.sensors);
                /* Queue up the sensor data */
                if(FilterIRSensors(objSend.message.Update.Data.sensordata)) {
                    if(isFull(&sensorDataQ)) {
                        removeData(&sensorDataQ);
                        insertData(&sensorDataQ, objSend.message.Update.Data.sensordata);
                    }
                    else {
                        insertData(&sensorDataQ, objSend.message.Update.Data.sensordata);
                    }
                }
                MESSAGE_CONTROLLER_THREAD_SendToQueue(objSend);
                break;
            }
            case RV1_POSUPDATE: {
                if(previousActionIsSet && size(&sensorDataQ) != 0) {
                    // Stop the ADC timer
                    DRV_TMR0_Stop();
                    point_t deltaPosition;
                    int i = 0;
                    deltaPosition.x = previousAction.x - objRecv.contents.r1_movement.x;
                    deltaPosition.y = previousAction.y - objRecv.contents.r1_movement.y;
                    float dtheta =  objRecv.contents.r1_movement.orientation - previousAction.orientation;
                    float distance = sqrt(pow(deltaPosition.x,2) + pow(deltaPosition.y,2));
                    float distance_increments = distance / size(&sensorDataQ);
                    float theta_increments = dtheta / size(&sensorDataQ);
                    float cosX = cos(objRecv.contents.r1_movement.orientation*M_PI/180)*distance_increments;
                    float sinY = sin(objRecv.contents.r1_movement.orientation*M_PI/180)*distance_increments;
                    int sizeofqueue = size(&sensorDataQ);
                    for(i = 0; i < sizeofqueue; i++) {
                        SensorDataType data = removeData(&sensorDataQ);
                        point_t location;
                        float theta = previousAction.orientation + theta_increments*i;
                        location.x = cosX*i+previousAction.x;
                        location.y = sinY*i+previousAction.y;
                        UpdateSensorLocations(&sensorInformation, data, location, theta);
                        updateOccupanyGrid2(sensorInformation, grid);
                    }
                    point_t loc;
                    loc.x = objRecv.contents.r1_movement.x;
                    loc.y = objRecv.contents.r1_movement.y;
                    SensorDataType data;
                    UpdateSensorLocations(&sensorInformation, data, loc, objRecv.contents.r1_movement.orientation);                    
                    objSend.message.Update.Data.sensorInformation = sensorInformation;
                    // Start the ADC timer
                    DRV_TMR0_Start();
                }
                else {
                    clearQueue(&sensorDataQ);
                    previousActionIsSet = true;
                }
                previousAction = objRecv.contents.r1_movement;
                break;
            }
            case REQUESTOCCUPANYGRID: {
                int i = 0;
                for(i = 0; i < HEIGHT; i++) {
                    Tx_Thead_Queue_DataType txObj;
                    memset(&txObj, 0, sizeof(Tx_Thead_Queue_DataType));
                    txObj.Destination = SERVER;
                    int j = 0;
                    sprintf(txObj.Data, "{\"type\":\"Response\",\"OccupancyGrid\":{\"row\":\"%d\",\"data\":\"", i);
                    for(j = 0; j < WIDTH; j++) {
                        sprintf(txObj.Data + strlen(txObj.Data), "%c", grid[i][j]);
                    }
                    sprintf(txObj.Data + strlen(txObj.Data), "\"}}");
                    TX_THREAD_SendToQueue(txObj);
                }
                break;
            }
        }
    }
}

void UpdateSensorLocations(SensorDataContainerType* sensors, SensorDataType distances, point_t roverLocation, int orientation) {
    /* Update the distances */
    sensors->leftFrontSensor.distance = distances.ir.leftFBSensor;
    sensors->middleFrontSensor.distance = distances.ir.middleFBSensor;
    sensors->rightFrontSensor.distance = distances.ir.rightFBSensor;
    sensors->leftFrontSensor.orientation = orientation;
    sensors->middleFrontSensor.orientation = orientation;
    sensors->rightFrontSensor.orientation = orientation;
    /* Compute a new sensor location */
    sensors->roverLocation = roverLocation;
    sensors->orientation = orientation;
    rotatePoint(&sensors->leftFrontSensor.sensorLocation, roverLocation.x, roverLocation.y, roverLocation.x + 5, roverLocation.y + 3, orientation);
    rotatePoint(&sensors->middleFrontSensor.sensorLocation, roverLocation.x, roverLocation.y, roverLocation.x + 5, roverLocation.y, orientation);
    rotatePoint(&sensors->rightFrontSensor.sensorLocation, roverLocation.x, roverLocation.y, roverLocation.x + 5, roverLocation.y - 3, orientation);
}

void ConvertSensorADCToDistance(SensorDataType* distances, SensorADC_t adcValues) {
    ConvertBottomFarLeftLongRangeIRToCM(&distances->ir.farLeftFBSensor, adcValues.IRSensors.farLeftFBSensor);
    ConvertBottomLeftLongRangeIRToCM(&distances->ir.leftFBSensor, adcValues.IRSensors.leftFBSensor);
    ConvertMidRangeToCM(&(distances->ir.middleFBSensor), adcValues.IRSensors.middleFBSensor);
    ConvertBottomRightLongRangeIRToCM(&(distances->ir.rightFBSensor),  adcValues.IRSensors.rightFBSensor);
    ConvertBottomFarRightLongRangeIRToCM(&(distances->ir.farRightFBSensor), adcValues.IRSensors.farRightFBSensor);
    ConvertTopLeftLongRangeIRToCM(&(distances->ir.leftFTSensor),  adcValues.IRSensors.leftFTSensor);
    ConvertTopRightLongRangeIRToCM(&(distances->ir.rightFTSensor),  adcValues.IRSensors.rightFTSensor);
}


void GetDistanceFromLookupTableIR(float* distanceCM, LookupTable_t lookupTable[], size_t size, uint32_t adcValue) {
	float voltage = adcValue / (310.303);
	if (voltage < lookupTable[size - 1].voltage) {
		*distanceCM = lookupTable[size - 1].distance;
	}
	else if (voltage < lookupTable[0].voltage){
		size_t i = 0;
		for (i = 0; i < size - 1; i++) {
			if (lookupTable[i].voltage >= voltage && voltage >= lookupTable[i + 1].voltage) {
				float slope = (lookupTable[i + 1].distance - lookupTable[i].distance) / (lookupTable[i + 1].voltage - lookupTable[i].voltage);
				
				float yintercept = lookupTable[i].distance - slope*lookupTable[i].voltage;
				*distanceCM = slope*voltage + yintercept;
				break;
			}
		}
	}
	else {
		*distanceCM = -1;
	}
}

void ConvertShortRangeToCM(float* distanceCM, uint32_t adcValue) {
    float voltage = adcValue/(310.303);
    *distanceCM = 0;
    if(voltage <= 0.52) {
        *distanceCM = 20;
    }
    else if(voltage < 1) {
        *distanceCM = 12.115/(voltage+0.01042);
    }
    else if(voltage < 2.74) {
        *distanceCM = 21.6282*(pow(0.491235, voltage));
    }
    else {
        *distanceCM = -1;
    }
    *distanceCM = voltage;
}

void ConvertMidRangeToCM(float* distanceCM, uint32_t adcValue) {
    float voltage = adcValue/(310.303);
    *distanceCM = 0;
    if(voltage < 0.82) {
        *distanceCM = 30;
    }
    else if(voltage < 2.284) {
        *distanceCM = 22.053/(voltage-0.0915);
    }
    else if(voltage < 3) {
        *distanceCM = 15.45/(voltage-0.7419);
    }
    else {
        *distanceCM = -1;
    }
    *distanceCM = voltage;
}

void ConvertBottomLeftLongRangeIRToCM(float* distanceCM, uint32_t adcValue) {
    float avgAdcValue = adcValue;
    float voltage = avgAdcValue/(310.303);
    
    if(voltage < 0.66) {
        *distanceCM = 70;
    }
    else if(voltage < 0.97) {
        *distanceCM = 79.765/(voltage+0.4716);
    }
    else if(voltage < 2.42) {
        *distanceCM = 107.703*(pow(0.5, voltage));
    }
    else {
        *distanceCM = -1;
    }
    *distanceCM = voltage;
}

void ConvertBottomRightLongRangeIRToCM(float* distanceCM, uint32_t adcValue) {
    float avgAdcValue = adcValue;
    float voltage = avgAdcValue/(310.303);
    
    if(voltage < 0.77) {
        *distanceCM = 70;
    }
    else if(voltage < 1.132) {
        *distanceCM = 63.402/(voltage+0.1329);
    }
    else if(voltage < 2.47) {
        *distanceCM = 106.133*(pow(0.5102, voltage));
    }
    else {
        *distanceCM = -1;
    }
    *distanceCM = voltage;
}


void ConvertBottomFarLeftLongRangeIRToCM(float* distanceCM, uint32_t adcValue) {
    float avgAdcValue = adcValue;
    float voltage = avgAdcValue/(310.303);
    *distanceCM = voltage;
}

void ConvertBottomFarRightLongRangeIRToCM(float* distanceCM, uint32_t adcValue) {
    float avgAdcValue = adcValue;
    float voltage = avgAdcValue/(310.303);
    *distanceCM = voltage;
}

void ConvertTopLeftLongRangeIRToCM(float* distanceCM, uint32_t adcValue) {
    float avgAdcValue = adcValue;
    float voltage = avgAdcValue/(310.303);
    
    if(voltage < 0.87) {
        *distanceCM = 70;
    }
    else if(voltage < 1.170) {
        *distanceCM = 51.833/(voltage-0.1318);
    }
    else if(voltage < 2.46) {
        *distanceCM = 110.775*(pow(0.501, voltage));
    }
    else {
        *distanceCM = -1;
    }
    *distanceCM = voltage;
}

void ConvertTopRightLongRangeIRToCM(float* distanceCM, uint32_t adcValue) {
    float avgAdcValue = adcValue;
    float voltage = avgAdcValue/(310.303);
    
    if(voltage < 0.87) {
        *distanceCM = 70;
    }
    else if(voltage < 1.170) {
        *distanceCM = 51.833/(voltage-0.1318);
    }
    else if(voltage < 2.46) {
        *distanceCM = 110.775*(pow(0.501, voltage));
    }
    else {
        *distanceCM = -1;
    }
    *distanceCM = voltage;
}


bool FilterIRSensors(SensorDataType sensors) {
    if(sensors.ir.middleFBSensor < 17 || sensors.ir.rightFBSensor == -1 || sensors.ir.leftFBSensor == -1) {
        return false;
    }
    return true;
}

void SENSOR_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
}

void SENSOR_THREAD_ReadFromQueue(TYPEOFQUEUE* pvBuffer) {
    xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
}

void SENSOR_THREAD_SendToQueue(TYPEOFQUEUE buffer) {
    xQueueSend(_queue, &buffer, portMAX_DELAY);
}

void SENSOR_THREAD_SendToQueueISR(TYPEOFQUEUE buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendFromISR(_queue, &buffer, pxHigherPriorityTaskWoken);
}


/*******************************************************************************
 End of File
 */
