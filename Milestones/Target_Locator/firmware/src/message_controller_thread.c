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
#define SIZEOFQUEUE 24

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
    items_t items[12];
    int numItems;
    TL_Queue_t sendTL;
    bool stopRequesting = false;
    while(1) {
        initParser();
        MessageObj obj;
        memset(&obj, 0, sizeof(MessageObj));
        memset(&sendTL, 0, sizeof(TL_Queue_t));
        Tx_Thead_Queue_DataType tx_thread_obj;
        memset(&tx_thread_obj, 0, sizeof(Tx_Thead_Queue_DataType));
        Movement_t r1_movement;
        dbgOutputLoc(BEFORE_READ_FROM_Q_MESSAGE_CONTROLLER_THREAD);
        MESSAGE_CONTROLLER_THREAD_ReadFromQueue(&obj);
        dbgOutputLoc(AFTER_READ_FROM_Q_MESSAGE_CONTROLLER_THREAD);
        int row = 0;
        switch(obj.type) {
            case EXTERNAL_REQUEST_RESPONSE: {
                dbgOutputLoc(CASE_EXTERNAL_REQUEST_RESPONSE_MESSAGE_CONTROLLER_THREAD);
                if(obj.message.External.Error) {
                    statObject.ErrorCount++;
                    continue;
                }

                statObject.GoodCount++;

                parseJSON(obj.message.External.Data, &type, items,  &numItems, &r1_movement, &row);

                switch(type) {
                    case request: {
                        switch(obj.message.External.Source) {
                            case SEARCHERMOVER:
                                if ((obj.message.External.MessageCount - statObject.Req_From_SearcherMover) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_SearcherMover) + obj.message.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (obj.message.External.MessageCount - statObject.Req_From_SearcherMover);
                                }
                                statObject.Req_From_SearcherMover++;
                                break;
                            case TARGETLOCATOR:
                                if ((obj.message.External.MessageCount - statObject.Req_From_TargetLocator) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_TargetLocator) + obj.message.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (obj.message.External.MessageCount - statObject.Req_From_TargetLocator);
                                }
                                statObject.Req_From_TargetLocator++;
                                break;
                            case PATHFINDER:
                                if ((obj.message.External.MessageCount - statObject.Req_From_PathFinder) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_PathFinder) + obj.message.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (obj.message.External.MessageCount - statObject.Req_From_PathFinder);
                                }
                                statObject.Req_From_PathFinder++;
                                break;
                            case TARGETGRABBER:
                                if ((obj.message.External.MessageCount - statObject.Req_From_TargetGrabber) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_TargetGrabber) + obj.message.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (obj.message.External.MessageCount - statObject.Req_From_TargetGrabber);
                                }
                                statObject.Req_From_TargetGrabber++;
                                break;
                            case SERVER:
                                break;
                            default:
                                continue;
                        }

                        int i = 0;
                        tx_thread_obj.Destination = obj.message.External.Source;
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
                                case TargetLocatorSensorData: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"TargetLocatorSensorData\": {"
                                            "\"IR\":{"
                                            "\"leftFT\":\"%.2f\","
                                            "\"rightFT\":\"%.2f\","
                                            "\"leftFB\":\"%.2f\","
                                            "\"middleFB\":\"%.2f\","
                                            "\"rightFB\":\"%.2f\","
                                            "\"farLeftFB\":\"%.2f\","
                                            "\"farRightFB\":\"%.2f\"},"
                                            "\"leftFT\":[\"%.1f\",\"%.1f\",\"%.1f\"],"
                                            "\"middleFT\":[\"%.1f\",\"%.1f\",\"%.1f\"],"
                                            "\"rightFT\":[\"%.1f\",\"%.1f\",\"%.1f\"]"
                                            "}",
                                            internalData.sensordata.ir.leftFTSensor,
                                            internalData.sensordata.ir.rightFTSensor,
                                            internalData.sensordata.ir.leftFBSensor,
                                            internalData.sensordata.ir.middleFBSensor,
                                            internalData.sensordata.ir.rightFBSensor,
                                            internalData.sensordata.ir.farLeftFBSensor,
                                            internalData.sensordata.ir.farRightFBSensor,
                                            internalData.sensorInformation.leftFrontSensor.sensorLocation.x,
                                            internalData.sensorInformation.leftFrontSensor.sensorLocation.y,
                                            internalData.sensorInformation.leftFrontSensor.orientation,
                                            internalData.sensorInformation.middleFrontSensor.sensorLocation.x,
                                            internalData.sensorInformation.middleFrontSensor.sensorLocation.y,
                                            internalData.sensorInformation.middleFrontSensor.orientation,
                                            internalData.sensorInformation.rightFrontSensor.sensorLocation.x,
                                            internalData.sensorInformation.rightFrontSensor.sensorLocation.y,
                                            internalData.sensorInformation.rightFrontSensor.orientation
                                            );
                                    tx_thread_obj.Destination = SERVER;
                                    break;
                                }
                                case msLocalTime:{
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"msLocalTime\":\"%d\"", getSystemClock() * 200);
                                    tx_thread_obj.Destination = obj.message.External.Source;
                                    break;
                                }
                                case OccupancyGrid: {
                                    sendTL.type = REQUESTOCCUPANYGRID;
                                    sendTL.contents.row = row;
                                    SENSOR_THREAD_SendToQueue(sendTL);
                                    break;
                                }
                                case LocationInformation: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data),",\"LocationInformation\":[\"%.1f\",\"%.1f\",\"%.1f\"]",
                                            internalData.sensorInformation.roverLocation.x,
                                            internalData.sensorInformation.roverLocation.y,
                                            internalData.sensorInformation.orientation
                                            );
                                    tx_thread_obj.Destination = obj.message.External.Source;
                                    break;
                                }
                                case SensorData: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data),",\"SensorData\":\"%c\"",
                                            (internalData.proximity.leftProximity || internalData.proximity.middleProximity || internalData.proximity.rightProximity) ? '1' : '0'
                                            );
                                    tx_thread_obj.Destination = obj.message.External.Source;
                                    break;
                                }
                                case InterpretGrid: {
                                    sendTL.type = INTERPRETGRIDREQUEST;
                                    SENSOR_THREAD_SendToQueue(sendTL);
                                }
                                default:
                                    break;
                            }
                        }
                        sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data),"}");
                        switch(obj.message.External.Source) {
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
                        if(items[0] != OccupancyGrid && items[0] != InterpretGrid) {
                            TX_THREAD_SendToQueue(tx_thread_obj);
                        }
                        break;
                    }
                    case response: {
                        switch(obj.message.External.Source) {
                            case SEARCHERMOVER: {
                                statObject.Res_From_SearcherMover++;
                                sendTL.type = RV1_POSUPDATE;
                                sendTL.contents.r1_movement = r1_movement;
                                SENSOR_THREAD_SendToQueue(sendTL);
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
                            case SERVER: {
//                                sendTL.type = RV1_POSUPDATE;
//                                sendTL.contents.r1_movement = r1_movement;
//                                SENSOR_THREAD_SendToQueue(sendTL);
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
                switch(obj.message.Request) {
                    case SMtoTL: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"Obstacles\",\"RV1_Location\",\"RV1_Orientation\"]}");
                        tx_thread_obj.Destination = TARGETLOCATOR;
                        tx_thread_obj.MessageCount = statObject.Req_To_TargetLocator;
                        statObject.Req_To_TargetLocator++;
                        break;
                    }
                    case TLtoSM: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"R1_Movement\"]}");
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
                switch(obj.message.Update.Type) {
                    case SENSORDATA: {
                        internalData.sensordata = obj.message.Update.Data.sensordata;
                        internalData.sensorInformation = obj.message.Update.Data.sensorInformation;
                        internalData.proximity = obj.message.Update.Data.proximity;
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
