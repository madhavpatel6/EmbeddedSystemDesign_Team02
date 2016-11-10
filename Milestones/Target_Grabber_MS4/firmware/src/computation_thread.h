/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    computation_thread.h

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

#ifndef _COMPUTATION_THREAD_H
#define _COMPUTATION_THREAD_H

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

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/
    
typedef enum
{
	/* Application's state machine's initial state. */
	COMPUTATION_THREAD_STATE_INIT=0,                          // 0
    COMPUTATION_THREAD_STATE_REQUEST_TARGET_PROXIMITY,        // 1
    COMPUTATION_THREAD_STATE_TARGET_ALIGNMENT,                // 2
    COMPUTATION_THREAD_STATE_MOVEMENT_STOPPED,                // 3
    COMPUTATION_THREAD_STATE_TARGET_ACQUIRE,                  // 4
    COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA,        // 5
    COMPUTATION_THREAD_STATE_TARGET_DROP                      // 6 
            
	/* TODO: Define states used by the application state machine. */

} COMPUTATION_THREAD_STATES;

// This holds the values that will be converted to a string to send to the PathFinder
typedef enum AlignmentResult_enum { NOT_ALIGNED = 0,       // 0 
                                    SLIGHT_RIGHT,          // 1
                                    SLIGHT_LEFT,           // 2
                                    MAJOR_RIGHT,           // 3
                                    MAJOR_LEFT,            // 4
                                    ALIGNED_NOT_CLOSE,     // 5
                                    ALIGNED,               // 6
                                    OBSTACLE               // 7
} ALIGNMENT_RESULTS;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    COMPUTATION_THREAD_STATES state;

    /* TODO: Define any additional data used by the application. */
    
    ALIGNMENT_RESULTS alignmentResult;

} COMPUTATION_THREAD_DATA;


// This define is for the sensor mounted distance when it is on the rover,
// it might need to be changed based on its physical mounted location
#define SENSOR_MOUNT_DISTANCE 6.250f

// This define is for the outter most mounted sensors EX; IR0 & IR4
#define SENSOR_MOUNT_OUTTER 3.810f

// This define is for the inner most mounted sensors EX; IR1 & IR3
#define SENSOR_MOUNT_INNER 1.905f

// This define is for the target midway point
// Entire target max diameter is 5.38 / 2 = 2.69
#define SENSOR_MOUNT_MIDWAY_TARGET 2.690f

// This define is for the midway point between inner and outter mounted sensors 
//                      EX; IR4 & IR3 or IR0 & IR1
// Entire target max diameter is 1.905 + (1.905 / 2)  = 2.8575
#define SENSOR_MOUNT_MIDWAY_OUTTER_EXACT 2.858f

// This define is for the midway point between inner and outter mounted sensors 
//                      EX; IR2 & IR3 or IR2 & IR1
// Entire target max diameter is 1.905 / 2  = 0.9525
#define SENSOR_MOUNT_MIDWAY_INNER_EXACT 0.953f

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void COMPUTATION_THREAD_Initialize ( void )

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
    COMPUTATION_THREAD_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void COMPUTATION_THREAD_Initialize ( void );


/*******************************************************************************
  Function:
    void COMPUTATION_THREAD_Tasks ( void )

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
    COMPUTATION_THREAD_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void COMPUTATION_THREAD_Tasks( void );


QueueHandle_t createComputationQ();


/* This converts the ultrasonic sensor values to cm for 10cm - 80cm */
void convertTocmUltra(float *ultraDigitalVal);


/* This converts the IR sensor values to cm for ~3.42cm -~ 20cm       */
/* This uses Chris' equation :             1                          */
/*                            ---------------------------------       */
/*                            (AnalogVoltage - 0.2243)/(9.6762)       */
/* Analog voltage is found by converting the digital value to analog  */
/* Inputs a uint32_t digital value from ADC                           */
/* Outputs float converted digital value to cm                        */
float convertTocmIR(uint32_t irDigitalVal); // return float


/* Determines if the converted cm value is reasonable                   */
/* Checks to make sure the value is within reading via Chris equation   */
/* and datasheet.  Also makes sure the distance is within 6" range      */
/* Inputs: float converted cm value                                     */
/* Outputs: true or false based on cm value                             */
bool cmValChecker(float cmVal);


/* This subroutine handles the target alignment data             */
/* Which pertains to scanning AN0, AN1, AN2 on the ADC           */
/* The ADC is in scan mode with autosampling but it is enabled   */
/* Only when TMR2 fires which is every 50ms                      */
/* Inputs:  N/A                                                  */
/* Outputs: Boolean that returns true once the target is aligned */
/*          Updates the Message Controller Thread Queue to let   */
/* PathFinder have updated values of the location of the target  */
bool targetAlignmentSubr( void );


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
                       float *thetaDegree);


/* This subroutine will progress the state machine to target alignment state    */
/* It reads the computation thread queue for the proximity distance value       */
/* from the PathFinder                                                          */
/* Inputs: N/A                                                                  */
/* Outputs: This function returns true if the PathFinder sends a value of       */
/* 6 inches or lower                                                            */
bool targetProximitySubr( void );


/* This subroutine will progress the state machine to target acquire   state    */
/* It reads the computation thread queue for the movement stopped value         */
/* from the PathFinder                                                          */
/* Inputs: N/A                                                                  */
/* Outputs: This function returns true if the PathFinder sends a value of       */
/* true                                                                         */
bool movementStoppedSubr( void );


/* This function turns the grabber on to grab or drop the target                */
/* Inputs: Char that determines the action of the grabber. GRAB or DROP         */
/* Outputs: Boolean to advance the state machine                                */
bool grabberActionSubr(char action);

/* This subroutine will progress the state machine to target drop state         */
/* It reads the computation thread queue for the outside arena value            */
/* from the PathFinder                                                          */
/* Inputs: N/A                                                                  */
/* Outputs: This function returns true if the PathFinder sends a value of       */
/* true                                                                         */
bool outsideArenaSubr( void );

/* This function will close the grabber enough to grab a target                 */
/* 430 was chosen due to the target size and for the grabber to close enough    */
/* to hold onto the target.                                                     */
/* Inputs: N/A                                                                  */
/* Outputs: Boolean to let caller function know its task is complete            */
bool targetServo(void);

/* This function will open the servo motor completely                           */
/* 500 was chosen because due to physical restraints of the grabber 500 opens   */
/* the grabber to its fullest.                                                  */
/* Inputs: N/A                                                                  */
/* Outputs: Boolean to let caller function know its task is complete            */
bool openServo(void);

/* This function will power the servo to close the grabber all the way          */
/* Inputs: N/A                                                                  */
/* Outputs: Boolean to let caller function know its task is complete            */
bool closeServo(void);

/* This function will filter out data that does not concern the alignment subr  */
/* at the current time.  Ex. say that two objects are being seen but one is     */
/* behind the other.  Therefore I only care about the object in the front.      */
/* Inputs:  All Five IR values and booleans that determine if the         */
/* values are within the correct reading range of the sensors             */
/* Outputs: */
void filterIRData(bool toBefilterIR_0_bool, bool toBefilterIR_1_bool, bool toBefilterIR_2_bool, bool toBefilterIR_3_bool, 
                   bool toBefilterIR_4_bool, float toBefilterIR_0, float toBefilterIR_1, float toBefilterIR_2, float toBefilterIR_3, 
                   float toBefilterIR_4, bool *filterIR_0_bool, bool *filterIR_1_bool, bool *filterIR_2_bool, 
                   bool *filterIR_3_bool, bool *filterIR_4_bool, float *filterIR_0, float *filterIR_1, 
                   float *filterIR_2, float *filterIR_3, float *filterIR_4);

#endif /* _COMPUTATION_THREAD_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

