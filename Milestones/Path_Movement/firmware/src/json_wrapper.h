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
#include "communication/messages.h"

// this one is cuz stephen loves outputing to uart
#include "tx_thread.h"

typedef enum {request, response, unknown} type_t;
typedef enum {CommStatsSearcherMover, CommStatsTargetLocator, CommStatsPathFinder, CommStatsTargetGrabber,
       DetailedCommStatsSearcherMover, DetailedCommStatsTargetLocator, DetailedCommStatsPathFinder,
       DetailedCommStatsTargetGrabber, R1_Est_Location, R1_Est_Orientation, Forward, Back, Left, 
       Right, SensorData, msLocalTime} items_t;

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
    {"R1_Est_Location", R1_Est_Location},
    {"R1_Est_Orientation", R1_Est_Orientation},
    {"Forward", Forward},
    {"Back", Back},
    {"Left", Left},
    {"Right", Right},
    {"SensorData", SensorData},
    {"msLocalTime", msLocalTime}
};

static jsmn_parser p;
static jsmntok_t t[128]; /* We expect no more than 128 tokens */
static int r;
static char stored_js_str[MAXMESSAGESIZE];

static char buf[255];

int jsoneq(const char *json, jsmntok_t *tok, const char *s) ;

void initParser();

void parseJSON(const char* JSON_STRING, type_t *type, items_t items[], int *numItems);

/* precondition for all these parseJSON must already be called and t filled*/
bool extractResponse_Vertices(float xArr[], float yArr[]);
bool extractResponse_Obstacles(float ***result);
void extractResponse_Targets(float *result);
void extractResponse_SafeRegions(float *result);
void extractResponse_R2_Location(float x, float y, float orientation);
  

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
