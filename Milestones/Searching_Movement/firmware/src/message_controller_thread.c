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
#include "adc_thread_public.h"
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
#define SIZEOFQUEUE 150

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
    internalData.location.x = 0;
    internalData.location.y = 0;
    internalData.orientation = 0;
    StatObjectType statObject;
    memset(&statObject, 0, sizeof(StatObjectType));
    type_t type = unknown;
    items_t items[12];
    int numItems = 0;
    int value = 0;
    uint8_t data;
    bool initialized = false;
    bool stopSending = false;
    
    while(1) {
        initParser();

        MessageObj messageObj;
        MotorObj motorObj;
        Tx_Thead_Queue_DataType tx_thread_obj;
        LineProperties lineProperties;
        
        memset(&messageObj, 0, sizeof(MessageObj));
        memset(&motorObj, 0, sizeof(MotorObj));
        memset(&tx_thread_obj, 0, sizeof(Tx_Thead_Queue_DataType));
        memset(&lineProperties, 0, sizeof(LineProperties));
        
        dbgOutputLoc(BEFORE_READ_FROM_Q_MESSAGE_CONTROLLER_THREAD);
        MESSAGE_CONTROLLER_THREAD_ReadFromQueue(&messageObj);
        dbgOutputLoc(AFTER_READ_FROM_Q_MESSAGE_CONTROLLER_THREAD);
        
        // Check the message type
        switch(messageObj.Type) {
            // External message
            case EXTERNAL_REQUEST_RESPONSE: {
                dbgOutputLoc(CASE_EXTERNAL_REQUEST_RESPONSE_MESSAGE_CONTROLLER_THREAD);
                if(messageObj.External.Error) {
                    statObject.ErrorCount++;
                    continue;
                }
                
                statObject.GoodCount++;
                
                // Parse message and extract data
                parseJSON(messageObj.External.Data, &type, items,  &numItems, &value, &data, &motorObj, &lineProperties);
                
                switch(type) {
                    case request: {
                        // Only handle message if rover has been initialized
                        if (!initialized) {
                            continue;
                        }
                        // Increment message counter from source
                        switch(messageObj.External.Source) {
                            case SEARCHERMOVER:
                                if ((messageObj.External.MessageCount - statObject.Req_From_SearcherMover) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_SearcherMover) + messageObj.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (messageObj.External.MessageCount - statObject.Req_From_SearcherMover);
                                }
                                statObject.Req_From_SearcherMover++;
                                break;
                            case TARGETLOCATOR:
                                if ((messageObj.External.MessageCount - statObject.Req_From_TargetLocator) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_TargetLocator) + messageObj.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (messageObj.External.MessageCount - statObject.Req_From_TargetLocator);
                                }
                                statObject.Req_From_TargetLocator++;
                                break;
                            case PATHFINDER:
                                if ((messageObj.External.MessageCount - statObject.Req_From_PathFinder) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_PathFinder) + messageObj.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (messageObj.External.MessageCount - statObject.Req_From_PathFinder);
                                }
                                statObject.Req_From_PathFinder++;
                                break;
                            case TARGETGRABBER:
                                if ((messageObj.External.MessageCount - statObject.Req_From_TargetGrabber) < 0) {
                                    statObject.PacketsDropped = (256 - statObject.Req_From_TargetGrabber) + messageObj.External.MessageCount;
                                } else {
                                    statObject.PacketsDropped = (messageObj.External.MessageCount - statObject.Req_From_TargetGrabber);
                                }
                                statObject.Req_From_TargetGrabber++;
                                break;
                            case SERVER:
                                break;
                            default:
                                continue;
                        }

                        int i = 0;
                        tx_thread_obj.Destination = messageObj.External.Source;
                        sprintf(tx_thread_obj.Data, "{\"type\":\"Response\"");
                        // Iterate through all requested items
                        for(i = 0; i < numItems; i++) {
                            switch(items[i]) {
                                // CommStats
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
                                //Detailed CommStats
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
                                // Handle request for rover movement
                                case R1_Movement: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), 
                                        ",\"R1_Movement\":[\"%0.02f\",\"%0.02f\",\"%0.02f\",\"%i\",\"%0.02f\"]",
                                        internalData.location.x, internalData.location.y, internalData.orientation,
                                        internalData.movement.action, internalData.movement.amount
                                        );
                                    break;
                                }
                                // Send forward command to motor_controller thread
                                case Forward: {
                                    motorObj.direction = 'F';
                                    motorObj.distance = value;
                                    MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                    break;
                                }
                                // Send reverse command to motor_controller thread
                                case Back: {
                                    motorObj.direction = 'B';
                                    motorObj.distance = value;
                                    MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                    break;
                                }
                                // Send left command to motor_controller thread
                                case Left: {
                                    motorObj.direction = 'L';
                                    motorObj.degrees = value;
                                    MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                    break;
                                }
                                // Send right command to motor_controller thread
                                case Right: {
                                    motorObj.direction = 'R';
                                    motorObj.degrees = value;
                                    MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                    break;
                                }
                                // Send start/continue command to motor_controller thread
                                case Start: {
                                    motorObj.stop = 'N';
                                    stopSending = false;
                                    MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                    break;
                                }
                                // Send stop command to motor_controller thread
                                case Stop: {
                                    motorObj.stop = 'Y';
                                    stopSending = true;
                                    MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                    break;
                                }
                                // Handle request for line sensor data
                                case LineLocation: {
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"LineLocation\":{\"0\":\"%0.02f\",\"1\":\"%0.02f\",\"2\":\"%0.02f\","
                                            "\"3\":\"%0.02f\",\"4\":\"%0.02f\",\"5\":\"%0.02f\",\"6\":\"%0.02f\",\"7\":\"%0.02f\"}", 
                                            internalData.lineLocation.IR_0,
                                            internalData.lineLocation.IR_1,
                                            internalData.lineLocation.IR_2,
                                            internalData.lineLocation.IR_3,
                                            internalData.lineLocation.IR_4,
                                            internalData.lineLocation.IR_5,
                                            internalData.lineLocation.IR_6,
                                            internalData.lineLocation.IR_7);
                                    break;
                                }
                                // Handle request for local system time
                                case msLocalTime:{
                                    sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data), ",\"msLocalTime\":\"%d\"", getSystemClock() * 200);
                                    tx_thread_obj.Destination = messageObj.External.Source;
                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                        sprintf(tx_thread_obj.Data+strlen(tx_thread_obj.Data),"}"); // Print ending brace }
                        // Increment response counter to source
                        switch(messageObj.External.Source) {
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
                        // Check source of response
                        switch(messageObj.External.Source) {
                            case SEARCHERMOVER: {
                                statObject.Res_From_SearcherMover++;
                                break;
                            }
                            case TARGETLOCATOR: {
                                statObject.Res_From_TargetLocator++;
                                
                                int i = 0;
                                for(i = 0; i < numItems; i++) {
                                    switch(items[i]) {
                                        // Send sensor data to motor_controller thread
                                        case SensorData: {
                                            motorObj.sensorData = data;
                                            MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                            break;
                                        }
                                        // Send updated position to motor_controller thread
                                        case R1_Location: {
                                            motorObj.type = UPDATE_POSITION;
                                            MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                            break;
                                        }
                                        default: {
                                            break;
                                        }
                                    }
                                }
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
                                int i = 0;
                                for(i = 0; i < numItems; i++) {
                                    switch(items[i]) {
                                        // Handle initial data from server
                                        case InitialData: {
                                            // Assert initialized bool to begin operation of rover
                                            initialized = true;
                                            motorObj.type = UPDATE_POSITION;
                                            MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                            break;
                                        }
                                        // Send line tuning data to adc thread
                                        case LineTuning: {
                                            ADC_THREAD_TuneLineSensor(lineProperties.color, lineProperties.threshold);
                                            break;
                                        }
                                        // Send sensor data to motor_controller thread
                                        case SensorData: {
                                            motorObj.sensorData = data;
                                            MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                            break;
                                        }
                                        // Send updated position to motor_controller thread
                                        case R1_Location: {
                                            motorObj.type = UPDATE_POSITION;
                                            MOTOR_CONTROLLER_THREAD_SendToQueue(motorObj);
                                            break;
                                        }
                                        default: {
                                            break;
                                        }
                                    }
                                }
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
                // Request initial data until it is received
                if (!initialized) {
                    sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"InitialData\"]}");
                    tx_thread_obj.Destination = SERVER;
                } else {
                    // Send requests to appropriate modules based on MYNAME
                    switch(messageObj.Request) {
                        case REQ_TO_TL: {
                            if (!stopSending) {
                                sprintf(tx_thread_obj.Data, "{\"type\":\"Request\",\"items\":[\"SensorData\",\"R1_Location\"]}");
                                tx_thread_obj.Destination = TARGETLOCATOR;
                                tx_thread_obj.MessageCount = statObject.Req_To_TargetLocator;
                                statObject.Req_To_TargetLocator++;
                            }
                            break;
                        }
                        default: {
                            continue;
                        }
                    }
                }
                if (!stopSending) {
                    TX_THREAD_SendToQueue(tx_thread_obj);
                }
                break;
            }
            case UPDATE: {
                dbgOutputLoc(CASE_UPDATE_MESSAGE_CONTROLLER_THREAD);
                switch(messageObj.Update.Type) {
                    // Update internal location, orientation, and movement
                    case MOVEMENT: {
                        internalData.location.x = messageObj.Update.Data.location.x;
                        internalData.location.y = messageObj.Update.Data.location.y;
                        internalData.orientation = messageObj.Update.Data.orientation;
                        internalData.movement.action = messageObj.Update.Data.movement.action;
                        internalData.movement.amount = messageObj.Update.Data.movement.amount;
                        break;
                    }
                    // Update internal line sensor readings
                    case LINELOCATION: {
                        internalData.lineLocation = messageObj.Update.Data.lineLocation;
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
