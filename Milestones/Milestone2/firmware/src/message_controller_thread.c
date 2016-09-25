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
#include "message_controller_thread_public.h"
#include "tx_thread_public.h"

static QueueHandle_t _queue;

#define TYPEOFQUEUE MessageObj
#define SIZEOFQUEUE 20

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
    initParser();
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
    while(1) {
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
                switch(obj.External.Source) {
                    case SEARCHERMOVER:
                        statObject.PacketsDropped += (obj.External.MessageCount - statObject.Req_From_SearcherMover);
                        statObject.Req_From_SearcherMover++;
                        break;
                    case TARGETLOCATOR:
                        statObject.PacketsDropped += (obj.External.MessageCount - statObject.Req_From_TargetLocator);
                        statObject.Req_From_TargetLocator++;
                        break;
                    case PATHFINDER:
                        statObject.PacketsDropped += (obj.External.MessageCount - statObject.Req_From_PathFinder);
                        statObject.Req_From_PathFinder++;
                        break;
                    case TARGETGRABBER:
                        statObject.PacketsDropped += (obj.External.MessageCount - statObject.Req_From_TargetGrabber);
                        statObject.Req_From_TargetGrabber++;
                        break;
                }
                parseJSON(obj.External.Data, &type, items,  &numItems);
                if(type == request){
                    int i = 0;
                    for(i = 0; i < numItems; i++) {
                        switch(items[i]) {
                            case commStats: {
                                sprintf(tx_thread_obj.Data, "{\"type\":\"Response\",\"myName\": \"%s\",\"numGoodMessagesRecved\": \"%d\", \"numCommErrors\": \"%d\",\"numJSONRequestsRecved\": \"%d\",\"numJSONResponsesRecved\": \"%d\",\"numJSONRequestsSent\": \"%d\",\"numJSONResponsesSent\": \"%d\"}",
                                "MYFUCKINGPMODULE",
                                statObject.GoodCount,
                                statObject.ErrorCount,
                                statObject.Req_From_PathFinder + statObject.Req_From_SearcherMover + statObject.Req_From_TargetGrabber + statObject.Req_From_TargetLocator,
                                statObject.Res_From_PathFinder + statObject.Res_From_SearcherMover + statObject.Res_From_TargetGrabber + statObject.Res_From_TargetLocator,
                                statObject.Req_To_PathFinder + statObject.Req_To_SearcherMover + statObject.Req_To_TargetGrabber + statObject.Req_To_TargetLocator,
                                statObject.Res_To_PathFinder + statObject.Res_To_SearcherMover + statObject.Res_To_TargetGrabber + statObject.Res_To_TargetLocator
                                );
                                tx_thread_obj.Destination = SERVER;
                                tx_thread_obj.MessageCount = 0;
                                TX_THREAD_SendToQueue(tx_thread_obj);
                                break;
                            }
                            case sensorData: {
                                sprintf(tx_thread_obj.Data, "{\"type\":\"Response\",\"SensorData\": \"%0.02f\"}", internalData.sensordata);
                                tx_thread_obj.Destination = obj.External.Source;
                                switch(obj.External.Source) {
                                    case TARGETLOCATOR: {
                                        tx_thread_obj.MessageCount = statObject.Req_To_TargetLocator;
                                        statObject.Req_To_TargetLocator++;
                                    }
                                    case PATHFINDER: {
                                        tx_thread_obj.MessageCount = statObject.Req_To_PathFinder;
                                        statObject.Req_To_PathFinder++;
                                    }
                                    case SEARCHERMOVER: {
                                        tx_thread_obj.MessageCount = statObject.Req_To_SearcherMover;
                                        statObject.Req_To_SearcherMover++;
                                    }
                                    case TARGETGRABBER: {
                                        tx_thread_obj.MessageCount = statObject.Req_To_TargetGrabber;
                                        statObject.Req_To_TargetGrabber++;
                                    }
                                }
                                TX_THREAD_SendToQueue(tx_thread_obj);
                                break;
                            }
                        }
                    }
                }

                //Parse JSON request or response
                //switch on response of request
                //if it is a request
                    //Create JSON response from local crap
                    //Send to TX
                //if it is a response
                    //convert the string to the proper
                break;
            }
            case SEND_REQUEST: {
                dbgOutputLoc(CASE_SEND_REQUEST_MESSAGE_CONTROLLER_THREAD);
                // We will only need to have a new case for something we are requesting from another PIC
                switch(obj.Request) {
                    case RV1_REQUEST_LOCATION_ORIENTATION: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"RV1_Location\",\"RV1_Orientation\"]}");
                        tx_thread_obj.Destination = SEARCHERMOVER;
                        tx_thread_obj.MessageCount = statObject.Req_To_SearcherMover;
                        statObject.Req_To_SearcherMover++;
                        TX_THREAD_SendToQueue(tx_thread_obj);
                        break;
                    }
                    case RV1_REQUEST_SENSOR_DATA: {
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"SensorData\"]}");
                        tx_thread_obj.Destination = TARGETLOCATOR;
                        tx_thread_obj.MessageCount = statObject.Req_To_TargetLocator;
                        statObject.Req_To_TargetLocator++;
                        TX_THREAD_SendToQueue(tx_thread_obj);
                        break;
                    }
                    default: {
                        continue;
                    }
                }

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
                        // This is for debugging purposes
//                        Tx_Thead_Queue_DataType obj;
//                        memset(&obj, 0, sizeof(Tx_Thead_Queue_DataType));
//                        sprintf(obj.Data, "%0.2f", _internalData.sensordata);
//                        obj.Destination = PATHFINDER;
//                        obj.MessageCount = statObject.Res_PathFinder;
//                        TX_THREAD_SendToQueue(obj);
                        break;
                    }
                }
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

/*******************************************************************************
 End of File
 */
