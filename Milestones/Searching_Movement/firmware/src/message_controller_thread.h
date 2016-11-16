/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    message_controller_thread.h

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

#ifndef _MESSAGE_CONTROLLER_THREAD_H
#define _MESSAGE_CONTROLLER_THREAD_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "communication/messages.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "adc_thread_public.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif

//Depending on what data you would need to send out you should create a member of this struct that can hold that data

typedef struct {
    float x;
    float y;
} Coordinates;

typedef enum ActionType { FORWARD, BACKWARD, RIGHT, LEFT } Action;

typedef struct {
    Action action;
    float amount;
} Movement;

//This is any data that someone else might request for
typedef struct {
    Coordinates location;
    float orientation;
    Movement movement;
    LineObj lineLocation;
} InternalData;

//This should include a new enum for anything in the InternalData
typedef enum UpdateType_enum { MOVEMENT, LINELOCATION } UpdateType;

//This is any request that you would want to ... well ... request
// these are really the categories of requests. the specific objects in them is added later
typedef enum InternalRequestType_enum { SMtoTL,
                                        TLtoSM, 
                                        TLtoPF, 
                                        PFtoTL, 
                                        PFtoTG, 
                                        TGtoPF,
                                        SD
 } InternalRequestType;

//------------------------------------------------------------------------------
//You should not need to change anything beyond this point
//------------------------------------------------------------------------------
typedef enum MessageItemType_enum {EXTERNAL_REQUEST_RESPONSE, SEND_REQUEST, UPDATE} MessageItemType;

typedef struct {
    //Set this to
    UpdateType Type;
    InternalData Data;
} UpdateObj;

typedef struct {
    char Source;
    bool Error;
    char Data[MAXMESSAGESIZE];
    char MessageCount;
} ExternalObj;

typedef struct {
    //This is set to update when we are updating internal information
    //It should be set to Request_response when we get an external message
    MessageItemType Type;
    //This source is set when we get a request or response from an external source
    ExternalObj External;
    //If this object is a SENDOUT_REQUEST
    InternalRequestType Request;
    //If if the MessageItemType is set to update then we will access the Update object and see what the update is from
    UpdateObj Update;
} MessageObj;

typedef struct {
    unsigned char Req_From_SearcherMover;
    unsigned char Req_From_TargetLocator;
    unsigned char Req_From_PathFinder;
    unsigned char Req_From_TargetGrabber;
    
    unsigned char Req_To_SearcherMover;
    unsigned char Req_To_TargetLocator;
    unsigned char Req_To_PathFinder;
    unsigned char Req_To_TargetGrabber;

    unsigned char Res_From_SearcherMover;
    unsigned char Res_From_TargetLocator;
    unsigned char Res_From_PathFinder;
    unsigned char Res_From_TargetGrabber;

    unsigned char Res_To_SearcherMover;
    unsigned char Res_To_TargetLocator;
    unsigned char Res_To_PathFinder;
    unsigned char Res_To_TargetGrabber;
    
    int32_t PacketsDropped;
    int32_t ErrorCount;
    int32_t GoodCount;
} StatObjectType;
/*******************************************************************************
  Function:
    void MESSAGE_CONTROLLER_THREAD_Initialize ( void )

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
    MESSAGE_CONTROLLER_THREAD_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void MESSAGE_CONTROLLER_THREAD_Initialize ( void );


/*******************************************************************************
  Function:
    void MESSAGE_CONTROLLER_THREAD_Tasks ( void )

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
    MESSAGE_CONTROLLER_THREAD_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void MESSAGE_CONTROLLER_THREAD_Tasks( void );

void MESSAGE_CONTROLLER_THREAD_InitializeQueue();

void MESSAGE_CONTROLLER_THREAD_ReadFromQueue(MessageObj* pvBuffer);

void resetSystemClock();

void incrementSystemClock();

int getSystemClock();

#endif /* _MESSAGE_CONTROLLER_THREAD_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */
