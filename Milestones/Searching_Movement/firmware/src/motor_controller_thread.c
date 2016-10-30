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
#include "peripheral/oc/plib_oc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

static QueueHandle_t _queue;

#define TYPEOFQUEUE MotorObj
#define SIZEOFQUEUE 32

static int rightCount = 0;
static int leftCount = 0;

static int rightSpeed = 28347;
static int leftSpeed = 28347;

static float totalDistance = 0;
static float initialOrientation = 0;
static float orientation = 0;
static bool motionComplete = true;

static int integral = 0;

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
    DRV_OC0_Start();
    DRV_OC1_Start();
}

/******************************************************************************
  Function:
    void MOTOR_CONTROLLER_THREAD_Tasks ( void )

  Remarks:
    See prototype in motor_controller_thread.h.
 */

void MOTOR_CONTROLLER_THREAD_Tasks ( void )
{
    MessageObj messageObj;
    MotorObj motorObj;
    
    int prevRightCount = 0;
    int prevLeftCount = 0;
    int deltaRight = 0;
    int deltaLeft = 0;
    int tempCount = 0;
    float x = 0;
    float y = 0;
    int rightSign = 1;
    int leftSign = 1;
    float distance = 0;
    int orientationCorrection = 0;
        
    messageObj.Type = UPDATE;
    messageObj.Update.Type = POSITION;

    enum states {
        init, controller
    } state;

    state = init;
    
    while(1) {
        /* Check the application's current state. */
        switch (state)
        {
            /* Application's initial state. */
            case init:
            {
                // Initialize motors - OFF and FORWARD
                disableMotors();
                setDirectionForward();
                state = controller;
                break;
            }
            case controller:
            {
                // Read direction of travel from queue
                if (motionComplete) {
                    dbgOutputLoc(BEFORE_RECEIVE_FR_QUEUE_MOTORCONTROLLERTHREAD);
                    MOTOR_CONTROLLER_THREAD_ReadFromQueue(&motorObj);
                    dbgOutputLoc(AFTER_RECEIVE_FR_QUEUE_MOTORCONTROLLERTHREAD);
                    motionComplete = false;
                }
                
                // Determine direction to travel
                switch(motorObj.direction) {
                    case 'F': {
                        setDirectionForward();
                        enableMotors();
                        rightSign = 1;
                        leftSign = 1;
                        
                        // Stop if desired distance is traveled
                        if (totalDistance > 10) {
                            completeMotion();
                        }
                        break;
                    }
                    case 'B': {
                        setDirectionBack();
                        enableMotors();
                        rightSign = -1;
                        leftSign = -1;
                        
                        // Stop if desired distance is traveled
                        if (totalDistance < -10) {
                            completeMotion();
                        }
                        break;
                    }
                    case 'L': {
                        setDirectionLeft();
                        enableMotors();
                        rightSign = 1;
                        leftSign = -1;
                        
                        // Correct orientation if it becomes greater than 360
                        if (orientation < initialOrientation) {
                            orientationCorrection = 360;
                        } else {
                            orientationCorrection = 0;
                        }
                        
                        // Stop rotating when desired angular displacement is achieved
                        if ((orientation + orientationCorrection - initialOrientation) > 90) {
                            completeMotion();
                        }
                        break;
                    }
                    case 'R': {
                        setDirectionRight();
                        enableMotors();
                        rightSign = -1;
                        leftSign = 1;
                        
                        // Correct orientation if it becomes less than 360
                        if (orientation > initialOrientation) {
                            orientationCorrection = -360;
                        } else {
                            orientationCorrection = 0;
                        }
                        
                        // Stop rotating when desired angular displacement is achieved
                        if ((orientation + orientationCorrection - initialOrientation) < -90) {
                            completeMotion();
                        }
                        break;
                    }
                    case 'S': {
                        // Stop motors
                        disableMotors();
                        motionComplete = true;
                        break;
                    }
                    default: {
                        break;
                    }
                }
                
                // Get encoder values from each motor
//                rightCount = PLIB_TMR_Counter16BitGet(TMR_ID_3) * rightSign;
//                leftCount = PLIB_TMR_Counter16BitGet(TMR_ID_4) * leftSign;
                
                deltaRight = (rightCount - prevRightCount) * rightSign;
                deltaLeft = (leftCount - prevLeftCount) * leftSign;
                
                // Clear encoder counters
//                PLIB_TMR_Counter16BitClear(TMR_ID_3);
//                PLIB_TMR_Counter16BitClear(TMR_ID_4);
                
                prevRightCount = rightCount;
                prevLeftCount = leftCount;
                
                // Calculate distance traveled
                distance = (((deltaRight+deltaLeft)/2.0)/ticksPerCm);
                totalDistance += distance;
                
                // Update position of rover - location & orientation
                x += distance*cos(orientation*2*M_PI/360.0);
                y += distance*sin(orientation*2*M_PI/360.0);
                
                orientation += (((((deltaRight-deltaLeft)/2.0)/ticksPerCm)/circumference)*360.0);
                
                // Roll over orientation if it is greater than 360
                if (orientation > 360) {
                    orientation -= 360;
                } else if (orientation < -360) {
                    orientation += 360;
                }
                
                // Send updated position to message controller thread
                messageObj.Update.Data.location.x = x;
                messageObj.Update.Data.location.y = y;
                messageObj.Update.Data.orientation = orientation;
                MESSAGE_CONTROLLER_THREAD_SendToQueue(messageObj);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void MOTOR_CONTROLLER_THREAD_InitializeQueue(void) {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
    if(_queue == 0) {
        /*Handle this Error*/
        dbgOutputBlock(pdFALSE);
    }
}

void MOTOR_CONTROLLER_THREAD_ReadFromQueue(MotorObj* obj) {
    xQueueReceive(_queue, obj, portMAX_DELAY);
}

void MOTOR_CONTROLLER_THREAD_SendToQueue(MotorObj obj) {
    xQueueSend(_queue, &obj, portMAX_DELAY);
}

void MOTOR_CONTROLLER_THREAD_SendToQueueISR(MotorObj obj, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendFromISR(_queue, &obj, pxHigherPriorityTaskWoken);
}

void MOTOR_CONTROLLER_THREAD_CorrectSpeed(void) {
    float error = 0;
    float output = 0;
    float Kp = 1000;
    float Ki = 100;
    
    error = rightCount - leftCount;
    integral += error;
    output = rightSpeed + ((Kp*error) + (Ki*integral));
    
    if (output > 31497) {
        output = 31497;
    }
    
    leftSpeed = (int)output;
}

void MOTOR_CONTROLLER_THREAD_IncrementRight(void) {
    rightCount++;
    dbgOutputVal('R');
    dbgOutputVal(rightCount);
    dbgOutputVal('R');
}

void MOTOR_CONTROLLER_THREAD_IncrementLeft(void) {
    leftCount++;
    dbgOutputVal('L');
    dbgOutputVal(leftCount);
    dbgOutputVal('L');
}

void completeMotion(void) {
    disableMotors();
    motionComplete = true;
    totalDistance = 0;
    initialOrientation = orientation;
    
}

void enableMotors(void) {
    PLIB_OC_PulseWidth16BitSet(OC_ID_1, rightSpeed);
    PLIB_OC_PulseWidth16BitSet(OC_ID_2, leftSpeed);
}

void disableMotors(void) {
    PLIB_OC_PulseWidth16BitSet(OC_ID_1, 0);
    PLIB_OC_PulseWidth16BitSet(OC_ID_2, 0);
}

void setDirectionForward(void) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 0);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 1);
}

void setDirectionBack(void) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 1);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 0);
}

void setDirectionLeft(void) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 0);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 0);
}

void setDirectionRight(void) {
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_C, 14, 1);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, 1, 1);
}

/*******************************************************************************
 End of File
 */
