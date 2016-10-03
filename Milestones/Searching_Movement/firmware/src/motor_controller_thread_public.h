/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    motor_controller_thread_public.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _MOTOR_CONTROLLER_THREAD_PUBLIC_H    /* Guard against multiple inclusion */
#define _MOTOR_CONTROLLER_THREAD_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

void MOTOR_CONTROLLER_THREAD_SendToQueue(char buffer);

void MOTOR_CONTROLLER_THREAD_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
