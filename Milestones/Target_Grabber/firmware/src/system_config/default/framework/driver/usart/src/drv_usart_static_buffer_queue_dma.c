/*******************************************************************************
  USART driver static implementation of Buffer Queue model with DMA support.

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart_static_buffer_queue_dma.c

  Summary:
    Source code for the USART driver static implementation of Buffer queue model
     with DMA support.

  Description:
    This file contains the source code for the static implementation of the
    USART driver Buffer queue routines supporting DMA mode(Using DMA channels for data
    transmission and reception).

  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.

    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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
//DOM-IGNORE-END
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "system_config.h"
#include "system_definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global section
// *****************************************************************************
// *****************************************************************************

/* This is the array of USART Driver Buffer object of combined Queue Depth length*/
DRV_USART_BUFFER_OBJ gDrvUSARTBufferObj[DRV_USART_QUEUE_DEPTH_COMBINED];

/*This a global token counter used to generate unique buffer handles*/
static uint16_t gDrvUSARTDMATokenCount = 0;

/*File scope function*/
static DRV_USART_BUFFER_OBJECT_INDEX _DRV_USART_QueueObjectIndexGet(void);


/*File scope function*/
static void _DRV_USART0_DMA_EventHandler(SYS_DMA_TRANSFER_EVENT,
        SYS_DMA_CHANNEL_HANDLE, uintptr_t);

/*Driver instance objects*/
extern DRV_USART_OBJ  gDrvUSART0Obj ;
// *****************************************************************************
// *****************************************************************************
// Section: Instance 0 static driver functions
// *****************************************************************************
// *****************************************************************************
void DRV_USART0_BufferAddWrite (DRV_USART_BUFFER_HANDLE * bufferHandle, void * source, const size_t nBytes)
{
    /* This function adds a buffer to the write queue */
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    DRV_USART_BUFFER_OBJ * bufferObj = NULL;
    DRV_USART_BUFFER_OBJ * iterator = NULL;
    bool interruptWasEnabled = false;
    DRV_USART_BUFFER_OBJECT_INDEX usartBufIndex;
    dObj = &gDrvUSART0Obj;

    /* This function adds a buffer to the write queue. We first check the
     * arguments and initialize the buffer handle */

    /* Check the arguments */
    if((0 == nBytes) || (NULL == source) || (NULL == bufferHandle))
    {
        /* We either got an invalid buffer handle, invalid source pointer or 0
         * bytes to transfer */

        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Invalid parameters");
        return;
    }

    /*Initialize the buffer handle */
    *bufferHandle = DRV_USART_BUFFER_HANDLE_INVALID;

    /*Comparing with the Transmit Queue size*/
    if(dObj->queueSizeCurrentWrite >= 10)
    {
        /* This means the queue is full. We cannot add this request */

        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Transmit Queue is full");
        return;
    }

    /* We will allow buffers to be added in the interrupt context of this USART
     * driver. But we must make sure that if we are in interrupt, then we should
     * not modify mutexes. */

    if(dObj->interruptNestingCount == 0)
    {
        /* Grab a mutex. This is okay because we are not in an
           interrupt context */

        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
        {
            /* We were able to get the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an invalid handle.
             * This code will not execute if there is no RTOS.  */
            return;
        }

        /* We will disable interrupts so that the queue status does not get
         * updated asynchronously.  This code will always execute. */

        if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleWrite)
        {
            interruptWasEnabled = SYS_INT_SourceDisable(INT_SOURCE_DMA_0);
        }
        else
        {
            interruptWasEnabled = SYS_INT_SourceDisable(INT_SOURCE_USART_1_TRANSMIT);
        }
    }

    usartBufIndex = _DRV_USART_QueueObjectIndexGet();
    if (usartBufIndex != DRV_USART_BUFFER_OBJECT_INDEX_INVALID)
    {
        bufferObj = &gDrvUSARTBufferObj[usartBufIndex];
        bufferObj->bufferHandle = _DRV_USART_DMA_MAKE_HANDLE(gDrvUSARTDMATokenCount, usartBufIndex);
        *bufferHandle = bufferObj->bufferHandle;

        _DRV_USART_DMA_UPDATE_BUFFER_TOKEN(gDrvUSARTDMATokenCount);

        bufferObj->size     = nBytes;
        bufferObj->inUse    = true;
        bufferObj->buffer   = (uint8_t*)source;
        bufferObj->nCurrentBytes = 0;
        bufferObj->next = NULL;
        bufferObj->previous = NULL;
        bufferObj->flags = (DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD);
    }

    else
    {
        /* This means we could not find a buffer. This will happen if the the
         * DRV_USART_QUEUE_DEPTH_COMBINED parameter is configured to be less */

        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Insufficient Combined Queue Depth");

        /* Enable the interrupt if it was enabled */
        if(interruptWasEnabled)
        {
            if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleWrite)
            {
                SYS_INT_SourceEnable(INT_SOURCE_DMA_0);
            }
            else
            {
                SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
            }
        }
        if(dObj->interruptNestingCount == 0)
        {
            /* Release mutex */
            OSAL_MUTEX_Unlock(&(dObj->mutexDriverInstance));
        }

        return;
    }

    /* Increment the current queue size*/
    dObj->queueSizeCurrentWrite ++;

    /* Check if the queue is empty */
    if(dObj->queueWrite == NULL)
    {
        /* This is the first buffer in the
           queue */

        dObj->queueWrite = bufferObj;

        /* Enabling the interrupt here will cause the task routine to start
         * processing this buffer. If this function is being called in an ISR,
         * then this statement will not have any effect. */

        /* Checking whether the transfer uses DMA channels */
        if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleWrite)
        {
            /* Since this is the first buffer in the queue Add it immediately to
             * DMA for processing */

            size_t destSize, cellSize;
            destSize = 1;
            cellSize = 1;
            SYS_DMA_ChannelTransferAdd(dObj->dmaChannelHandleWrite, bufferObj->buffer, bufferObj->size,
                            PLIB_USART_TransmitterAddressGet(USART_ID_1), destSize,cellSize);
            _SYS_DMA_ChannelForceStart(dObj->dmaChannelHandleWrite);
            SYS_INT_SourceEnable(INT_SOURCE_DMA_0);
        }
        else
        {
            SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
        }
    }
    else
    {
        /* This means the write queue is not empty. We must add the buffer
         * object to the end of the queue */

        iterator = dObj->queueWrite;
        while(iterator->next != NULL)
        {
            /* Get the next buffer object */
            iterator = iterator->next;
        }

        /* At this point, iterator will point to the last object in the queue.
         * We add the buffer object to the linked list. Note that we need to set
         * up the previous pointer as well because buffer should be deleted when
         * the client closes the driver */

        iterator->next = bufferObj;
        bufferObj->previous = iterator;

        /* We are done. Restore the interrupt enable status
           and return. */

        if(interruptWasEnabled)
        {
            if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleWrite)
            {
                /* There is already a buffer under processing in the queue.
                 * This buffer will be added to DMA for processing immediately
                 * after the processing of the buffer prior to this buffer
                 * completes.  (This functionality is implemented in
                 * _DRV_USART_BufferQueueTxTasks)*/

                _SYS_DMA_ChannelForceStart(dObj->dmaChannelHandleWrite);
                SYS_INT_SourceEnable(INT_SOURCE_DMA_0);
            }
            else
            {
                SYS_INT_SourceEnable(INT_SOURCE_USART_1_TRANSMIT);
            }
        }
    }
    if(dObj->interruptNestingCount == 0)
    {
        /* Release mutex */
        OSAL_MUTEX_Unlock(&(dObj->mutexDriverInstance));
    }
    return;
}


void DRV_USART0_BufferAddRead(DRV_USART_BUFFER_HANDLE * const bufferHandle,void * destination,const size_t nBytes)
{
    /* This function adds a buffer to the read queue */
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    DRV_USART_BUFFER_OBJ * bufferObj = NULL;
    DRV_USART_BUFFER_OBJ * iterator = NULL;
    bool interruptWasEnabled = false;
    DRV_USART_BUFFER_OBJECT_INDEX usartBufIndex;

    dObj = &gDrvUSART0Obj;

    /* This function adds a buffer to the read queue. We first check the
     * arguments and initialize the buffer handle */
 
    /* Check the arguments*/
    if((nBytes == 0) || (NULL == destination) || (bufferHandle == NULL))
    {
        /* We either got an invalid buffer handle, invalid destination pointer
         * or 0 bytes to transfer */

        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Invalid parameters");
        return;
    }

    /* Initialize the buffer handle */
    *bufferHandle = DRV_USART_BUFFER_HANDLE_INVALID;

    /* Compare with the Receiver Queue size */
    if(dObj->queueSizeCurrentRead >= 10)
    {
        /* This means the queue is full. We cannot add this request */

        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Receive Queue is full");
        return;
    }

    /* We will allow buffers to be added in the interrupt context of this USART
     * driver. But we must make sure that if we are in interrupt, then we should
     * not modify mutexes. */

    if(dObj->interruptNestingCount == 0)
    {
        /* Grab a mutex. This is okay because we are not in an interrupt context
         * */

        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
        {
            /* We were able to get the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an invalid handle.
             * This code will not execute if there is no RTOS. */
            return;
        }
        /* We will disable interrupts so that the queue status does not get
         * updated asynchronously.  This code will always execute. */

        if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleRead)
        {
            interruptWasEnabled = SYS_INT_SourceDisable(INT_SOURCE_DMA_1);
        }
        else
        {
            interruptWasEnabled = SYS_INT_SourceDisable(INT_SOURCE_USART_1_RECEIVE);
        }
    }

    usartBufIndex = _DRV_USART_QueueObjectIndexGet();
    if (usartBufIndex != DRV_USART_BUFFER_OBJECT_INDEX_INVALID)
    {
        bufferObj = &gDrvUSARTBufferObj[usartBufIndex];
        bufferObj->bufferHandle = _DRV_USART_DMA_MAKE_HANDLE(gDrvUSARTDMATokenCount, usartBufIndex);
        *bufferHandle = bufferObj->bufferHandle;

        _DRV_USART_DMA_UPDATE_BUFFER_TOKEN(gDrvUSARTDMATokenCount);

        bufferObj->size     = nBytes;
        bufferObj->inUse    = true;
        bufferObj->buffer   = (uint8_t*)destination;
        bufferObj->next     = NULL;
        bufferObj->previous = NULL;
        bufferObj->nCurrentBytes = 0;
        bufferObj->flags = (DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD);
    }

    else
    {
        /* This means we could not find a buffer. This will happen if the the
         * DRV_USART_QUEUE_DEPTH_COMBINED parameter is configured to be less */

        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Insufficient Combined Queue Depth");

        /* Enable the interrupt if it was enabled */
        if(interruptWasEnabled)
        {
            if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleRead)
            {
                SYS_INT_SourceEnable(INT_SOURCE_DMA_1);
            }
            else
            {
                SYS_INT_SourceEnable(INT_SOURCE_USART_1_RECEIVE);
            }
        }
        if(dObj->interruptNestingCount == 0)
        {
            /* Release mutex */
            OSAL_MUTEX_Unlock(&(dObj->mutexDriverInstance));
        }
        return;
    }

    /* Increment the current queue size*/
    dObj->queueSizeCurrentRead ++;

    /* Check if the queue is empty */
    if(dObj->queueRead == NULL)
    {
        /* This is the first buffer in the queue */


        dObj->queueRead = bufferObj;

        /* This is the first item in the queue. Enable RX interrupt. */


        if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleRead)
        {
            /* Since this is the first buffer in the queue Add it immediately to
             * DMA for processing */

            size_t srcSize, cellSize;
            srcSize = 1;
            cellSize = 1;
            SYS_DMA_ChannelTransferAdd(dObj->dmaChannelHandleRead,
                    PLIB_USART_ReceiverAddressGet(USART_ID_1),
                    srcSize,bufferObj->buffer, bufferObj->size,
                    cellSize);
            _SYS_DMA_ChannelForceStart(dObj->dmaChannelHandleRead);
            SYS_INT_SourceEnable(INT_SOURCE_DMA_1);
        }
        else
        {
            SYS_INT_SourceEnable(INT_SOURCE_USART_1_RECEIVE);

        }
    }
    else
    {
        /* This means the read queue is not empty. We must add the buffer object
         * to the end of the queue */

        iterator = dObj->queueRead;
        while(iterator->next != NULL)
        {
            /* Get the next buffer object */
            iterator = iterator->next;
        }

        /* At this point, iterator will point to the last object in the queue.
         * We add the buffer object to the linked list. Note that we need to set
         * up the previous pointer as well because buffer should be deleted when
         * the client closes the driver */

        iterator->next = bufferObj;
        bufferObj->previous = iterator;

        /* We are done. Restore the interrupt enable status
           and return. */

        if(interruptWasEnabled)
        {
            if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleRead)
            {
               /* There is already a buffer under processing in the queue.  This
                * buffer will be added to DMA for processing immediately after
                * the processing of the buffer prior to this buffer completes.
                * (This functionality is implemented in
                * _DRV_USART_BufferQueueRxTasks)*/

                _SYS_DMA_ChannelForceStart(dObj->dmaChannelHandleRead);
                SYS_INT_SourceEnable(INT_SOURCE_DMA_1);
            }
            else
            {
                SYS_INT_SourceEnable(INT_SOURCE_USART_1_RECEIVE);
            }
        }
    }
    if(dObj->interruptNestingCount == 0)
    {
        /* Release mutex */
        OSAL_MUTEX_Unlock(&(dObj->mutexDriverInstance));
    }
    return;
}


void DRV_USART0_BufferEventHandlerSet(const DRV_USART_BUFFER_EVENT_HANDLER eventHandler,const uintptr_t context)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART0Obj;

    /* Register the event handler with the client */
    dObj->eventHandler = eventHandler;
    dObj->context = context;

    /* Setup the Event handler for the DMA TX channel interrupts */
    if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleWrite )
    {
        SYS_DMA_ChannelTransferEventHandlerSet(dObj->dmaChannelHandleWrite,
            (SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER)_DRV_USART0_DMA_EventHandler,
            (uintptr_t)dObj);
    }
    /* Setup the Event handler for the DMA Rx channel interrupts */
    if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleRead)
    {
        SYS_DMA_ChannelTransferEventHandlerSet(dObj->dmaChannelHandleRead,
            (SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER)_DRV_USART0_DMA_EventHandler,
            (uintptr_t)dObj);
    }
}

size_t DRV_USART0_BufferProcessedSizeGet( DRV_USART_BUFFER_HANDLE bufferHandle )
{
    DRV_USART_BUFFER_OBJ * bufferObj;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    size_t processedBytes = 0;

    dObj = &gDrvUSART0Obj;

    /* Validate the handle */
    if(DRV_USART_BUFFER_HANDLE_INVALID == bufferHandle ||
            (0 == bufferHandle))
    {
        return 0;
    }

    /* The buffer index is the contained in the lower 16 bits of the buffer
     * handle */
    bufferObj = (DRV_USART_BUFFER_OBJ *)&gDrvUSARTBufferObj[bufferHandle & 0xFFFF];

    if(!bufferObj->inUse)
    {
        return 0;
    }

    processedBytes= SYS_DMA_ChannelSourceTransferredSizeGet(dObj->dmaChannelHandleWrite);
    processedBytes+= SYS_DMA_ChannelDestinationTransferredSizeGet(dObj->dmaChannelHandleRead);

    /* Return the number of processed bytes */

    return processedBytes;
}

// *****************************************************************************
// *****************************************************************************
// Section: File scope functions
// *****************************************************************************
// *****************************************************************************
/*******************************************************************************
  Function:
    void _DRV_USART0_DMA_EventHandler
    (
        SYS_DMA_TRANSFER_EVENT event,
        SYS_DMA_CHANNEL_HANDLE handle,
        uintptr_t contextHandle
    )

  Summary:
    Callback for the DMA channels

  Description:
    Callback for the DMA channels

  Remarks:
    None
 */
static void _DRV_USART0_DMA_EventHandler
(
    SYS_DMA_TRANSFER_EVENT event,
    SYS_DMA_CHANNEL_HANDLE handle,
    uintptr_t contextHandle
)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    DRV_USART_BUFFER_OBJ *bufObject;

    dObj = &gDrvUSART0Obj;

    if(SYS_DMA_CHANNEL_HANDLE_INVALID == handle || 0 == handle ||
            0 == contextHandle )
    {
        /* This means the handle is invalid */
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nHandle is invalid.");
        return;
    }

    if(handle == dObj->dmaChannelHandleWrite)
    {
        bufObject = dObj->queueWrite;
    }
    else
    {
        bufObject = dObj->queueRead;
    }

    if(SYS_DMA_TRANSFER_EVENT_COMPLETE == event)
    {
        bufObject->size = 0;
        dObj->bufferEvent = DRV_USART_BUFFER_EVENT_COMPLETE;
    }
    else if(SYS_DMA_TRANSFER_EVENT_ABORT == event)
    {
        bufObject->size = DRV_USART_DMA_TRANSFER_ABORT;
        dObj->bufferEvent = DRV_USART_BUFFER_EVENT_ABORT;
    }
    else if(SYS_DMA_TRANSFER_EVENT_ERROR == event)
    {
        bufObject->size = DRV_USART_DMA_TRANSFER_ERROR;
        dObj->bufferEvent = DRV_USART_BUFFER_EVENT_ERROR;
    }
    else
    {
        ;
    }

    /* Call the USART Tasks */
    if(handle == dObj->dmaChannelHandleWrite)
    {
        DRV_USART0_TasksTransmit();
    }
    else if(handle == dObj->dmaChannelHandleRead)
    {
        DRV_USART0_TasksReceive();
    }

    /*To be Enabled when DRV_USART_TasksError function is implemented */
    //DRV_USART_TasksError();
}

static DRV_USART_BUFFER_OBJECT_INDEX _DRV_USART_QueueObjectIndexGet(void)
{
    DRV_USART_BUFFER_OBJ *queueObj;
    uint32_t index;

    queueObj = &gDrvUSARTBufferObj[0];
    for (index = 0; index < DRV_USART_QUEUE_DEPTH_COMBINED; index++)
    {
        if (false == queueObj->inUse)
        {
            queueObj->inUse = true;
            return index;
        }
        queueObj++;
    }
    return DRV_USART_BUFFER_OBJECT_INDEX_INVALID;
}
/*******************************************************************************
 End of File
*/
