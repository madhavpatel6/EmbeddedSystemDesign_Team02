/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    motor_controller_thread.c

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

#include "debug.h"
#include "motor_controller_thread.h"
#include "motor_controller_thread_public.h"
#include "math.h"

static QueueHandle_t _queue;

#define TYPEOFQUEUE message_in_t
#define SIZEOFQUEUE 36

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

MOTOR_CONTROLLER_THREAD_DATA motor_controller_threadData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void MOTOR_CONTROLLER_THREAD_Initialize ( void )

  Remarks:
    See prototype in motor_controller_thread.h.
 */

void MOTOR_CONTROLLER_THREAD_Initialize ( void )
{
    MOTOR_CONTROLLER_THREAD_InitializeQueue();
    DRV_TMR2_Start();
    DRV_TMR3_Start();
    motor_controller_threadData.state = MOTOR_CONTROLLER_THREAD_STATE_INIT;
    DRV_OC0_Start();
    DRV_OC1_Start();
    // i think DRV_TMR0 (timer2 for our OC) is started somewhere else
}

/******************************************************************************
  Function:
    void MOTOR_CONTROLLER_THREAD_Tasks ( void )

  Remarks:
    See prototype in motor_controller_thread.h.
 */

void MOTOR_CONTROLLER_THREAD_Tasks ( void )
{
    message_in_t msg;
    
    while(1) {
        /* Check the application's current state. */
        switch ( motor_controller_threadData.state )
        {
            /* Application's initial state. */
            case MOTOR_CONTROLLER_THREAD_STATE_INIT:
            {
                disableMotors();
                setDirectionForward();
                initWorld();
                motor_controller_threadData.state = MOTOR_CONTROLLER_THREAD_STATE_SERVICE_TASKS;
                break;
            }
            case MOTOR_CONTROLLER_THREAD_STATE_SERVICE_TASKS:
            {
                // Read direction of travel from queue
                MOTOR_CONTROLLER_THREAD_ReadFromQueue(&msg);
                addToMap(msg);
                
                break;
            }
            default:
            {
                break;
            }
        }
    }
}
                /* the following commented out stuff is helpful stuff from eric add it back later maybe go check his project*/
//                distance = (((leftCount+rightCount)/2.0)/ticksPerCm);
//                totalDistance += distance;
//                
//                x += distance*cos(orientation*2*M_PI/360);
//                y += distance*sin(orientation*2*M_PI/360);
//                
//                // Update position of rover
//                orientation += (((((rightCount-leftCount)/2.0)/ticksPerCm)/circumference)*360);
//                if (orientation > 360 || orientation < -360) {
//                    orientation -= ((int)orientation/360)*360;
//                    orientation *= 360;
//                }
//MessageObj obj;
//obj.Type = UPDATE;
//obj.Update.Type = POSITION;
//                obj.Update.Data.location.x = x;
//                obj.Update.Data.location.y = y;
//                obj.Update.Data.orientation = orientation;
//                
//                MESSAGE_CONTROLLER_THREAD_SendToQueue(obj);

void MOTOR_CONTROLLER_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
    if(_queue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

void MOTOR_CONTROLLER_THREAD_ReadFromQueue(message_in_t* pvBuffer) {
    xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
}

void MOTOR_CONTROLLER_THREAD_SendToQueue(message_in_t buffer) {
    xQueueSend(_queue, &buffer, portMAX_DELAY);
}

void MOTOR_CONTROLLER_THREAD_SendToQueueISR(message_in_t buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendFromISR(_queue, &buffer, pxHigherPriorityTaskWoken);
}

void enableMotors( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, 0, 1);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, 1, 1);
}

void disableMotors( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, 0, 0);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_D, 1, 0);
}

void setDirectionForward( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 0);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 0);
}

void setDirectionBack( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 1);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 1);
}

void setDirectionLeft( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 0); // right forward
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 1); // left back
}

void setDirectionRight( void ) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 1); // right back
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 0); // left forward
}
void addVerticesToMap(float arrX[], float arrY[], int len){
    int i;
    Point temp;
    for(i = 0; i < len; i++){
        temp.x = arrX[i];
        temp.y = arrY[i];
        addVertex(temp);
    }
    
    
}
void addObstacleToMap(float arrX[], float arrY[], int len){
    // traverse the list of a_star obstacles
    
    // if this one is different from all of them add it
}
void addTargetToMap(float arrX[], float arrY[], int len){
    // traverse the list of a_star vertices
    
    // if this on is new add it
}

void addToMap(message_in_t msg){
    switch(msg.type){
        case vertex:{
            addVerticesToMap(msg.x, msg.y, msg.len);
            break;
        }
        case obstacle:{
            addObstacleToMap(msg.x, msg.y, msg.len);
            break;
        }
        case target:{
            addTargetToMap(msg.x, msg.y, msg.len);
            break;
        }
        default:{
            
        }
    }
}

/*******************************************************************************
 End of File
 */
