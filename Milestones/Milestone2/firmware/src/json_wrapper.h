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

#ifndef _JSON_WRAPPER_H    /* Guard against multiple inclusion */
#define _JSON_WRAPPER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "communication/jsmn.h"

typedef enum {unknown, request, response} type_t;
typedef enum {commStats, sensorData} items_t;

typedef struct {
    type_t type;
    items_t items[12];
    int numItems;
} JSONObjType;

static jsmn_parser p;
static jsmntok_t t[128]; /* We expect no more than 128 tokens */

static char buf[255];

int jsoneq(const char *json, jsmntok_t *tok, const char *s) ;

void initParser();

void parseJSON(const char* JSON_STRING, type_t *type, items_t items[], int *numItems);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
