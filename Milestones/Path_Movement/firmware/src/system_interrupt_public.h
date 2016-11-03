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

#ifndef _SYSTEM_INTERRUPT_PUBLIC_H    /* Guard against multiple inclusion */
#define _SYSTEM_INTERRUPT_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

void InitializeISRQueues();

void Usart0_SendToQueue(char buffer);

void Usart0_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken);

void addMotorTask(int type, int distance);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
