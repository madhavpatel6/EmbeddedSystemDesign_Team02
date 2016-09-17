/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    grabberthread.c

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

#include "grabberthread.h"

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

GRABBERTHREAD_DATA grabberthreadData;
GRABBER_ISR_DATA grabber_ISRData;
GRABBER_CMD_DATA grabber_CMDData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

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
    void GRABBERTHREAD_Initialize ( void )

  Remarks:
    See prototype in grabberthread.h.
 */

void GRABBERTHREAD_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    //grabberthreadData.state = GRABBERTHREAD_STATE_INIT;
    grabber_ISRData.grabber_ISR_Q = createGrabberISRQ();
    grabber_CMDData.grabber_CMD_Q = createGrabberCMDQ();
    // DRV_ADC_Open(); // Might not need this due to being called in computation thread

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/* Following functions are for the Grabber Thread */
/* GRABBER_ISR_Q_SIZE can be changed in grabberthread_public.h */
QueueHandle_t createGrabberISRQ(){
    return xQueueCreate(10,sizeof(GRABBER_ISR_Q_SIZE)); 
}

/* GRABBER_CMD_Q_SIZE can be changed in grabberthread_public.h */
QueueHandle_t createGrabberCMDQ(){
    return xQueueCreate(10,sizeof(GRABBER_CMD_Q_SIZE)); 
}

/* Sends value from computation or Rx thread to grabber CMD Queue */
int grabberthread_SendToCMDQ(char grabberCMD){
    return xQueueSend(grabber_CMDData.grabber_CMD_Q, &grabberCMD, portMAX_DELAY);
}

/* Sends value from grabber thread to grabber thread Queue for the physical grabber */
int grabberthread_SendValToISRQ(float grabberAdcVal){
    return xQueueSend(grabber_ISRData.grabber_ISR_Q, &grabberAdcVal, portMAX_DELAY);
}
///* Sends value from grabber thread to grabber thread Queue for the physical grabber */
//int grabberthread_SendValToISRQ(float grabberAdcVal, BaseType_t *pxHigherPriorityTaskWoken){
//    return xQueueSendFromISR(grabber_ISRData.grabber_ISR_Q, &grabberAdcVal, pxHigherPriorityTaskWoken);
//}

/******************************************************************************
  Function:
    void GRABBERTHREAD_Tasks ( void )

  Remarks:
    See prototype in grabberthread.h.
 */

void GRABBERTHREAD_Tasks ( void )
{
    dbgOutputLoc(ENTER_GRABBER_THREAD);
    char grabberCMDRecv;
    dbgOutputLoc(BEFORE_WHILE_GRABBER_THREAD);
    while(1){
        dbgOutputLoc(BEFORE_RECEIVE_FROM_Q_GRABBER_THREAD);
        if(xQueueReceive(grabber_CMDData.grabber_CMD_Q, &grabberCMDRecv, portMAX_DELAY)){
            UARTTXTHREAD_SendToQueue(grabberCMDRecv); // Sending to Tx Thread Q
        }
        dbgOutputLoc(AFTER_RECEIVE_FROM_Q_GRABBER_THREAD);
    }

//    /* Check the application's current state. */
//    switch ( grabberthreadData.state )
//    {
//        /* Application's initial state. */
//        case GRABBERTHREAD_STATE_INIT:
//        {
//            bool appInitialized = true;
//       
//        
//            if (appInitialized)
//            {
//            
//                grabberthreadData.state = GRABBERTHREAD_STATE_SERVICE_TASKS;
//            }
//            break;
//        }
//
//        case GRABBERTHREAD_STATE_SERVICE_TASKS:
//        {
//        
//            break;
//        }
//
//        /* TODO: implement your application state machine.*/
//        
//
//        /* The default state should never be executed. */
//        default:
//        {
//            /* TODO: Handle error in application's state machine. */
//            break;
//        }
//    }
}

 

/*******************************************************************************
 End of File
 */
