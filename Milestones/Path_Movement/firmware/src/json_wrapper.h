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
int extractResponse_Vertices(float xArr[], float yArr[]);
int extractResponse_Obstacles(float arrX[10][4], float arrY[10][4]);
int extractResponse_Targets(float arrX[10][4], float arrY[10][4]);

bool extractResponse_targetAlignment(bool *g_align, float *distance, float *angle, bool *acquired);
  

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
