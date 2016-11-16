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

#define MIN_ANGLE -5
#define MAX_ANGLE 5
#define FAR_DIST 5
#define CLOSE_DIST 1


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


static Point myLoc;
static Point path[100];
static int pathLen = 0;
static int pathIndex = 0;
static bool inProg = false;
static bool holdingTarg = false;
static  int myDir = 0;
static int desiredDir = 0;

void MOTOR_CONTROLLER_THREAD_Tasks ( void )
{
    message_in_t msg;
    
    initWorld();
    // lets start at 9 9 cuz were starting at a target
    myLoc.x = 6;
    myLoc.y = 6;
//    int myDir = 0;
//    int desiredDir = 0;
    
//    Point path[100];
//    int pathLen = 0;
//    int pathIndex = 0;
//    bool inProg = false;
//    bool holdingTarg = false;
    
    float driftAng = 0;
    float driftLocX = 0;
    float driftLocY = 0;
    while(1) {
        
        /* Check the application's current state. */
        switch ( motor_controller_threadData.state )
        {
            /* Application's initial state. */
            case MOTOR_CONTROLLER_THREAD_STATE_INIT:
            {
                disableMotors();
                setDirectionForward();
                addMotorTask(2, 3); // get on edge
                motor_controller_threadData.state = tuneDir;
                enableTrue();
                // motor_controller_threadData.state = spin;
                Nop();
                MessageObj upObj;
                upObj.Type = UPDATE;
                upObj.Update.Type = TARG_PROX;
                upObj.Update.Data.targetProx = 10.0f;
                MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                upObj.Update.Type = OUTS_ARENA;
                upObj.Update.Data.outsArena = false;
                MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                upObj.Update.Type = MOV_STOP;
                upObj.Update.Data.movStop = false;
                MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                break;
            }
            case wait_calc_path:
            {
                enableTrue();
                MOTOR_CONTROLLER_THREAD_ReadFromQueue(&msg);
                addToMap(msg);
                if(holdingTarg == false){
                    if(can_I_go_targ(myLoc)){
                        motor_controller_threadData.state = exec_path;
                        getPath(path, &pathLen);
                        pathIndex = 0;
                        inProg = false;
                        incStartingPoint();
                    }
                }else{
                    if(can_I_go_home(myLoc)){
                        motor_controller_threadData.state = exec_path;
                        getPath(path, &pathLen);
                        pathIndex = 0;
                        inProg = false;
                    }
                }
                Nop();
                
                break;
            }
            case exec_path:
            {   
                MessageObj upObj;
                upObj.Type = UPDATE;
                upObj.Update.Type = TARG_PROX;
                upObj.Update.Data.targetProx = 10.0f;
                MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                upObj.Type = UPDATE;
                upObj.Update.Type = MOV_STOP;
                upObj.Update.Data.movStop = false;
                MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                Nop();
                MOTOR_CONTROLLER_THREAD_ReadFromQueue(&msg);
                addToMap(msg);
                
                if(pathIndex >= pathLen){
                    if(holdingTarg == true){
                        motor_controller_threadData.state = waitAcquired;
                    }else{
                        motor_controller_threadData.state = tuneDir;
                    }
                }
                
                if (msg.type == update && msg.cacheType == 1){
                    myDir += msg.cacheDistance;
                    if(myDir < 0){
                        myDir = 360 + myDir;
                    }
                    myDir %= 360;
                    inProg = false;
                }else if( msg.type  == update && msg.cacheType == 0){
                     if(myDir == 0){
                         myLoc.x += msg.cacheDistance;
                     }else if(myDir == 90){
                         myLoc.y += msg.cacheDistance;
                     }else if(myDir == 180){
                         myLoc.x -= msg.cacheDistance;
                     }else if(myDir == 270){
                         myLoc.y -= msg.cacheDistance;
                     }
                     inProg = false;
                }
                
                float deltaX = path[pathIndex].x - myLoc.x;
                float deltaY = path[pathIndex].y - myLoc.y;
                
                if (deltaX == 0 && deltaY == 0){
                    // move on cuz we there
                    pathIndex++;
                    continue;
                }else if (deltaX == 0){
                    if(deltaY > 0){
                        desiredDir = 90;
                    }else{
                        desiredDir = 270;
                    }
                }else if (deltaY == 0){
                    if(deltaX > 0){
                        desiredDir = 0;
                    }else{
                        desiredDir = 180;
                    }
                }
                
// this was supposed to copy the below section       
//                if(desiredDir != myDir && desiredDir != ((myDir + 180) % 360)){
//                    if(desiredDir - myDir < 180 & desiredDir - myDir > 0){
//                        // turn left
//                        addMotorTask(1, 90);
//                    }else{
//                         // turn right
//                        addMotorTask(1, -90);
//                        
//                    }
//                }else{
//                    int distance = (deltaX == 0 ? deltaY: deltaX) ;
//                    distance = abs(distance);
//                    if( desiredDir != myDir){
//                        distance *= -1;
//                    }
//                    addMotorTask(0, distance);
//                }
                
   
// this is show things were beore pid
                if( !inProg && 
                        desiredDir != myDir && desiredDir != ((myDir + 180) % 360)){
                    if(desiredDir - myDir < 180 & desiredDir - myDir > 0){
                        // turn left
                        addMotorTask(1, 90);
                    }else{
                        // turn right
                        addMotorTask(1, -90);
                        
                    }
                    inProg = true;
                }else if( !inProg){
                    int distance = (deltaX == 0 ? deltaY: deltaX) ;
                    distance = abs(distance);
                    if( desiredDir != myDir){
                        distance *= -1;
                    }
                    addMotorTask(0, distance);
                    inProg = true;
                }
                
                
                 
                // freak out if something breaks your path, we can block on queue
                Nop();
                break;
                
            }
            case tuneDir:
            {
                MessageObj upObj;
                upObj.Type = UPDATE;
                upObj.Update.Type = TARG_PROX;
                upObj.Update.Data.targetProx = 5.9f;
                MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                upObj.Type = UPDATE;
                upObj.Update.Type = MOV_STOP;
                upObj.Update.Data.movStop = false;
                MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                MOTOR_CONTROLLER_THREAD_ReadFromQueue(&msg);
                enableFalse();
                // talk to warthog 1, find stuff, be fast and adaptive
                MOTOR_CONTROLLER_THREAD_ReadFromQueue(&msg);
                addToMap(msg);
                if(msg.type == targetAlignment){
                    if(msg.targetAligned == true){
                        setDirectionForward();
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 0);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 0);  
                        motor_controller_threadData.state = waitAcquired;
                    } else if(msg.targetAngle > MAX_ANGLE){
                        setDirectionLeft();
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 65535/3);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 65535/3);
                    }else if (msg.targetAngle < MIN_ANGLE){
                        setDirectionRight();
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 65535/3);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 65535/3);
                        driftAng -= 10;
                    }else{
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 0);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 0);
                        motor_controller_threadData.state = tuneDist;
                    }
                    driftAng += msg.targetAngle;
                    
                }
                break;
            }
            case tuneDist:
            {
                MOTOR_CONTROLLER_THREAD_ReadFromQueue(&msg);
                addToMap(msg);
                if(msg.type == targetAlignment){
                    if(msg.targetAngle > MAX_ANGLE || msg.targetAngle < MIN_ANGLE){
                        motor_controller_threadData.state = tuneDir;                    
                    }
                    
                    if(msg.targetAligned == true){
                        setDirectionForward();
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 0);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 0);  
                        motor_controller_threadData.state = waitAcquired;
                    } else if(msg.targetDistance > FAR_DIST){
                        setDirectionForward();
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 65535/3);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 65535/3);     
                        
                    } else if(msg.targetDistance > CLOSE_DIST){
                        setDirectionForward();
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 65535/4);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 65535/4);                  
                    } else if(msg.targetAligned == false){
                        setDirectionForward();
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 65535/4);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 65535/4); 
                    }else{
                        setDirectionForward();
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 0);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 0);  
                        motor_controller_threadData.state = waitAcquired;
                    } 
                    if(myDir == 0){
                        driftLocX += msg.targetDistance;
                    }else if(myDir == 90){
                        driftLocY += msg.targetDistance;
                    }else if(myDir == 180){
                        driftLocX -= msg.targetDistance;
                    }else if(myDir == 270){
                        driftLocX -= msg.targetDistance;
                    }
                }
                break;
            }
            case waitAcquired:
            {
                MessageObj upObj;
                upObj.Type = UPDATE;
                upObj.Update.Type = MOV_STOP;
                upObj.Update.Data.movStop = true;
                MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                
                if(holdingTarg == true){
                    upObj.Type = UPDATE;
                    upObj.Update.Type = OUTS_ARENA;
                    upObj.Update.Data.outsArena = true;
                    MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                }
                
                MOTOR_CONTROLLER_THREAD_ReadFromQueue(&msg);
                addToMap(msg);
                if(holdingTarg == false && msg.type == targetAlignment){
                    if(msg.targetAligned == false){
                        setDirectionForward();
                        PLIB_OC_PulseWidth16BitSet(OC_ID_1, 0);
                        PLIB_OC_PulseWidth16BitSet(OC_ID_2, 0); 
                        motor_controller_threadData.state = wait_calc_path;
                    }else if(msg.targetAcquired == true){
                        motor_controller_threadData.state = wait_calc_path;
                        holdingTarg = true;
                    }
                    
                }else if (msg.type == targetAlignment){
                    if(msg.targetAcquired == false){
                        holdingTarg = false;
                        motor_controller_threadData.state = wait_calc_path;
                                            upObj.Type = UPDATE;
                        upObj.Update.Type = OUTS_ARENA;
                        upObj.Update.Data.outsArena = false;
                        MESSAGE_CONTROLLER_THREAD_SendToQueue(upObj);
                    }
                }
                break;
            }
            case spin:
            {
                MOTOR_CONTROLLER_THREAD_ReadFromQueue(&msg);
                addToMap(msg);
                Nop();
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
    Point one;
    one.x = arrX[0];
    one.y = arrY[0];
    Point two;
    two.x = arrX[1];
    two.y = arrY[1];
    Point three;
    three.x = arrX[2];
    three.y = arrY[2];
    Point four;
    four.x = arrX[3];
    four.y = arrY[3];
    addObstacle(one, two, three, four);
}
void addTargetToMap(float arrX[], float arrY[], int len){
    Point one;
    one.x = arrX[0];
    one.y = arrY[0];
    Point two;
    two.x = arrX[1];
    two.y = arrY[1];
    Point three;
    three.x = arrX[2];
    three.y = arrY[2];
    Point four;
    four.x = arrX[3];
    four.y = arrY[3];
    addTarget(one, two, three, four);
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
//    
//    if (msg.type == update && msg.cacheType == 1){
//        myDir += msg.cacheDistance;
//        myDir %= 360;
//        inProg = false;
//    }else if( msg.type  == update && msg.cacheType == 0){
//         if(myDir == 0){
//             myLoc.x += msg.cacheDistance;
//         }else if(myDir == 90){
//             myLoc.y += msg.cacheDistance;
//         }else if(myDir == 180){
//             myLoc.x -= msg.cacheDistance;
//         }else if(myDir == 270){
//             myLoc.y -= msg.cacheDistance;
//         }
//         inProg = false;
//    }
    
    if(msg.type == debug){
        if(msg.targetDistance == 0){
            // increaseSP();
            addMotorTask(0, 3);
        } else if(msg.targetDistance == 1){
            addMotorTask(0, -3);
        }
    }
}

/*******************************************************************************
 End of File
 */
