/* ************************************************************************** */
/** Descriptive File Name

  @Company
    N/A

  @File Name
    app1_public.h

  @Summary
    This header file will expose public functions for sending data to queues.

  @Description
    This header file will expose public functions for sending data to queues.
 */
/* ************************************************************************** */

#ifndef _APP1_PUBLIC_H
#define _APP1_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/**
 * [APP1_SendToQueue Function to send data to APP1's queue]
 * @param  buffer [value to send to the queue]
 * @return        [error code; 1 if success, 0 otherwise]
 */
int APP1_SendToQueue(char buffer);

/**
 * [APP1_SendToQueueISR Function to send data to APP1's queue from an ISR]
 * @param  buffer [value to send to the queue]
 * @return        [error code; 1 if success, 0 otherwise]
 */
int APP1_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken);


#endif
/* *****************************************************************************
 End of File
 */
