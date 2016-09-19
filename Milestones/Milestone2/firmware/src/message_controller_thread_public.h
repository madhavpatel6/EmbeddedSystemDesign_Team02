/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _MESSAGE_CONTROLLER_THREAD_PUBLIC_H    /* Guard against multiple inclusion */
#define _MESSAGE_CONTROLLER_THREAD_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#ifdef __cplusplus
{
#endif

void MESSAGE_CONTROLLER_THREAD_SendToQueue(MessageObj buffer);

void MESSAGE_CONTROLLER_THREAD_SendToQueueISR(MessageObj buffer, BaseType_t *pxHigherPriorityTaskWoken);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MESSAGE_CONTROLLER_THREAD_PUBLIC_H */

/* *****************************************************************************
 End of File
 */
