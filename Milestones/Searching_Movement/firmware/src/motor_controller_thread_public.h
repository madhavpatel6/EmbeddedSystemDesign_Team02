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

typedef struct {
    char mode;
    char direction;
    float distance;
    float degrees;
    uint8_t lineLocation;
} MotorObj;
    
void MOTOR_CONTROLLER_THREAD_SendToQueue(MotorObj obj);

void MOTOR_CONTROLLER_THREAD_SendToQueueISR(MotorObj obj, BaseType_t *pxHigherPriorityTaskWoken);

void MOTOR_CONTROLLER_THREAD_CorrectSpeed(int timer);

void MOTOR_CONTROLLER_THREAD_IncrementRight(void);

void MOTOR_CONTROLLER_THREAD_IncrementLeft(void);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
