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
    InternalData _internalData;
    memset(&_internalData, 0, sizeof(InternalData));
    StatObjectType statObject;
    memset(&statObject, 0, sizeof(StatObjectType));
    type_t type = unknown;
    items_t items[12];
    int numItems;
    while(1) {
        MessageObj obj;
        memset(&obj, 0, sizeof(MessageObj));
        char jsonmessage[MAXMESSAGESIZE];
        memset(jsonmessage, 0, MAXMESSAGESIZE);
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
                        statObject.PacketsDropped += (obj.External.MessageCount - statObject.Req_SearcherMover);
                        statObject.Req_SearcherMover++;
                        break;
                    case TARGETLOCATOR:
                        statObject.PacketsDropped += (obj.External.MessageCount - statObject.Req_TargetLocator);
                        statObject.Req_TargetLocator++;
                        break;
                    case PATHFINDER:
                        statObject.PacketsDropped += (obj.External.MessageCount - statObject.Req_PathFinder);
                        statObject.Req_PathFinder++;
                        break;
                    case TARGETGRABBER:
                        statObject.PacketsDropped += (obj.External.MessageCount - statObject.Req_TargetGrabber);
                        statObject.Req_TargetGrabber++;
                        break;
                }
                parseJSON(obj.External.Data, &type, items,  &numItems);
                if(type == request){
                    int i;
                    for(i = 0; i < numItems; i++){
                        items[i];
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
                switch(obj.Request) {
                    case REQUEST_LOCATION: {

                        break;
                    }
                    case REQUEST_ARE_WE_THERE_YET: {
                        break;
                    }
                    case REQUEST_DO_YOU_HAVE_IT: {
                        break;
                    }
                }
            }
            case UPDATE: {
                dbgOutputLoc(CASE_UPDATE_MESSAGE_CONTROLLER_THREAD);
                switch(obj.Update.Type) {
                    case LOCATION:{
                        _internalData.location = obj.Update.Data.location;
                        break;
                    }
                    case ORIENTATION: {
                        _internalData.orientation = obj.Update.Data.orientation;
                        break;
                    }
                    case SENSORDATA: {
                        _internalData.sensordata = obj.Update.Data.sensordata;
                        // This is for debugging purposes
                        Tx_Thead_Queue_DataType obj;
                        memset(&obj, 0, sizeof(Tx_Thead_Queue_DataType));
                        sprintf(obj.Data, "%0.2f", _internalData.sensordata);
                        obj.Destination = PATHFINDER;
                        obj.MessageCount = statObject.Res_PathFinder;
                        TX_THREAD_SendToQueue(obj);
                        break;
                    }
                }
                break;
            }
        }
    }
    dbgOutputLoc(LEAVE_MESSAGE_CONTROLLER_THREAD);
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
