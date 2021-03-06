/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    message_controller_thread.c

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

#include "message_controller_thread.h"
#include "motor_controller_thread_public.h"
#include <string.h>
#include <stddef.h>
#include "communication/messages.h"
#include "system_config.h"
#include "system_definitions.h"
#include "debug.h"
#include "json_wrapper.h"
#include "tx_thread_public.h"

static QueueHandle_t _queue;
static int systemClock;

#define TYPEOFQUEUE MessageObj
#define SIZEOFQUEUE 36

static float targetProximityVal = -1.0f;
static bool outsideArenaVal = false;
static bool movementStoppedVal = false;

/*******************************************************************************
  Function:
    void MESSAGE_CONTROLLER_THREAD_Initialize ( void )

  Remarks:
    See prototype in message_controller_thread.h.
 */

void MESSAGE_CONTROLLER_THREAD_Initialize ( void )
{
    MESSAGE_CONTROLLER_THREAD_InitializeQueue();
    DRV_TMR1_Start();
    resetSystemClock();
}


/******************************************************************************
  Function:
    void MESSAGE_CONTROLLER_THREAD_Tasks ( void )

  Remarks:
    See prototype in message_controller_thread.h.
 */

void MESSAGE_CONTROLLER_THREAD_Tasks ( void )
{
    dbgOutputLoc(ENTER_MESSAGE_CONTROLLER_THREAD);
    InternalData internalData;
    InternalData initialData;
    memset(&internalData, 0, sizeof(InternalData));
    memset(&initialData, 0, sizeof(InternalData));
    internalData.location.x = 0;
    internalData.location.y = 0;
    internalData.orientation = 0;
    initialData.location.x = 0;
    initialData.location.y = 0;
    initialData.orientation = 0;
    StatObjectType statObject;
    memset(&statObject, 0, sizeof(StatObjectType));
    type_t type = unknown;
    items_t items[12];
    int numItems;
    
    while(1) {
        initParser();
        MessageObj obj;
        memset(&obj, 0, sizeof(MessageObj));

        Tx_Thead_Queue_DataType tx_thread_obj;
        memset(&tx_thread_obj, 0, sizeof(Tx_Thead_Queue_DataType));

        dbgOutputLoc(BEFORE_READ_FROM_Q_MESSAGE_CONTROLLER_THREAD);
        MESSAGE_CONTROLLER_THREAD_ReadFromQueue(&obj);
        dbgOutputLoc(AFTER_READ_FROM_Q_MESSAGE_CONTROLLER_THREAD);
        switch(obj.Type) {
            case EXTERNAL_REQUEST_RESPONSE: {
                dbgOutputLoc(CASE_EXTERNAL_REQUEST_RESPONSE_MESSAGE_CONTROLLER_THREAD);
                if(obj.External.Error) {
                    statObject.ErrorCount++;
                    continue;
                }
                
                statObject.GoodCount++;
                
                parseJSON(obj.External.Data, &type, items,  &numItems);
                
                switch(type) {
                    case request: {
                        switch(obj.External.Source) {
                            case SEARCHERMOVER:
                                if ((obj.External.MessageCount - statObject.Req_From_SearcherMover) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_SearcherMover) + obj.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (obj.External.MessageCount - statObject.Req_From_SearcherMover);
                                }
                                statObject.Req_From_SearcherMover++;
                                break;
                            case TARGETLOCATOR:
                                if ((obj.External.MessageCount - statObject.Req_From_TargetLocator) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_TargetLocator) + obj.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (obj.External.MessageCount - statObject.Req_From_TargetLocator);
                                }
                                statObject.Req_From_TargetLocator++;
                                break;
                            case PATHFINDER:
                                if ((obj.External.MessageCount - statObject.Req_From_PathFinder) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_PathFinder) + obj.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (obj.External.MessageCount - statObject.Req_From_PathFinder);
                                }
                                statObject.Req_From_PathFinder++;
                                break;
                            case TARGETGRABBER:
                                if ((obj.External.MessageCount - statObject.Req_From_TargetGrabber) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_TargetGrabber) + obj.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (obj.External.MessageCount - statObject.Req_From_TargetGrabber);
                                }
                                statObject.Req_From_TargetGrabber++;
                                break;
                            case SERVER:
                                break;
                            default:
                                continue;
                        }

                        int i = 0;
                        tx_thread_obj.Destination = obj.External.Source;
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Response\"");
                        for(i = 0; i < numItems; i++) {
                            switch(items[i]) {
                                case CommStatsSearcherMover: case CommStatsTargetLocator: case CommStatsPathFinder: case CommStatsTargetGrabber: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), 
                                        ",\"CommStats%s\":{"
                                        "\"myName\":\"%s\","
                                        "\"numGoodMessagesRecved\":\"%d\","
                                        "\"numCommErrors\":\"%d\","
                                        "\"numMessagesDropped\":\"%d\","    
                                        "\"numJSONRequestsRecved\":\"%d\","
                                        "\"numJSONResponsesRecved\":\"%d\","
                                        "\"numJSONRequestsSent\":\"%d\","
                                        "\"numJSONResponsesSent\":\"%d\"}",
                                        MYMODULESTRING,
                                        MYMODULESTRING,
                                        statObject.GoodCount,
                                        statObject.ErrorCount,
                                        statObject.PacketsDropped,
                                        statObject.Req_From_PathFinder + statObject.Req_From_SearcherMover + statObject.Req_From_TargetGrabber + statObject.Req_From_TargetLocator,
                                        statObject.Res_From_PathFinder + statObject.Res_From_SearcherMover + statObject.Res_From_TargetGrabber + statObject.Res_From_TargetLocator,
                                        statObject.Req_To_PathFinder + statObject.Req_To_SearcherMover + statObject.Req_To_TargetGrabber + statObject.Req_To_TargetLocator,
                                        statObject.Res_To_PathFinder + statObject.Res_To_SearcherMover + statObject.Res_To_TargetGrabber + statObject.Res_To_TargetLocator
                                        );
                                    tx_thread_obj.Destination = SERVER;
                                    break;
                                }
                                case DetailedCommStatsSearcherMover: case DetailedCommStatsTargetLocator: case DetailedCommStatsPathFinder: case DetailedCommStatsTargetGrabber: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data),
                                        ",\"DetailedStats%s\":{"
                                        "\"numMessagesDropped\":\"%d\","
                                        "\"RequestTo\":{"
                                        "\"SM\":\"%d\","
                                        "\"TL\":\"%d\","
                                        "\"PF\":\"%d\","
                                        "\"TG\":\"%d\"},"
                                        "\"ResponseTo\":{"
                                        "\"SM\":\"%d\","
                                        "\"TL\":\"%d\","
                                        "\"PF\":\"%d\","
                                        "\"TG\":\"%d\"},"
                                        "\"RequestFrom\":{"
                                        "\"SM\":\"%d\","
                                        "\"TL\":\"%d\","
                                        "\"PF\":\"%d\","
                                        "\"TG\":\"%d\"},"
                                        "\"ResponseFrom\":{"
                                        "\"SM\":\"%d\","
                                        "\"TL\":\"%d\","
                                        "\"PF\":\"%d\","
                                        "\"TG\":\"%d\"}}",
                                        MYMODULESTRING,
                                        statObject.PacketsDropped,
                                        statObject.Req_To_SearcherMover,
                                        statObject.Req_To_TargetLocator,
                                        statObject.Req_To_PathFinder,
                                        statObject.Req_To_TargetGrabber,
                                        statObject.Res_To_SearcherMover,
                                        statObject.Res_To_TargetLocator,
                                        statObject.Res_To_PathFinder,
                                        statObject.Res_To_TargetGrabber,
                                        statObject.Req_From_SearcherMover,
                                        statObject.Req_From_TargetLocator,
                                        statObject.Req_From_PathFinder,
                                        statObject.Req_From_TargetGrabber,
                                        statObject.Res_From_SearcherMover,
                                        statObject.Res_From_TargetLocator,
                                        statObject.Res_From_PathFinder,
                                        statObject.Res_From_TargetGrabber
                                        );
                                    tx_thread_obj.Destination = SERVER;
                                    break;
                                }
                                case R1_Est_Location: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"R1_Est_Location\":{\"x\":\"%0.02f\",\"y\":\"%0.02f\"}", internalData.location.x, internalData.location.y);
                                    break;
                                }
                                case R1_Est_Orientation: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"R1_Est_Orientation\":\"%0.02f\"", internalData.orientation);
                                    break;
                                }
                                case Forward: {
                                    // MOTOR_CONTROLLER_THREAD_SendToQueue('F');
                                    break;
                                }
                                case Back: {
                                    // MOTOR_CONTROLLER_THREAD_SendToQueue('B');
                                    break;
                                }
                                case Left: {
                                    // MOTOR_CONTROLLER_THREAD_SendToQueue('L');
                                    break;
                                }
                                case Right: {
                                    // MOTOR_CONTROLLER_THREAD_SendToQueue('R');
                                    break;
                                }
                                case SensorData: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"SensorData\":\"%0.02f\"", internalData.sensordata);
                                    break;
                                }
                                case msLocalTime:{
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"msLocalTime\":\"%d\"", getSystemClock() * 200);
                                    tx_thread_obj.Destination = obj.External.Source;
                                    break;
                                }
                                case targetProximity:{
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"targetProximity\":\"%d\"", (int) targetProximityVal);
                                    tx_thread_obj.Destination = obj.External.Source;
                                    break;
                                }
                                case outsideArena:{
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"outsideArena\":\"%s\"", outsideArenaVal ? "true": "false");
                                    tx_thread_obj.Destination = obj.External.Source;
                                    break;
                                }
                                case movementStopped:{
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"movementStopped\":\"%s\"", movementStoppedVal ? "true": "false");
                                    tx_thread_obj.Destination = obj.External.Source;
                                    break;
                                }
                                
                                default:
                                    break;
                            }
                        }
                        sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data),"}"); // Print ending brace }
                        switch(obj.External.Source) {
                            case SEARCHERMOVER: {
                                tx_thread_obj.MessageCount = statObject.Res_To_SearcherMover;
                                statObject.Res_To_SearcherMover++;
                                break;
                            }
                            case TARGETLOCATOR: {
                                tx_thread_obj.MessageCount = statObject.Res_To_TargetLocator;
                                statObject.Res_To_TargetLocator++;
                                break;
                            }
                            case PATHFINDER: {
                                tx_thread_obj.MessageCount = statObject.Res_To_PathFinder;
                                statObject.Res_To_PathFinder++;
                                break;
                            }
                            case TARGETGRABBER: {
                                tx_thread_obj.MessageCount = statObject.Res_To_TargetGrabber;
                                statObject.Res_To_TargetGrabber++;
                                break;
                            }
                            default: {
                                break;
                            }
                        }
                        TX_THREAD_SendToQueue(tx_thread_obj);
                        break;
                    }
                    case response: {
                        switch(obj.External.Source) {
                            case SEARCHERMOVER: {
                                statObject.Res_From_SearcherMover++;
                                break;
                            }
                            case TARGETLOCATOR: {
                                statObject.Res_From_TargetLocator++;
                                break;
                            }
                            case PATHFINDER: {
                                statObject.Res_From_PathFinder++;
                                break;
                            }
                            case TARGETGRABBER: {
                                statObject.Res_From_TargetGrabber++;
                                break;
                            }
                            default: {
                                break;
                            }
                        }
                        bool g_align, t_acq;
                        bool obstacle = false;
                        float distance, angle;
                        float val;
                        
                        if(extractResponse_val(&val)){
                            message_in_t msg;
                            msg.type = debug;
                            msg.targetDistance = val;
                            MOTOR_CONTROLLER_THREAD_SendToQueue(msg);
                        } else if(extractResponse_targetAlignment(&g_align,&distance,&angle, &t_acq, &obstacle)){
                            message_in_t msg;
                            msg.type = targetAlignment;
                            msg.targetAligned = g_align;
                            msg.targetAcquired = t_acq;
                            msg.targetAngle = angle;
                            msg.targetDistance = distance;
                            msg.obstacle = obstacle;
                            MOTOR_CONTROLLER_THREAD_SendToQueue(msg);
                        }else{
                        
                            float xArr[10], yArr[10];
                            int len = extractResponse_Vertices(xArr, yArr);
                            message_in_t msg;
                            int j;
                            if(len > 0){
                                msg.type = vertex;
                                for(j = 0; j < len; j++){
                                    msg.x[j] = xArr[j];
                                    msg.y[j] = yArr[j];
                                }
                                msg.len = len;
                                MOTOR_CONTROLLER_THREAD_SendToQueue(msg);
                            }

                            float obsXArr[10][4], obsYArr[10][4];
                            len = extractResponse_Obstacles(obsXArr, obsYArr);
                            int i;
                            for(i = 0; i < len; i++){
                                msg.type = obstacle;
                                for(j = 0; j < 4; j++){
                                    msg.x[j] = obsXArr[i][j];
                                    msg.y[j] = obsYArr[i][j];
                                }
                                msg.len = 4;
                                MOTOR_CONTROLLER_THREAD_SendToQueue(msg);
                            }

                            float tarXArr[10][4], tarYArr[10][4];
                            len = extractResponse_Targets(tarXArr, tarYArr);
                            for(i = 0; i < len; i++){
                                msg.type = target;
                                for(j = 0; j < 1; j++){
                                    msg.x[j] = tarXArr[i][j];
                                    msg.y[j] = tarYArr[i][j];
                                }
                                msg.len = 1;
                                MOTOR_CONTROLLER_THREAD_SendToQueue(msg);
                            }
                        }
                        
                        break;
                    }
                    case unknown: {
                        break;
                    }
                    default: {
                        break;
                    }
                }
                break;
            }
            case SEND_REQUEST: {
                dbgOutputLoc(CASE_SEND_REQUEST_MESSAGE_CONTROLLER_THREAD);
                // We will only need to have a new case for something we are requesting from another PIC
                switch(obj.Request) {
                    case SMtoTL: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"Obstacles\",\"R1_Location\",\"R1_Orientation\"]}");
                        tx_thread_obj.Destination = TARGETLOCATOR;
                        tx_thread_obj.MessageCount = statObject.Req_To_TargetLocator;
                        statObject.Req_To_TargetLocator++;
                        break;
                    }
                    case TLtoSM: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"TLtoSM\"]}");
                        tx_thread_obj.Destination = SEARCHERMOVER;
                        tx_thread_obj.MessageCount = statObject.Req_To_SearcherMover;
                        statObject.Req_To_SearcherMover++;
                        break;
                    }
                    case TLtoPF: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"TLtoPF\"]}");
                        tx_thread_obj.Destination = PATHFINDER;
                        tx_thread_obj.MessageCount = statObject.Req_To_PathFinder;
                        statObject.Req_To_PathFinder++;
                        break;
                    }
                    case PFtoTL: {
                        // for now we are requesting vertices from the tl because i am lazy
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"Obstacles\", \"SafeRegions\", \"R2_Location\", \"Vertices\"]}");
                        tx_thread_obj.Destination = SERVER;
                        TX_THREAD_SendToQueue(tx_thread_obj);
                        
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"Obstacles\", \"Targets\", \"SafeRegions\", \"R2_Location\", \"Vertices\"]}");
                        tx_thread_obj.Destination = TARGETLOCATOR;
                        tx_thread_obj.MessageCount = statObject.Req_To_TargetLocator;
                        statObject.Req_To_TargetLocator++;
                        break;
                    }
                    case PFtoTG: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"targetAlignment\",\"targetAcquired\"]}");
                        tx_thread_obj.Destination = TARGETGRABBER;
                        tx_thread_obj.MessageCount = statObject.Req_To_TargetGrabber;
                        statObject.Req_To_TargetGrabber++;
                        break;
                    }
                    case TGtoPF: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"TGtoPF\"]}");
                        tx_thread_obj.Destination = PATHFINDER;
                        tx_thread_obj.MessageCount = statObject.Req_To_PathFinder;
                        statObject.Req_To_PathFinder++;
                        break;
                    }
                    case SD: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"SensorData\"]}");
                        tx_thread_obj.Destination = TARGETLOCATOR;
                        tx_thread_obj.MessageCount = statObject.Req_To_TargetLocator;
                        statObject.Req_To_TargetLocator++;
                        break;
                    }
                    default: {
                        continue;
                    }
                }
                TX_THREAD_SendToQueue(tx_thread_obj);
                break;
            }
            case UPDATE: {
                dbgOutputLoc(CASE_UPDATE_MESSAGE_CONTROLLER_THREAD);
                switch(obj.Update.Type) {
                    case POSITION: {
                        internalData.location.x = initialData.location.x + obj.Update.Data.location.x;
                        internalData.location.y = initialData.location.y + obj.Update.Data.location.y;
                        internalData.orientation = (initialData.orientation + obj.Update.Data.orientation);
                        break;
                    }
                    case SENSORDATA: {
                        internalData.sensordata = obj.Update.Data.sensordata;
                        break;
                    }
                    case TARG_PROX: {
                        targetProximityVal = obj.Update.Data.targetProx;
                        break;
                    }
                    case OUTS_ARENA: {
                        outsideArenaVal = obj.Update.Data.outsArena;
                        break;
                    }
                    case MOV_STOP: {
                        movementStoppedVal = obj.Update.Data.movStop;
                        break;
                    }
                    default: {
                        break;
                    }
                }
                break;
            }
            default: {
                break;
            }
    }
    dbgOutputLoc(LEAVE_MESSAGE_CONTROLLER_THREAD);
}
}

void MESSAGE_CONTROLLER_THREAD_InitializeQueue() {
    _queue = xQueueCreate(SIZEOFQUEUE, sizeof(TYPEOFQUEUE));
}

void MESSAGE_CONTROLLER_THREAD_ReadFromQueue(MessageObj* pvBuffer) {
    xQueueReceive(_queue, pvBuffer, portMAX_DELAY);
}

void MESSAGE_CONTROLLER_THREAD_SendToQueue(MessageObj buffer) {
    xQueueSend(_queue, &buffer, portMAX_DELAY);
}

void MESSAGE_CONTROLLER_THREAD_SendToQueueISR(MessageObj buffer, BaseType_t *pxHigherPriorityTaskWoken) {
    xQueueSendFromISR(_queue, &buffer, pxHigherPriorityTaskWoken);
}

void resetSystemClock(){
    systemClock = 0;
}

void incrementSystemClock(){
    systemClock++;
}

int getSystemClock(){
    return systemClock;
}

/*******************************************************************************
 End of File
 */
