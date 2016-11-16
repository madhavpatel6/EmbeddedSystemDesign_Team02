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
    
typedef enum {vertex, target, obstacle, update, targetAlignment, debug} coord_t;

typedef struct {
    float x[10];
    float y[10];
    // len could be the array length or the length of the turn or distance
    int len;
    coord_t type;
    int cacheType;
    int cacheDistance;
    float targetDistance;
    float targetAngle;
    bool targetAligned;
    bool targetAcquired;
} message_in_t;

void MOTOR_CONTROLLER_THREAD_SendToQueue(message_in_t buffer);

void MOTOR_CONTROLLER_THREAD_SendToQueueISR(message_in_t buffer, BaseType_t *pxHigherPriorityTaskWoken);

void MOTOR_CONTROLLER_THREAD_ReadFromQueue(message_in_t* pvBuffer);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
