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
#include "motor_controller_thread_public.h"

typedef enum {request, response, unknown} type_t;
typedef enum {CommStatsSearcherMover, CommStatsTargetLocator, CommStatsPathFinder, 
            CommStatsTargetGrabber, DetailedCommStatsSearcherMover, 
            DetailedCommStatsTargetLocator, DetailedCommStatsPathFinder,
            DetailedCommStatsTargetGrabber, R1_Movement, R1_Location, Forward, 
            Back, Left, Right, Start, Stop, InitialData, SensorData, LineLocation, 
            LineTuning, msLocalTime} items_t;

typedef struct {
  char stringValue[512];
  items_t enumValue;
} DictionaryType;

// Dictionary that json parser iterates through to find keys
static const DictionaryType Dictionary[] = {
    {"CommStatsSearcherMover", CommStatsSearcherMover},
    {"CommStatsTargetLocator", CommStatsTargetLocator},
    {"CommStatsPathFinder", CommStatsPathFinder},
    {"CommStatsTargetGrabber",CommStatsTargetGrabber},
    {"DetailedCommStatsSearcherMover", DetailedCommStatsSearcherMover},
    {"DetailedCommStatsTargetLocator", DetailedCommStatsTargetLocator},
    {"DetailedCommStatsPathFinder", DetailedCommStatsPathFinder},
    {"DetailedCommStatsTargetGrabber", DetailedCommStatsTargetGrabber},
    {"DetailedCommStatsTargetLocator", DetailedCommStatsTargetLocator},
    {"R1_Movement", R1_Movement},
    {"R1_Location", R1_Location},
    {"Forward", Forward},
    {"Back", Back},
    {"Left", Left},
    {"Right", Right},
    {"Start", Start},
    {"Stop", Stop},
    {"InitialData", InitialData},
    {"SensorData", SensorData},
    {"LineLocation", LineLocation},
    {"LineTuning", LineTuning},
    {"msLocalTime", msLocalTime}
};

static jsmn_parser p;
static jsmntok_t t[128]; /* We expect no more than 128 tokens */

static char buf[255];

int jsoneq(const char *json, jsmntok_t *tok, const char *s) ;

void initParser();

void parseJSON(const char* JSON_STRING, type_t *type, items_t items[], int *numItems, int *value, uint8_t *data, MotorObj *motorObj, LineProperties *lineProperties);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
