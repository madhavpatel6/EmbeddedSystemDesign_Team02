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

#include "system_config.h"
#include "system_definitions.h"
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
    while(1) {
        MessageObj obj;
        memset(&obj, 0, sizeof(MessageObj));

        JSONObjType parsedJSONInfo;
        memset(&parsedJSONInfo, 0, sizeof(JSONObjType));
        parsedJSONInfo.type = unknown;
        
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
                parseJSON(obj.External.Data, &parsedJSONInfo.type, parsedJSONInfo.items,  &parsedJSONInfo.numItems);
                ComputeAndUpdatePacketsDropped(parsedJSONInfo.type, obj.External.Source, obj.External.MessageCount, &statObject);
                switch(parsedJSONInfo.type) {
                    case request: {
                        ConstructResponse(tx_thread_obj.Data, internalData, statObject, parsedJSONInfo);
                        tx_thread_obj.Destination = obj.External.Source;
                        GetResponseMessageCountAndIncrement(&tx_thread_obj.MessageCount, obj.External.Source, &statObject);
                        TX_THREAD_SendToQueue(tx_thread_obj);
                        break;
                    }
                    case response: {
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
                if(CreateRequestObject(&tx_thread_obj, obj.Request, &statObject)) {
                    TX_THREAD_SendToQueue(tx_thread_obj);
                }
                break;
            }
            case UPDATE: {
                dbgOutputLoc(CASE_UPDATE_MESSAGE_CONTROLLER_THREAD);
                UpdateInternalData(obj.Update, &internalData);
                break;
            }
    }
    dbgOutputLoc(LEAVE_MESSAGE_CONTROLLER_THREAD);
}
}

bool CreateRequestObject(Tx_Thead_Queue_DataType* requestObject, InternalRequestType typeOfRequest, StatObjectType* statObject) {
    switch(typeOfRequest) {
        case RV1_REQUEST_LOCATION_ORIENTATION: {
            sprintf(requestObject->Data, "{\"type\":\"Request\",\"items\":[\"RV1_Location\",\"RV1_Orientation\"]}");
            requestObject->Destination = SEARCHERMOVER;
            requestObject->MessageCount = statObject->Req_To_SearcherMover;
            statObject->Req_To_SearcherMover++;
            return true;
            break;
        }
        case RV1_REQUEST_SENSOR_DATA: {
            sprintf(requestObject->Data, "{\"type\":\"Request\",\"items\":[\"SensorData\"]}");
            requestObject->Destination = TARGETLOCATOR;
            requestObject->MessageCount = statObject->Req_To_TargetLocator;
            statObject->Req_To_TargetLocator++;
            return true;
            break;
        }
        default: {
            return false;
            break;
        }
    }
    return false;
}

void UpdateInternalData(UpdateObj update, InternalData *internalData) {
    switch(update.Type) {
        case LOCATION:{
            internalData->location = update.Data.location;
            break;
        }
        case ORIENTATION: {
            internalData->orientation = update.Data.orientation;
            break;
        }
        case SENSORDATA: {
            internalData->sensordata = update.Data.sensordata;
            break;
        }
    }
}

void ConstructResponse(char response[], InternalData internalData, StatObjectType statObject, JSONObjType parsedJSONInfo) {
    int i = 0;
    sprintf(response, "{\"type\":\"Response\",");
    for(i = 0; i < parsedJSONInfo.numItems; i++) {
        switch(parsedJSONInfo.items[i]) {
            case commStats: {
                sprintf(response + strlen(response), "\"myName\": \"%s\",\"numGoodMessagesRecved\": \"%d\", \"numCommErrors\": \"%d\",\"numJSONRequestsRecved\": \"%d\",\"numJSONResponsesRecved\": \"%d\",\"numJSONRequestsSent\": \"%d\",\"numJSONResponsesSent\": \"%d",
                "MYFUCKINGPMODULE",
                statObject.GoodCount,
                statObject.ErrorCount,
                statObject.Req_From_PathFinder + statObject.Req_From_SearcherMover + statObject.Req_From_TargetGrabber + statObject.Req_From_TargetLocator,
                statObject.Res_From_PathFinder + statObject.Res_From_SearcherMover + statObject.Res_From_TargetGrabber + statObject.Res_From_TargetLocator,
                statObject.Req_To_PathFinder + statObject.Req_To_SearcherMover + statObject.Req_To_TargetGrabber + statObject.Req_To_TargetLocator,
                statObject.Res_To_PathFinder + statObject.Res_To_SearcherMover + statObject.Res_To_TargetGrabber + statObject.Res_To_TargetLocator
                );
                break;
            }
            case sensorData: {
                sprintf(response + strlen(response), "\"SensorData\": \"%0.02f", internalData.sensordata);
                break;
            }
        }
    }
    sprintf(response + strlen(response),"}");
}

void GetResponseMessageCountAndIncrement(char* messageCount, char source, StatObjectType* statObject) {
    switch(source) {
        case TARGETLOCATOR: {
            *messageCount = statObject->Res_To_TargetLocator;
            statObject->Res_To_TargetLocator++;
            break;
        }
        case PATHFINDER: {
            *messageCount = statObject->Res_To_PathFinder;
            statObject->Res_To_PathFinder++;
            break;
        }
        case SEARCHERMOVER: {
            *messageCount = statObject->Res_To_SearcherMover;
            statObject->Res_To_SearcherMover++;
            break;
        }
        case TARGETGRABBER: {
            *messageCount = statObject->Res_To_TargetGrabber;
            statObject->Res_To_TargetGrabber++;
            break;
        }
        default: {
            break;
        }
    }
}

void ComputeAndUpdatePacketsDropped(type_t type, char source, char messageCount, StatObjectType* statObject) {
    switch(type) {
        case request: {
            switch(source) {
            case SEARCHERMOVER:
                statObject->PacketsDropped += (messageCount - statObject->Req_From_SearcherMover);
                statObject->Req_From_SearcherMover++;
                break;
            case TARGETLOCATOR:
                statObject->PacketsDropped += (messageCount - statObject->Req_From_TargetLocator);
                statObject->Req_From_TargetLocator++;
                break;
            case PATHFINDER:
                statObject->PacketsDropped += (messageCount - statObject->Req_From_PathFinder);
                statObject->Req_From_PathFinder++;
                break;
            case TARGETGRABBER:
                statObject->PacketsDropped += (messageCount - statObject->Req_From_TargetGrabber);
                statObject->Req_From_TargetGrabber++;
                break;
            default:
                break;
            }
            break;
        }
        case response: {
            switch(source) {
            case SEARCHERMOVER:
                statObject->PacketsDropped += (messageCount - statObject->Res_From_SearcherMover);
                statObject->Res_From_SearcherMover++;
                break;
            case TARGETLOCATOR:
                statObject->PacketsDropped += (messageCount - statObject->Res_From_TargetLocator);
                statObject->Res_From_TargetLocator++;
                break;
            case PATHFINDER:
                statObject->PacketsDropped += (messageCount - statObject->Res_From_PathFinder);
                statObject->Res_From_PathFinder++;
                break;
            case TARGETGRABBER:
                statObject->PacketsDropped += (messageCount - statObject->Res_From_TargetGrabber);
                statObject->Res_From_TargetGrabber++;
                break;
            default:
                break;
            }
            break;
        }
        default: {
            break;
        }
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
