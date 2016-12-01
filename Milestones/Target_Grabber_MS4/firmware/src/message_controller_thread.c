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
    memset(&internalData, 0, sizeof(InternalData));
    StatObjectType statObject;
    memset(&statObject, 0, sizeof(StatObjectType));
    type_t type = unknown;
    pathFinderResponse_t pathFinderResponseMC;
    items_t items[12];
    int pathFinderResponseValueProximity;
    bool pathFinderResponseValueOutsideArena;
    bool pathFinderResponseValueMovementStopped;
    int numItems;
    while(1) {
        initParser();
        MessageObj obj;
        memset(&obj, 0, sizeof(MessageObj));
        
        TargetAlignment_Type alignmentPFResponse;
        memset(&alignmentPFResponse, 0, (size_t) sizeof(TargetAlignment_Type));

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
                
                parseJSON(obj.External.Data, &type, items,  &numItems, &pathFinderResponseMC, &pathFinderResponseValueProximity, &pathFinderResponseValueOutsideArena, &pathFinderResponseValueMovementStopped);
                
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
                                case targetAlignment: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"targetAlignment\":{"
                                                                                           "\"Target_Distance\":\"%0.2f\","
                                                                                           "\"Target_Angle\":\"%0.2f\","
                                                                                           "\"Grabber_Aligned\":\"%d\","
                                                                                           "\"Alignment_Info\":\"%s\"}",
                                                                                           internalData.alignmentData.Target_Distance,
                                                                                           internalData.alignmentData.Target_Angle,
                                                                                           internalData.alignmentData.Grabber_Aligned,
                                                                                           getAlignment(internalData.alignmentData.internalAlignment)
                                                                                           );
//                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"targetAlignment\":{"
//                                                                                           "\"IR_0\":\"%0.2f\","
//                                                                                           "\"IR_1\":\"%0.2f\","
//                                                                                           "\"IR_2\":\"%0.2f\","
//                                                                                           "\"IR_3\":\"%0.2f\","
//                                                                                           "\"IR_4\":\"%0.2f\","
//                                                                                           "\"Target_Distance\":\"%0.2f\","
//                                                                                           "\"Target_Angle\":\"%0.2f\","
//                                                                                           "\"IR_0_bool\":\"%d\","
//                                                                                           "\"IR_1_bool\":\"%d\","
//                                                                                           "\"IR_2_bool\":\"%d\","
//                                                                                           "\"IR_3_bool\":\"%d\","
//                                                                                           "\"IR_4_bool\":\"%d\","
//                                                                                           "\"Grabber_Aligned\":\"%d\","
//                                                                                           "\"Alignment_Info\":\"%s\"}",
//                                                                                           internalData.alignmentData.IR_0,
//                                                                                           internalData.alignmentData.IR_1,
//                                                                                           internalData.alignmentData.IR_2,
//                                                                                           internalData.alignmentData.IR_3,
//                                                                                           internalData.alignmentData.IR_4,
//                                                                                           internalData.alignmentData.Target_Distance,
//                                                                                           internalData.alignmentData.Target_Angle,
//                                                                                           internalData.alignmentData.IR_0_bool,
//                                                                                           internalData.alignmentData.IR_1_bool,
//                                                                                           internalData.alignmentData.IR_2_bool,
//                                                                                           internalData.alignmentData.IR_3_bool,
//                                                                                           internalData.alignmentData.IR_4_bool,
//                                                                                           internalData.alignmentData.Grabber_Aligned,
//                                                                                           getAlignment(internalData.alignmentData.internalAlignment)
//                                                                                           );
                                    //tx_thread_obj.Destination = obj.External.Source; // looks like it is set at top
                                    break;
                                }
                                case targetAcquired: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"targetAcquired\":\"%d\"", internalData.targetAcquiredBool);
                                    //tx_thread_obj.Destination = obj.External.Source; // looks like it is set at top
                                    break;
                                }
                                case SensorData: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"SensorData\":\"%0.02f\"", internalData.sensordata);
                                    //tx_thread_obj.Destination = obj.External.Source; // looks like it is set at top
                                    break;
                                }
                                case msLocalTime:{
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"msLocalTime\":\"%d\"", getSystemClock() * 200);
                                    //tx_thread_obj.Destination = obj.External.Source; // looks like it is set at top
                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                        sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data),"}");
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
                                alignmentPFResponse.targetProximityInt = pathFinderResponseValueProximity;
                                alignmentPFResponse.outsideArenaBool = pathFinderResponseValueOutsideArena;
                                alignmentPFResponse.movementStoppedBool = pathFinderResponseValueMovementStopped;
                                computation_thread_SendValToMsgQ(alignmentPFResponse);
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
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"Obstacles\",\"RV1_Location\",\"RV1_Orientation\"]}");
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
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"PFtoTL\"]}");
                        tx_thread_obj.Destination = TARGETLOCATOR;
                        tx_thread_obj.MessageCount = statObject.Req_To_TargetLocator;
                        statObject.Req_To_TargetLocator++;
                        break;
                    }
                    case PFtoTG: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"PFtoTG\"]}");
                        tx_thread_obj.Destination = TARGETGRABBER;
                        tx_thread_obj.MessageCount = statObject.Req_To_TargetGrabber;
                        statObject.Req_To_TargetGrabber++;
                        break;
                    }
                    case TGtoPF: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"outsideArena\",\"targetProximity\",\"movementStopped\"]}");
                        tx_thread_obj.Destination = PATHFINDER;
                        tx_thread_obj.MessageCount = statObject.Req_To_PathFinder;
                        statObject.Req_To_PathFinder++;
                        break;
                    }
                    case SD: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"SensorData\"]}");
                        tx_thread_obj.Destination = obj.External.Source;
                        //tx_thread_obj.MessageCount = statObject.Req_To_TargetLocator;
                        //statObject.Req_To_TargetLocator++;
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
                    case LOCATION:{
                        internalData.location = obj.Update.Data.location;
                        break;
                    }
                    case ORIENTATION: {
                        internalData.orientation = obj.Update.Data.orientation;
                        break;
                    }
                    case SENSORDATA: {
                        internalData.sensordata = obj.Update.Data.sensordata;
                        break;
                    }
                    case ALIGNMENTDATA: {
                        internalData.alignmentData = obj.Update.Data.alignmentData;
                        break;
                    }
                    case TARGETACQUIRED:{
                        internalData.targetAcquiredBool = obj.Update.Data.targetAcquiredBool;
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

// This returns a string to build the correct target alignment json string
const char* getAlignment(INTERNAL_ALIGNMENT_RESULTS InternalAlignmentResults){
    const char* alignmentStr = "alignmentStr";
    switch(InternalAlignmentResults){
        case NOT_ALIGNED_INTERNAL:{
            alignmentStr = "Not_Aligned";
            return alignmentStr;
            break;
        }
        case SLIGHT_RIGHT_INTERNAL:{
            alignmentStr = "Slight_Right";
            return alignmentStr;
            break;
        }
        case SLIGHT_LEFT_INTERNAL:{
            alignmentStr = "Slight_Left";
            return alignmentStr;
            break;
        }
        case MAJOR_RIGHT_INTERNAL:{
            alignmentStr = "Major_Right";
            return alignmentStr;
            break;
        }
        case MAJOR_LEFT_INTERNAL:{
            alignmentStr = "Major_Left";
            return alignmentStr;
            break;
        }
        case ALIGNED_NOT_CLOSE_INTERNAL:{
            alignmentStr = "Aligned_Not_Close";
            return alignmentStr;
            break;
        }
        case ALIGNED_INTERNAL:{
            alignmentStr = "Aligned";
            return alignmentStr;
            break;
        }
        case OBSTACLE_INTERNAL:{
            alignmentStr = "Obstacle";
            return alignmentStr;
            break;
        }
        default:{
            alignmentStr = "alignmentStr";
            return alignmentStr;
            break;
        }
    }
                
                                
}

/*******************************************************************************
 End of File
 */
