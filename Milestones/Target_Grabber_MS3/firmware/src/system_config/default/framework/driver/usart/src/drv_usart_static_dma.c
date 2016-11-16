/*******************************************************************************
  USART Driver Static implementation with DMA support

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart_static_dma.c

  Summary:
    Source code for the USART driver static implementation with DMA support.

  Description:
    The USART device driver provides a simple interface to manage the USART
    modules on Microchip microcontrollers. This file contains static implementation
    for the USART driver supporting DMA mode(Using DMA channels for data transmission
    and reception).

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
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver static object . */
DRV_USART_OBJ  gDrvUSART0Obj ;

// *****************************************************************************
// *****************************************************************************
// Section: Instance 0 static driver functions
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_USART0_Initialize(void)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    uint32_t clockSource;

    dObj = &gDrvUSART0Obj;

    /* Disable the USART module to configure it*/
    PLIB_USART_Disable (USART_ID_1);

    /* Update the USART STATIC OBJECT parameters. */
    dObj->interruptNestingCount = 0;
    dObj->queueSizeCurrentRead  = 0;
    dObj->queueSizeCurrentWrite = 0;
    dObj->queueRead             = NULL;
    dObj->queueWrite            = NULL;
    dObj->eventHandler          = NULL;
    dObj->context               = (uintptr_t)NULL;
    dObj->error                 = DRV_USART_ERROR_NONE;
    dObj->dmaChannelHandleWrite = SYS_DMA_CHANNEL_HANDLE_INVALID;
    dObj->dmaChannelHandleRead  = SYS_DMA_CHANNEL_HANDLE_INVALID;

    /* Create the hardware instance mutex. */
     if(OSAL_MUTEX_Create(&(dObj->mutexDriverInstance)) != OSAL_RESULT_TRUE)
     {
        return SYS_MODULE_OBJ_INVALID;
     }
    /* DMA mode of operation. Allocate a handle for the specified channel.
     * Setup the channel for transfer */

    /* TX DMA channel setup */
    if( DMA_CHANNEL_NONE != DMA_CHANNEL_0 &&
        DMA_NUMBER_OF_CHANNELS != DMA_CHANNEL_0)
    {
        dObj->dmaChannelHandleWrite = SYS_DMA_ChannelAllocate(DMA_CHANNEL_0);
        if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleWrite)
        {
            /* Setting the channel priority same as the channel number */
            SYS_DMA_ChannelSetup(dObj->dmaChannelHandleWrite,
                SYS_DMA_CHANNEL_OP_MODE_BASIC, INT_SOURCE_USART_1_TRANSMIT);
        }
    }

    /* RX DMA channel setup */
    if(DMA_CHANNEL_NONE != DMA_CHANNEL_1 &&
        DMA_NUMBER_OF_CHANNELS != DMA_CHANNEL_1)
    {
        dObj->dmaChannelHandleRead = SYS_DMA_ChannelAllocate(DMA_CHANNEL_1);
        if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleRead)
        {
            /* Setting the channel priority same as the channel number */
            SYS_DMA_ChannelSetup(dObj->dmaChannelHandleRead,
                SYS_DMA_CHANNEL_OP_MODE_BASIC, INT_SOURCE_USART_1_RECEIVE);
        }
    }


    /* Initialize the USART based on configuration settings */
    PLIB_USART_InitializeModeGeneral(USART_ID_1,
            false,  /*Auto baud*/
            false,  /*LoopBack mode*/
            false,  /*Auto wakeup on start*/
            false,  /*IRDA mode*/
            false);  /*Stop In Idle mode*/

    /* Set the line control mode */
    PLIB_USART_LineControlModeSelect(USART_ID_1, DRV_USART_LINE_CONTROL_8NONE1);

    /* We set the receive interrupt mode to receive an interrupt whenever FIFO
       is not empty */
    PLIB_USART_InitializeOperation(USART_ID_1,
            USART_RECEIVE_FIFO_ONE_CHAR,
            USART_TRANSMIT_FIFO_IDLE,
            USART_ENABLE_TX_RX_USED);

    /* Get the USART clock source value*/
    clockSource = SYS_CLK_PeripheralFrequencyGet ( CLK_BUS_PERIPHERAL_1 );

    /* Set the baud rate and enable the USART */
    PLIB_USART_BaudSetAndEnable(USART_ID_1,
            clockSource,
            9600);  /*Desired Baud rate value*/

    /* Clear the interrupts to be on the safer side*/
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_TRANSMIT);
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_RECEIVE);
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_ERROR);
    SYS_INT_SourceStatusClear(INT_SOURCE_DMA_0);
    SYS_INT_SourceStatusClear(INT_SOURCE_DMA_1);

    /* Enable the error interrupt source */
    SYS_INT_SourceEnable(INT_SOURCE_USART_1_ERROR);


    /* Return the driver instance value*/
    return (SYS_MODULE_OBJ)DRV_USART_INDEX_0;
}

void  DRV_USART0_Deinitialize(void)
{
    bool status;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART0Obj;


    /* Disable the interrupts */
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_TRANSMIT) ;
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_RECEIVE) ;
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_ERROR);
    status = SYS_INT_SourceDisable(INT_SOURCE_DMA_0);
    status = SYS_INT_SourceDisable(INT_SOURCE_DMA_1);
    /* Ignore the warning */
    (void)status;

    /* Deallocate the allocated DMA channel handles  */
    if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleWrite)
    {
        SYS_DMA_ChannelRelease(dObj->dmaChannelHandleWrite);
    }
    if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleRead)
    {
        SYS_DMA_ChannelRelease(dObj->dmaChannelHandleRead);
    }

    /* Disable USART module */
    PLIB_USART_Disable (USART_ID_1);

    /* Deallocate all mutexes */
    if(OSAL_MUTEX_Delete(&(dObj->mutexDriverInstance)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Mutex Delete Failed");
        return;
    }

}


SYS_STATUS DRV_USART0_Status(void)
{
    /* Return the status as ready always */
    return SYS_STATUS_READY;
}

void DRV_USART0_TasksTransmit(void)
{
    /* This is the USART Driver Transmit tasks routine.
       In this function, the driver checks if a transmit
       interrupt is active and if there are any buffers in
       queue. If so the buffer is serviced. A buffer that
       is serviced completely is removed from the queue.
     */

    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_TRANSMIT) ||
       SYS_INT_SourceStatusGet(INT_SOURCE_DMA_0))
    {
        /* The USART driver is configured to generate an
           interrupt when the FIFO is empty. Additionally
           the queue is not empty. Which means there is
           work to be done in this routine. */

        _DRV_USART0_BufferQueueTxTasks();

        /* Checking whether the transfer uses DMA channels */
        if(SYS_DMA_CHANNEL_HANDLE_INVALID == gDrvUSART0Obj.dmaChannelHandleWrite)
        {

            /* Clear up the interrupt flag */
            SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_TRANSMIT);
        }
    }
}

void DRV_USART0_TasksReceive(void)
{
    /* This is the USART Driver Receive tasks routine. If the receive
       interrupt flag is set, the tasks routines are executed.
     */

    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_RECEIVE) ||
       SYS_INT_SourceStatusGet(INT_SOURCE_DMA_1))
    {
        _DRV_USART0_BufferQueueRxTasks();

        /* Checking whether the transfer uses DMA channels */
        if(SYS_DMA_CHANNEL_HANDLE_INVALID == gDrvUSART0Obj.dmaChannelHandleRead)
        {
            /* Clear up the interrupt flag */
            SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_RECEIVE);
        }
    }
}


void DRV_USART0_TasksError(void)
{
    /* This is the USART Driver Error tasks routine. In this function, the
     * driver checks if an error interrupt has occurred. If so the error
     * condition is cleared.  */

    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_ERROR))
    {
        /* This means an error has occurred */
        if(PLIB_USART_ReceiverOverrunHasOccurred(USART_ID_1))
        {
            PLIB_USART_ReceiverOverrunErrorClear(USART_ID_1);
        }

        /* Clear up the error interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_ERROR);
    }
}

DRV_HANDLE DRV_USART0_Open(const SYS_MODULE_INDEX index, const DRV_IO_INTENT ioIntent)
{

    /* Return the driver instance value*/
    return ((DRV_HANDLE)DRV_USART_INDEX_0 );
}

void DRV_USART0_Close(void)
{
    return;
}

DRV_USART_CLIENT_STATUS DRV_USART0_ClientStatus(void)
{
    /* Return the status as ready always*/
    return DRV_USART_CLIENT_STATUS_READY;
}

DRV_USART_TRANSFER_STATUS DRV_USART0_TransferStatus(void)
{
    DRV_USART_TRANSFER_STATUS result = 0;

    /* Check if RX data available */
    if(PLIB_USART_ReceiverDataIsAvailable(USART_ID_1))
    {
        result|= DRV_USART_TRANSFER_STATUS_RECEIVER_DATA_PRESENT;
    }
    else
    {
        result|= DRV_USART_TRANSFER_STATUS_RECEIVER_EMPTY;
    }

    /* Check if TX Buffer is empty */
    if(PLIB_USART_TransmitterIsEmpty(USART_ID_1))
    {
        result|= DRV_USART_TRANSFER_STATUS_TRANSMIT_EMPTY;
    }

    /* Check if the TX buffer is full */
    if(PLIB_USART_TransmitterBufferIsFull(USART_ID_1))
    {
        result|= DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL;
    }

    return(result);
}

// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************

void _DRV_USART0_BufferQueueRxTasks(void)
{
    DRV_USART_BUFFER_OBJ * bufferObj;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    bool status;

    dObj = &gDrvUSART0Obj;
    bufferObj = dObj->queueRead;

    /* If this driver is configured for polled mode in an RTOS, the tasks
       routine would be called from another thread. We need to get the driver
       instance mutex before updating the queue. If the driver is configured for
       interrupt mode, then _DRV_USART_TAKE_MUTEX will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
        {
            /* We were able to take the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an
               invalid handle. This code will not execute
               if there is no RTOS. */
            return;
        }
    }
    /* Checking whether the transfer uses DMA channels */
    if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleRead)
    {
        if(bufferObj != NULL)
        {
            /* If Data reception is completed or an data transfer
             * Abort has occurred */
            if(0 == bufferObj->size ||
               DRV_USART_DMA_TRANSFER_ABORT == bufferObj->size)
            {
                if((dObj->eventHandler != NULL) &&
                        (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
                {
                    dObj->interruptNestingCount ++;
                    dObj->eventHandler(dObj->bufferEvent,
                            bufferObj->bufferHandle,
                            dObj->context);
                    /* Decrement the nesting count */
                    dObj->interruptNestingCount -- ;
                }

                dObj->queueRead = bufferObj->next;
                bufferObj->inUse = false;
                dObj->queueSizeCurrentRead --;

                /* Reset the next and the previous pointers */
                bufferObj->next = NULL;
                bufferObj->previous = NULL;

                /* While the first buffer was getting processed, if any
                 * buffer requests were Added, Start the added buffer request
                 * by adding the request to DMA now */
                if(dObj->queueRead != NULL)
                {
                    size_t srcSize, cellSize;

                    srcSize = 1;
                    cellSize = 1;
                    bufferObj = dObj->queueRead;

                    /* Reset the current head's previous pointer */
                    bufferObj->previous = NULL;

                    SYS_DMA_ChannelTransferAdd(dObj->dmaChannelHandleRead,
                            PLIB_USART_ReceiverAddressGet(USART_ID_1),
                            srcSize, bufferObj->buffer, bufferObj->size,cellSize);
                }
            }

        }
        if(dObj->queueRead == NULL)
        {
            /* If the queue is empty, then disable the DMA Receive Interrupt */
            status = SYS_INT_SourceDisable(INT_SOURCE_DMA_1);
            /* Ignore the warning */
            (void)status;
        }
        /* Release the mutex */
        _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));
    }
    else
    {
        /* In this function, the driver checks if there are any buffers in queue. If
        so the buffer is serviced. A buffer that is serviced completely is
        removed from the queue. Start by getting the buffer at the head of the
        queue */
        if(bufferObj != NULL)
        {
            /* The USART driver is configured to generate an interrupt when the FIFO
               is not empty. Additionally the queue is not empty. Which means there
               is work to done in this routine. Read data from the FIFO till either
               the FIFO is empty or till we have read the requested number of bytes.
               */

            while((PLIB_USART_ReceiverDataIsAvailable(USART_ID_1))
                    && (bufferObj->nCurrentBytes < bufferObj->size ))
            {
                bufferObj->buffer[bufferObj->nCurrentBytes] = PLIB_USART_ReceiverByteReceive(USART_ID_1);
                bufferObj->nCurrentBytes ++;
            }

            /* Check if this buffer is done */

            if(bufferObj->nCurrentBytes >= bufferObj->size)
            {
                /* This means the buffer is completed. If there
                   is a callback registered with client, then
                   call it */

                if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
                {
                    /* Call the event handler. We additionally increment the
                       interrupt nesting count which lets the driver functions
                       that are called from the event handler know that an
                       interrupt context is active.
                       */

                    dObj->interruptNestingCount ++;

                    dObj->eventHandler(DRV_USART_BUFFER_EVENT_COMPLETE,
                            bufferObj->bufferHandle,
                            dObj->context);

                    dObj->interruptNestingCount --;
                }

                /* Get the next buffer in the queue and deallocate
                   this buffer */

                dObj->queueRead = bufferObj->next;
                bufferObj->inUse = false;
                dObj->queueSizeCurrentRead --;

                /* Reset the next and the previous pointers */
                bufferObj->next = NULL;
                bufferObj->previous = NULL;

                /* Reset the current head's previous pointer */
                if (dObj->queueRead != NULL)
                {
                    dObj->queueRead->previous = NULL;
                }

            }
        }
        if(dObj->queueRead == NULL)
        {
            /* The queue is empty. We can disable the interrupt */
            status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_RECEIVE);
            /* Ignore the warning */
            (void)status;
        }

        /* Release the mutex */
       _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));
    }
}

void _DRV_USART0_BufferQueueTxTasks(void)
{
    DRV_USART_BUFFER_OBJ * bufferObj;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    bool status;

    dObj = &gDrvUSART0Obj;
    bufferObj = dObj->queueWrite;

    /* If this driver is configured for polled mode in an RTOS, the tasks
       routine would be called from another thread. We need to get the driver
       instance mutex before updating the queue. If the driver is configured for
       interrupt mode, then _DRV_USART_TAKE_MUTEX will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER))
        {
            /* We were able to take the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an
               invalid handle. This code will not execute
               if there is no RTOS. */
            return;
        }
    }
    /* Checking whether the transfer uses DMA channels */
    if(SYS_DMA_CHANNEL_HANDLE_INVALID != dObj->dmaChannelHandleWrite)
    {
        if(bufferObj != NULL)
        {
            /* If Data transmission is completed or an data transfer
             * Abort has occurred */
            if(0==bufferObj->size ||
               DRV_USART_DMA_TRANSFER_ABORT == bufferObj->size)
            {
                if((dObj->eventHandler != NULL) &&
                        (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
                {
                    dObj->interruptNestingCount ++;
                    dObj->eventHandler(dObj->bufferEvent,
                            bufferObj->bufferHandle,
                            dObj->context);
                    /* Decrement the nesting count */
                    dObj->interruptNestingCount -- ;
                }

                dObj->queueWrite = bufferObj->next;
                bufferObj->inUse = false;
                dObj->queueSizeCurrentWrite --;

                /* Reset the next and the previous pointers */
                bufferObj->next = NULL;
                bufferObj->previous = NULL;

                /* While the first buffer was getting processed, if any
                 * buffer requests were Added, Start the added buffer request
                 * by adding the request to DMA now */
                if(dObj->queueWrite!=NULL)
                {
                    size_t destSize, cellSize;
                    destSize = 1;
                    cellSize = 1;

                    bufferObj = dObj->queueWrite;
                    /* Reset the current head's previous pointer */
                    bufferObj->previous = NULL;

                    SYS_DMA_ChannelTransferAdd(dObj->dmaChannelHandleWrite,
                            bufferObj->buffer, bufferObj->size,
                            PLIB_USART_TransmitterAddressGet(USART_ID_1),
                            destSize,cellSize);
                }
            }

        }
        if(dObj->queueWrite == NULL)
        {
            /* If the queue is empty, then disable the DMA Transmit Interrupt */
           // status = SYS_INT_SourceDisable(INT_SOURCE_DMA_0);
           /* Ignore the warning */
           // (void)status;
        }
        /* Release the mutex */
        _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));
    }
    else
    {
        if(bufferObj != NULL)
        {
            /* This means the queue is not empty. Check if this buffer is done */
            if(bufferObj->nCurrentBytes >= bufferObj->size)
            {
                /* This means the buffer is completed. If there
                   is a callback registered with client, then
                   call it */

                if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
                {
                    /* Before calling the event handler, the interrupt nesting
                       counter is incremented. This will allow driver routine that
                       are called from the event handler to know the interrupt
                       nesting level. Events are only generated for buffers that
                       were submitted using the buffer add routine */

                    dObj->interruptNestingCount ++;

                    dObj->eventHandler(DRV_USART_BUFFER_EVENT_COMPLETE,
                            bufferObj->bufferHandle,
                            dObj->context);

                    /* Decrement the nesting count */
                    dObj->interruptNestingCount -- ;
                }

                /* Get the next buffer in the queue and deallocate
                 * this buffer */

                dObj->queueWrite = bufferObj->next;
                bufferObj->inUse = false;
                dObj->queueSizeCurrentWrite --;

                /* Reset the next and the previous pointers */
                bufferObj->next = NULL;
                bufferObj->previous = NULL;

                /* Reset the current head's previous pointer */
                if (dObj->queueWrite != NULL)
                {
                    dObj->queueWrite->previous = NULL;
                }

            }
        }

        /* Check if the queue is still not empty and process
           the buffer */

        if(dObj->queueWrite != NULL)
        {
            bufferObj = dObj->queueWrite;

            /* Fill up the FIFO with data till the FIFO is full
               and we have data to send */
            while((!PLIB_USART_TransmitterBufferIsFull(USART_ID_1))
                    && (bufferObj->nCurrentBytes < bufferObj->size ))
            {
                PLIB_USART_TransmitterByteSend(USART_ID_1, bufferObj->buffer[bufferObj->nCurrentBytes]);
                bufferObj->nCurrentBytes ++;
            }
        }
        else
        {
            /* If the queue is empty, then disable the TX interrupt */
            status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_TRANSMIT);
            /* Ignore the warning */
            (void)status;
        }

        /* Release the mutex */
        _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));
    }

}


DRV_USART_BAUD_SET_RESULT DRV_USART0_BaudSet(uint32_t baud)
{
    uint32_t clockSource;
    int32_t brgValueLow=0;
    int32_t brgValueHigh=0;
    DRV_USART_BAUD_SET_RESULT retVal = DRV_USART_BAUD_SET_SUCCESS;
#if defined (PLIB_USART_ExistsModuleBusyStatus)
    bool isEnabled = false;
#endif

    /* Making this function thread safe */
    if(OSAL_MUTEX_Lock(&(gDrvUSART0Obj.mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        /* We were able to take the mutex */
    }
    else
    {
        /* The mutex timed out */
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Hardware instance mutex time out in DRV_USART_BaudSet() function");
        return(DRV_USART_BAUD_SET_ERROR);
    }

    /* Get the USART clock source value*/
    clockSource = SYS_CLK_PeripheralFrequencyGet ( CLK_BUS_PERIPHERAL_1 );

    /* Calculate low and high baud values */
    brgValueLow  = ( (clockSource/baud) >> 4 ) - 1;
    brgValueHigh = ( (clockSource/baud) >> 2 ) - 1;

#if defined (PLIB_USART_ExistsModuleBusyStatus)
        isEnabled = PLIB_USART_ModuleIsBusy (USART_ID_1);
        if (isEnabled)
        {
            PLIB_USART_Disable (USART_ID_1);
            while (PLIB_USART_ModuleIsBusy (USART_ID_1));
        }
#endif

    /* Check if the baud value can be set with high baud settings */
    if ((brgValueHigh >= 0) && (brgValueHigh <= UINT16_MAX))
    {
        PLIB_USART_BaudRateHighEnable(USART_ID_1);
        PLIB_USART_BaudRateHighSet(USART_ID_1,clockSource,baud);
    }
	
    /* Check if the baud value can be set with low baud settings */
    else if ((brgValueLow >= 0) && (brgValueLow <= UINT16_MAX))
    {
        PLIB_USART_BaudRateHighDisable(USART_ID_1);
        PLIB_USART_BaudRateSet(USART_ID_1, clockSource, baud);
    }
    else
    {
        retVal = DRV_USART_BAUD_SET_ERROR;
    }
#if defined (PLIB_USART_ExistsModuleBusyStatus)
    if (isEnabled)
    {
        PLIB_USART_Enable (USART_ID_1);
    }
#endif
    /* Unlock Mutex */
    OSAL_MUTEX_Unlock(&(gDrvUSART0Obj.mutexDriverInstance));

    return retVal;
}

DRV_USART_LINE_CONTROL_SET_RESULT DRV_USART0_LineControlSet(DRV_USART_LINE_CONTROL lineControlMode)
{
#if defined (PLIB_USART_ExistsModuleBusyStatus)
    bool isEnabled = false;
#endif

    /* Making this function thread safe */
    if(OSAL_MUTEX_Lock(&(gDrvUSART0Obj.mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        /* We were able to take the mutex */
    }
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Hardware Instance Mutex time out in DRV_USART_LineControlSet() function");
        return DRV_USART_LINE_CONTROL_SET_ERROR;
    }
#if defined (PLIB_USART_ExistsModuleBusyStatus)
        isEnabled = PLIB_USART_ModuleIsBusy (USART_ID_1);
        if (isEnabled)
        {
            PLIB_USART_Disable (USART_ID_1);
            while (PLIB_USART_ModuleIsBusy (USART_ID_1));
        }
#endif

    /* Set the Line Control Mode */
    PLIB_USART_LineControlModeSelect(USART_ID_1, lineControlMode);

#if defined (PLIB_USART_ExistsModuleBusyStatus)
        if (isEnabled)
        {
            PLIB_USART_Enable (USART_ID_1);
        }
#endif
    OSAL_MUTEX_Unlock(&(gDrvUSART0Obj.mutexDriverInstance));

    /* Return success */
    return(DRV_USART_LINE_CONTROL_SET_SUCCESS);
}

/*******************************************************************************
 End of File
*/
