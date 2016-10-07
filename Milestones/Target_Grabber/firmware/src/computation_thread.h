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
    COMPUTATION_THREAD_STATE_TARGET_ACQUIRE,                  // 3
    COMPUTATION_THREAD_STATE_REQUEST_OUTSIDE_OF_ARENA,        // 4
    COMPUTATION_THREAD_STATE_TARGET_DROP                      // 5
            
	/* TODO: Define states used by the application state machine. */

} COMPUTATION_THREAD_STATES;

// This holds the values that will be converted to a string to send to the PathFinder
typedef enum AlignmentResult_enum { NOT_ALIGNED = 0,       // 0 
                                    SLIGHT_RIGHT,          // 1
                                    SLIGHT_LEFT,           // 2
                                    MAJOR_RIGHT,           // 3
                                    MAJOR_LEFT,            // 4
                                    ALIGNED_NOT_CLOSE,     // 5
                                    ALIGNED                // 6
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
/* Inputs:  MessageObj pointer to read the current alignment values       */
/* Outputs: 2 if the target is aligned and close enough to grab           */
/*          1 if the target is not close enough                           */
/*          0 if the target is not aligned                                */
//int determineAlignment(MessageObj* pvBufferMSG);
int determineAlignment(bool ir_0_bool, bool ir_1_bool, bool ir_2_bool, float ir_0, float ir_1, float ir_2);

#endif /* _COMPUTATION_THREAD_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

