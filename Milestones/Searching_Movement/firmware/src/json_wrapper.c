/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include "json_wrapper.h"

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

void initParser(){
    jsmn_init(&p);
}


void parseJSON(const char* JSON_STRING, type_t *type, items_t items[], int *numItems, int *value, char *mode, uint8_t *data){
    int r, i;
    r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
    for(i = 1; i < r; i++){
        if (jsoneq(JSON_STRING, &t[i], "type") == 0) {
            /* We may use strndup() to fetch string value */
            sprintf(buf, "%.*s\0", t[i+1].end-t[i+1].start,
                    JSON_STRING + t[i+1].start);
            i++;
            if(strcmp(buf, "Request") == 0){
                *type = request;
            }else if(strcmp(buf, "Response") == 0){
                *type = response;
            }else{
                *type = unknown;
            }
        } else if (jsoneq(JSON_STRING, &t[i], "items") == 0) {
            int j;
            int k = 0;
            if (t[i+1].type != JSMN_ARRAY) {
                continue; /* We expect items to be an array of strings */
            }
            for (j = 0; j < t[i+1].size; j++) {
                jsmntok_t *g = &t[i+j+2];
                sprintf(buf, "%.*s\0", g->end - g->start, JSON_STRING + g->start);
                int l = 0;
                for (l = 0; (l < (sizeof(Dictionary) / sizeof(*Dictionary))); l++) {
                    if (strcmp(buf, Dictionary[l].stringValue) == 0) {
                        items[k] = Dictionary[l].enumValue;
                        if (strcmp(buf, "Forward") == 0 ||
                                strcmp(buf, "Back") == 0 ||
                                strcmp(buf, "Left") == 0 ||
                                strcmp(buf, "Right") == 0) {
                            sprintf(buf, "%.*s\0", t[i+j+3].end - t[i+j+3].start, JSON_STRING + t[i+j+3].start);
                            *value = atof(buf);
                        }
                        k++;
                        break;
                    }
                }
                *numItems = k;
            }
            i += t[i+1].size + 1;
        } else if (jsoneq(JSON_STRING, &t[i], "InitialData") == 0) {
            int l = 0;
            int k = 0;
            for (l = 0; (l < (sizeof(Dictionary) / sizeof(*Dictionary))); l++) {
                if (strcmp("InitialData", Dictionary[l].stringValue) == 0) {
                    items[k] = Dictionary[l].enumValue;
                    sprintf(buf, "%.*s\0", t[i+3].end - t[i+3].start, JSON_STRING + t[i+3].start);
                    *mode = buf[0];
                    k++;
                    break;
                }
            }
            *numItems = k;
        } else if (jsoneq(JSON_STRING, &t[i], "SensorData") == 0) {
            int j = 0;
            int l = 0;
            int k = 0;
            if (t[i+1].type != JSMN_ARRAY) {
                continue; /* We expect items to be an array of strings */
            }
            for (j = 0; j < t[i+1].size; j++) {
                jsmntok_t *g = &t[i+j+2];
                sprintf(buf, "%.*s\0", g->end - g->start, JSON_STRING + g->start);
                if (strcmp(buf, "0") == 0) {
                    *data = 0;
                } else {
                    *data = 1;
                    break;
                }
            }
            for (l = 0; (l < (sizeof(Dictionary) / sizeof(*Dictionary))); l++) {
                if (strcmp("SensorData", Dictionary[l].stringValue) == 0) {
                    items[k] = Dictionary[l].enumValue;
                    k++;
                    break;
                }
            }
            *numItems = k;
        }
    }
}


/* *****************************************************************************
 End of File
 */
