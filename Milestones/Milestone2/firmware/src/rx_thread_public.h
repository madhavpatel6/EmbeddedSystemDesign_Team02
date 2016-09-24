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

#ifndef _RX_THREAD_PUBLIC_H    /* Guard against multiple inclusion */
#define _RX_THREAD_PUBLIC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include "system_config.h"
#include "system_definitions.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

void RX_THREAD_SendToQueue(char buffer);

void RX_THREAD_SendToQueueISR(char buffer, BaseType_t *pxHigherPriorityTaskWoken);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
