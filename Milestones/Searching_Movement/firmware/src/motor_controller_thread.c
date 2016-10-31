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

static int rightSpeed = 52430;
static int leftSpeed = 52430;

static float totalDistance = 0;
static float initialOrientation = 0;
static float orientation = 0;
static bool motionComplete = true;

static float integralLeft = 0;

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
    int weightedDiff = 0;
    float x = 0;
    float y = 0;
    int rightSign = 1;
    int leftSign = 1;
    float distance = 0;
    int orientationCorrection = 0;
        
    messageObj.Type = UPDATE;
    messageObj.Update.Type = POSITION;

    enum states {
        forward, inchForward, inchBackward, turnRight, turnLeft, followLine
    } state, prevState;

    state = forward;
    prevState = state;
    disableMotors();
    setDirectionForward();
    
    while(1) {
        if (MODE == "Debug") {
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
                    if (totalDistance > motorObj.distance) {
                        completeMotion();
                    }
                    break;
                }
                case 'B': {
                    setDirectionBackward();
                    enableMotors();
                    rightSign = -1;
                    leftSign = -1;
                    
                    // Stop if desired distance is traveled
                    if (totalDistance < -motorObj.distance) {
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
                    if ((orientation + orientationCorrection - initialOrientation) > motorObj.degrees) {
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
                    if ((orientation + orientationCorrection - initialOrientation) < -motorObj.degrees) {
                        completeMotion();
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        } else {
            dbgOutputLoc(BEFORE_RECEIVE_FR_QUEUE_MOTORCONTROLLERTHREAD);
            MOTOR_CONTROLLER_THREAD_ReadFromQueue(&motorObj);
            dbgOutputLoc(AFTER_RECEIVE_FR_QUEUE_MOTORCONTROLLERTHREAD);
            
            switch (state) {
                case forward: {
                    if (motorObj.lineLocation == 0) {
                        setDirectionForward();
                        enableMotors();
                        rightSign = 1;
                        leftSign = 1;
                    } else {
                        completeMotion();
                        prevState = forward;
                        state = inchBackward;
                    }
                    break;
                }
                case inchForward: {
                    setDirectionForward();
                    enableMotors();
                    rightSign = 1;
                    leftSign = 1;

                    if (totalDistance > 15) {
                        completeMotion();
                        
                        if (prevState == turnRight) {
                            prevState = inchForward;
                            state = turnRight;
                        } else if (prevState == turnLeft) {
                            prevState = inchForward;
                            state = turnLeft;
                        }
                        
                    }
                    break;
                }
                case inchBackward: {
                    setDirectionBackward();
                    enableMotors();
                    rightSign = -1;
                    leftSign = -1;

                    if (totalDistance < -3) {
                        completeMotion();
                        prevState = inchBackward;
                        state = turnRight;
                    }
                    break;
                }
                case turnRight: {
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
                        if (prevState == inchBackward) {
                            prevState = turnRight;
                            state = inchForward;
                        } else if (prevState == inchForward) {
                            prevState = turnRight;
                            state = forward;
                        }
                    }
                    break;
                }
                case turnLeft: {
                    setDirectionLeft();
                    enableMotors();
                    rightSign = 1;
                    leftSign = -1;

                    // Correct orientation if it becomes less than 360
                    if (orientation < initialOrientation) {
                        orientationCorrection = 360;
                    } else {
                        orientationCorrection = 0;
                    }
                    
                    // Stop rotating when desired angular displacement is achieved
                    if ((orientation + orientationCorrection - initialOrientation) > 90) {
                        completeMotion();
                        if (prevState == inchBackward) {
                            prevState = turnLeft;
                            state = inchForward;
                        } else if (prevState == inchForward) {
                            prevState = turnLeft;
                            state = forward;
                        }
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
        
        deltaRight = (rightCount - prevRightCount) * rightSign;
        deltaLeft = (leftCount - prevLeftCount) * leftSign;
        
        prevRightCount = rightCount;
        prevLeftCount = leftCount;
        
        switch(motorObj.direction) {
            case 'F': case 'B': {
                weightedDiff = (deltaRight - deltaLeft)/2.0;
                deltaLeft = deltaRight - weightedDiff;
                break;
            }
        }
        
        // Calculate distance traveled
        distance = (((deltaRight+deltaLeft)/2.0)/TICKS_PER_CM);
        totalDistance += distance;
        
        // Update position of rover - location & orientation
        x += distance*cos(orientation*2*M_PI/360.0);
        y += distance*sin(orientation*2*M_PI/360.0);
        
        orientation += (((((deltaRight-deltaLeft)/2.0)/TICKS_PER_CM)/CIRCUMFERENCE)*360.0);
        
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

int prevRightPID = 0;
int prevLeftPID = 0;

void MOTOR_CONTROLLER_THREAD_CorrectSpeed(int timer) {
    float errorLeft = 0;
    float outputLeft = 0;
    
    float Kp_Left = 6000;
    float Ki_Left = 2500;
    
    errorLeft = (rightCount - prevRightPID) - (leftCount - prevLeftPID);
    integralLeft += errorLeft;
    outputLeft = ((Kp_Left*errorLeft) + (Ki_Left*integralLeft));
    
    if (outputLeft < 0) {
        outputLeft = 0;
    }
    if (outputLeft > MAX_PWM) {
        outputLeft = MAX_PWM;
    }
    
    leftSpeed = (int)outputLeft;
    
    if(timer % 10 == 0) {
            Tx_Thead_Queue_DataType tx_thread_obj;
            memset(&tx_thread_obj, 0, sizeof(Tx_Thead_Queue_DataType));
            tx_thread_obj.Destination = TARGETLOCATOR;
            BaseType_t *ptr;
            sprintf(tx_thread_obj.Data, "{\"type\": \"PID\", \"motor\": \"1\", \"vel\": \"%4d\", \"time\": \"%4d\", \"output\": \"%4f\", \"pwm\": \"%4d\"}", leftCount - prevLeftPID, timer, outputLeft, leftSpeed);
            TX_THREAD_SendToQueueISR(tx_thread_obj, ptr);
            sprintf(tx_thread_obj.Data, "{\"type\": \"PID\", \"motor\": \"2\", \"vel\": \"%4d\", \"time\": \"%4d\"}", rightCount - prevRightPID, timer);
            TX_THREAD_SendToQueueISR(tx_thread_obj, ptr);
    }
    
    prevRightPID = rightCount;
    prevLeftPID = leftCount;
}

void MOTOR_CONTROLLER_THREAD_IncrementRight(void) {
    rightCount++;
}

void MOTOR_CONTROLLER_THREAD_IncrementLeft(void) {
    leftCount++;
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

void setDirectionBackward(void) {
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
