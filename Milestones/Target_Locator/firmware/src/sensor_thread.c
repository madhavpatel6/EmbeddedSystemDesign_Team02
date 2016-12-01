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
static GridUpdatedType updated;
SensorDataType currentSensorData;
LookupTable_t bottomLeftLongRangeTable[] = {
    {20,	2.508},
    {25,	2.213},
    {30,	1.881},
    {35,	1.621},
    {40,	1.415},
    {45,	1.245},
//    {50,	1.113},
//    {55,	1.007},
//    {60,	0.922},
//    {65,	0.841},
//    {70,	0.781},
};

LookupTable_t bottomRightLongRangeTable[] = {
    {20,	2.430},
    {25,	2.127},
    {30,	1.854},
    {35,	1.588},
    {40,	1.387},
    {45,	1.219},
//    {50,	1.098},
//    {55,	0.980},
//    {60,	0.881},
//    {65,	0.804},
//    {70,	0.732},
};

LookupTable_t topLeftLongRangeTable[] = {
    {20,	2.535},
    {25,	2.233},
    {30,	1.941},
    {35,	1.683},
    {40,	1.479},
    {45,	1.318},
//    {50,	1.173},
//    {55,	1.067},
//    {60,	0.963},
//    {65,	0.889},
//    {70,	0.838},
};
LookupTable_t topRightLongRangeTable[] = {
    {20,	2.510},
    {25,	2.221},
    {30,	1.948},
    {35,	1.688},
    {40,	1.489},
    {45,	1.330},
//    {50,	1.211},
//    {55,	1.099},
//    {60,	1.001},
//    {65,	0.930},
//    {70,	0.865},
};

LookupTable_t middleMidRangeTable[] = {
    {7,     3.051},
    {10,	2.312},
    {15,	1.600},
    {20,	1.232},
    {25,	0.981},
    {30,	0.825},
//    {35,	0.731},
//    {40,	0.647},
};

LookupTable_t leftMidRangeTable[] = {
    {7, 	3.048},
    {10,	2.301},
    {15,	1.562},
    {20,	1.213},
    {25,	0.983},
    {30,	0.842},
//    {35,	0.733},
//    {40,	0.666},
};

LookupTable_t rightMidRangeTable[] = {
    {7,     2.959},
    {10,	2.218},
    {15,	1.569},
    {20,	1.221},
    {25,	0.991},
    {30,	0.833},
//    {35,	0.730},
//    {40,	0.644},  
};

void addRayTrace(SensorData_t sensorData, bool maximum) {
    if(sensorData.distance != -1) {
        point_t rayTracePoint;
        rayTracePoint.x = sensorData.sensorLocation.x + sensorData.distance*cos(sensorData.orientation*M_PI/180);
        rayTracePoint.y = sensorData.sensorLocation.y + sensorData.distance*sin(sensorData.orientation*M_PI/180);
        raytrace3(sensorData.sensorLocation.x, sensorData.sensorLocation.y,
                rayTracePoint.x, rayTracePoint.y, maximum, grid, updated);
    }
}

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


    initializeGrid(grid);
    
    
    while(1) {
        memset(&objRecv, 0, sizeof(TL_Queue_t));
        SENSOR_THREAD_ReadFromQueue(&objRecv);
        switch(objRecv.type) {
            case SENSORADC: {
                ConvertSensorADCToDistance(&objSend.message.Update.Data.sensordata, objRecv.contents.sensors);
                /* Queue up the sensor data */
                if(FilterIRSensors(objSend.message.Update.Data.sensordata)) {
                    currentSensorData = objSend.message.Update.Data.sensordata;
                    if(isFull(&sensorDataQ)) {
                        removeData(&sensorDataQ);
                        insertData(&sensorDataQ, objSend.message.Update.Data.sensordata);
                    }
                    else {
                        insertData(&sensorDataQ, objSend.message.Update.Data.sensordata);
                    }
                }
                UpdateProximityInformation(&objSend.message.Update.Data.proximity, objSend.message.Update.Data.sensordata);
                MESSAGE_CONTROLLER_THREAD_SendToQueue(objSend);
                break;
            }
            case RV1_POSUPDATE: {
//                if(previousActionIsSet && size(&sensorDataQ) != 0) {
                    // Stop the ADC timer
                    DRV_TMR0_Stop();
                    point_t location;
                    float theta = objRecv.contents.r1_movement.orientation;
                    location.x = objRecv.contents.r1_movement.x;
                    location.y = objRecv.contents.r1_movement.y;
                    UpdateSensorInformation(&sensorInformation, currentSensorData, location, theta);
                    addRayTrace(sensorInformation.middleFrontSensor, sensorInformation.middleFrontSensor.distance == 30.00);
                    addRayTrace(sensorInformation.farLeftSensor, sensorInformation.farLeftSensor.distance == 30.00);
                    addRayTrace(sensorInformation.farRightSensor, sensorInformation.farRightSensor.distance == 30.00);
                    addRayTrace(sensorInformation.leftFrontSensor, sensorInformation.leftFrontSensor.distance == 45.00);
                    addRayTrace(sensorInformation.rightFrontSensor, sensorInformation.rightFrontSensor.distance == 45.00);
//                    updateOccupanyGrid3(sensorInformation, grid, updated);
//                    point_t deltaPosition;
//                    int i = 0;
//                    deltaPosition.x = previousAction.x - objRecv.contents.r1_movement.x;
//                    deltaPosition.y = previousAction.y - objRecv.contents.r1_movement.y;
//                    float dtheta;
//                    float distance;
//                    if(objRecv.contents.r1_movement.action == 0 || objRecv.contents.r1_movement.action == 1) {
//                        dtheta = 0;
//                        distance = sqrt(pow(deltaPosition.x,2) + pow(deltaPosition.y,2));
//                        if(distance == 0) {
//                            clearQueue(&sensorDataQ);
//                            DRV_TMR0_Start();
//                            continue;
//                        }
//                    }
//                    else if(objRecv.contents.r1_movement.action == 2 || objRecv.contents.r1_movement.action == 3) {
//                        distance = 0;
//                        dtheta = objRecv.contents.r1_movement.orientation - previousAction.orientation;
//                        if(dtheta == 0) {
//                            clearQueue(&sensorDataQ);
//                            DRV_TMR0_Start();
//                            continue;
//                        }
//                    }
//                    
//                    float distance_increments = distance / size(&sensorDataQ);
//                    float theta_increments = dtheta / size(&sensorDataQ);
//                    float cosX = cos(objRecv.contents.r1_movement.orientation*M_PI/180)*distance_increments;
//                    float sinY = sin(objRecv.contents.r1_movement.orientation*M_PI/180)*distance_increments;
//                    int sizeofqueue = size(&sensorDataQ);
//                    
//                    for(i = 0; i < sizeofqueue; i++) {
//                        SensorDataType data = removeData(&sensorDataQ);
//                        point_t location;
//                        float theta = previousAction.orientation + (theta_increments*i);
//                        location.x = (cosX*i)+previousAction.x;
//                        location.y = (sinY*i)+previousAction.y;
//                        UpdateSensorInformation(&sensorInformation, data, location, theta);
//                        updateOccupanyGrid3(sensorInformation, grid, updated);
//                    }
//                    point_t loc;
//                    loc.x = objRecv.contents.r1_movement.x;
//                    loc.y = objRecv.contents.r1_movement.y;
//                    SensorDataType data;
//                    UpdateSensorInformation(&sensorInformation, data, loc, objRecv.contents.r1_movement.orientation);                    
//                    objSend.message.Update.Data.sensorInformation = sensorInformation;
                    // Start the ADC timer
                    DRV_TMR0_Start();
//                }
//                else {
//                    clearQueue(&sensorDataQ);
//                    previousActionIsSet = true;
//                }
//                previousAction = objRecv.contents.r1_movement;
                break;
            }
            case REQUESTOCCUPANYGRID: {
//                if(updated[objRecv.contents.row] == true) {
                    Tx_Thead_Queue_DataType txObj;
                    memset(&txObj, 0, sizeof(Tx_Thead_Queue_DataType));
                    txObj.Destination = SERVER;
                    int j = 0;
                    sprintf(txObj.Data, "{\"type\":\"Response\",\"OccupancyGrid\":{\"row\":\"%d\",\"data\":\"", objRecv.contents.row);
                    for(j = 0; j < WIDTH; j++) {
                        sprintf(txObj.Data + strlen(txObj.Data), "%c", grid[objRecv.contents.row][j]);
                    }
                    sprintf(txObj.Data + strlen(txObj.Data), "\"}}");
                    updated[objRecv.contents.row] = false;
                    TX_THREAD_SendToQueue(txObj);
//                }
                break;
            }
        }
    }
}

void UpdateSensorInformation(SensorDataContainerType* sensors, SensorDataType distances, point_t roverLocation, float orientation) {
    /* Update the distances */
    sensors->leftFrontSensor.distance = distances.ir.leftFBSensor;
    sensors->middleFrontSensor.distance = distances.ir.middleFBSensor;
    sensors->rightFrontSensor.distance = distances.ir.rightFBSensor;
    sensors->farLeftSensor.distance = distances.ir.farLeftFBSensor;
    sensors->farRightSensor.distance = distances.ir.farRightFBSensor;
    sensors->leftFrontSensor.orientation = orientation;
    sensors->middleFrontSensor.orientation = orientation;
    sensors->rightFrontSensor.orientation = orientation;
    sensors->farLeftSensor.orientation = orientation;
    sensors->farRightSensor.orientation = orientation;
    /* Compute a new sensor location */
    sensors->roverLocation = roverLocation;
    sensors->orientation = orientation;
    /* Compute Is Valid */
    sensors->LongRangeIsValid = sensors->middleFrontSensor.distance > 15 && !(sensors->leftFrontSensor.distance == -1) && !(sensors->rightFrontSensor.distance == -1);
    sensors->MidRangeIsValid = !(sensors->farLeftSensor.distance == -1) && !(sensors->farRightSensor.distance == -1);
    /* Rotate the sensor positions */
    rotatePoint(&sensors->leftFrontSensor.sensorLocation,   roverLocation.x, roverLocation.y, roverLocation.x + 6, roverLocation.y + 2, orientation);
    rotatePoint(&sensors->middleFrontSensor.sensorLocation, roverLocation.x, roverLocation.y, roverLocation.x + 6, roverLocation.y,     orientation);
    rotatePoint(&sensors->rightFrontSensor.sensorLocation,  roverLocation.x, roverLocation.y, roverLocation.x + 6, roverLocation.y - 2, orientation);
    rotatePoint(&sensors->farLeftSensor.sensorLocation,     roverLocation.x, roverLocation.y, roverLocation.x + 6, roverLocation.y + 4, orientation);
    rotatePoint(&sensors->farRightSensor.sensorLocation,    roverLocation.x, roverLocation.y, roverLocation.x + 6, roverLocation.y - 4, orientation);
}

void ConvertSensorADCToDistance(SensorDataType* distances, SensorADC_t adcValues) {
    GetDistanceFromLookupTableIR(&distances->ir.leftFBSensor,       bottomLeftLongRangeTable,   sizeof(bottomLeftLongRangeTable) / sizeof(*bottomLeftLongRangeTable),       adcValues.IRSensors.leftFBSensor);
    GetDistanceFromLookupTableIR(&distances->ir.rightFBSensor,      bottomRightLongRangeTable,  sizeof(bottomRightLongRangeTable) / sizeof(*bottomRightLongRangeTable),     adcValues.IRSensors.rightFBSensor);
    GetDistanceFromLookupTableIR(&distances->ir.leftFTSensor,       topLeftLongRangeTable,      sizeof(topLeftLongRangeTable) / sizeof(*topLeftLongRangeTable),             adcValues.IRSensors.leftFTSensor);
    GetDistanceFromLookupTableIR(&distances->ir.rightFTSensor,      topRightLongRangeTable,     sizeof(topRightLongRangeTable) / sizeof(*topRightLongRangeTable),           adcValues.IRSensors.rightFTSensor);
    GetDistanceFromLookupTableIR(&distances->ir.middleFBSensor,     middleMidRangeTable,        sizeof(middleMidRangeTable) / sizeof(*middleMidRangeTable),                 adcValues.IRSensors.middleFBSensor);
    GetDistanceFromLookupTableIR(&distances->ir.farLeftFBSensor,    leftMidRangeTable,          sizeof(leftMidRangeTable) / sizeof(*leftMidRangeTable),                     adcValues.IRSensors.farLeftFBSensor);
    GetDistanceFromLookupTableIR(&distances->ir.farRightFBSensor,   rightMidRangeTable,         sizeof(rightMidRangeTable) / sizeof(*rightMidRangeTable),                   adcValues.IRSensors.farRightFBSensor);
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
//    *distanceCM = voltage;
}

bool FilterIRSensors(SensorDataType sensors) {
    if((sensors.ir.farLeftFBSensor == -1 || sensors.ir.farRightFBSensor == -1 || sensors.ir.middleFBSensor == -1) && (sensors.ir.rightFBSensor == -1 || sensors.ir.leftFBSensor == -1)) {
        return false;
    }
    return true;
}

void UpdateProximityInformation(Proximity_t *proximity, SensorDataType sensors) {
    proximity->leftProximity = sensors.ir.farLeftFBSensor < 15;
    proximity->middleProximity = sensors.ir.middleFBSensor < 15;
    proximity->rightProximity = sensors.ir.farRightFBSensor < 15;
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
