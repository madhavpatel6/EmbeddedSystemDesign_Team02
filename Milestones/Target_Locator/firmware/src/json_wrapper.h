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
#include "sensor_thread.h"

typedef enum {request, response, unknown} type_t;
typedef enum {CommStatsSearcherMover, CommStatsTargetLocator, CommStatsPathFinder, CommStatsTargetGrabber,
       DetailedCommStatsSearcherMover, DetailedCommStatsTargetLocator, DetailedCommStatsPathFinder,
       DetailedCommStatsTargetGrabber, TargetLocatorSensorData, msLocalTime, OccupancyGrid, LocationInformation, SensorData, InterpretGrid, StartResponding, R1_Movement,
        Targets} items_t;

typedef struct {
  char stringValue[512];
  items_t enumValue;
} DictionaryType;

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
    {"TargetLocatorSensorData", TargetLocatorSensorData},
    {"msLocalTime", msLocalTime},
    {"OccupancyGrid", OccupancyGrid},
    {"LocationInformation", LocationInformation},
    {"SensorData", SensorData},
    {"InterpretGrid", InterpretGrid},
    {"StartResponding", StartResponding},
    {"Targets", Targets},
    {"R1_Movement", R1_Movement},
};

static jsmn_parser p;
static jsmntok_t t[128]; /* We expect no more than 128 tokens */

static char buf[255];

int jsoneq(const char *json, jsmntok_t *tok, const char *s) ;

void initParser();

void parseJSON(const char* JSON_STRING, type_t *type, items_t items[], int *numItems, Movement_t* r1_movement, int *row);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
