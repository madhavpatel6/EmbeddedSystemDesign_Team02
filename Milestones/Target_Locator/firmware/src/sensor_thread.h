/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    sensor_thread.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _SENSOR_THREAD_H
#define _SENSOR_THREAD_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "message_controller_thread.h"
#include "gridhelper.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

typedef struct {
    uint32_t leftFTSensor;
    uint32_t rightFTSensor;
    uint32_t farLeftFBSensor;
    uint32_t leftFBSensor;
    uint32_t middleFBSensor;
    uint32_t rightFBSensor;
    uint32_t farRightFBSensor;
} IRSensorsADC_t;


typedef struct {
    IRSensorsADC_t IRSensors;
} SensorADC_t;

typedef enum { SENSORADC, RV1_POSUPDATE, REQUESTOCCUPANYGRID} TLUpdate_t;

typedef struct {
    float x;
    float y;
    float orientation;
    int action;
    float amount;
} Movement_t;

typedef union {
	SensorADC_t sensors;
	Movement_t r1_movement;
    int row;
} TL_Message_t;

typedef struct {
	TLUpdate_t type;
	TL_Message_t contents;
} TL_Queue_t;



typedef struct {
    bool leftfront;
    bool middlefront;
    bool rightfront;
    bool leftside;
    bool rightside;
} UltrasonicIsSet_t;

typedef struct {
	float distance;
	float voltage;
} LookupTable_t; 

/*******************************************************************************
  Function:
    void SENSOR_THREAD_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    SENSOR_THREAD_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void SENSOR_THREAD_Initialize ( void );


/*******************************************************************************
  Function:
    void SENSOR_THREAD_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    SENSOR_THREAD_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void SENSOR_THREAD_Tasks( void );

void SENSOR_THREAD_InitializeQueue();

void SENSOR_THREAD_ReadFromQueue(TL_Queue_t* pvBuffer);

void ConvertSensorADCToDistance(SensorDataType* distances, SensorADC_t adcValues);

void GetDistanceFromLookupTableIR(float* distanceCM, LookupTable_t lookupTable[], size_t size, uint32_t adcValue);

void UpdateSensorInformation(SensorDataContainerType* sensors, SensorDataType distances, point_t roverLocation, float orientation);

void ConvertShortRangeToCM(float* distanceCM, uint32_t adcValue);

void ConvertMidRangeToCM(float* distanceCM, uint32_t adcValue);

void ConvertBottomLeftLongRangeIRToCM(float* distanceCM, uint32_t adcValue);

void ConvertBottomRightLongRangeIRToCM(float* distanceCM, uint32_t adcValue);

void ConvertBottomFarLeftLongRangeIRToCM(float* distanceCM, uint32_t adcValue);

void ConvertBottomFarRightLongRangeIRToCM(float* distanceCM, uint32_t adcValue);

void ConvertTopLeftLongRangeIRToCM(float* distanceCM, uint32_t adcValue);

void ConvertTopRightLongRangeIRToCM(float* distanceCM, uint32_t adcValue);

bool FilterIRSensors(SensorDataType sensors);

void UpdateProximityInformation(Proximity_t *proximity, SensorDataType sensors);

#endif /* _SENSOR_THREAD_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

