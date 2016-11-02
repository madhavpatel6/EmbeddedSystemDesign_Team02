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


void parseJSON(const char* JSON_STRING, type_t *type, items_t items[], int *numItems){
    int i;
    strcpy(stored_js_str, JSON_STRING);
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
        }else if (jsoneq(JSON_STRING, &t[i], "items") == 0) {
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
                        k++;
                        break;
                    }
                }
                *numItems = k;
            }
            i += t[i+1].size + 1;
        }
    }
}
bool extractResponse_Vertices(float xArr[], float yArr[]){
    int i;
    bool result = false;
    for(i = 1; i < r; i++){
        if (jsoneq(stored_js_str, &t[i], "Vertices") == 0) {
            int j;
            if (t[i+1].type == JSMN_ARRAY) {
                // lets loop thru the set of vertices
                int j;
                int numVert = t[i+1].size;
                for(j = 0; j < numVert; j++){
                    sprintf(buf, "%.*s\0", t[i+1 + (j*3 + 2)].end-t[i+1 + (j*3 + 2)].start, stored_js_str + t[i+1 + (j*3 + 2)].start);
                    xArr[j] = atof(buf);
                    sprintf(buf, "%.*s\0", t[i+1 + (j*3 + 3)].end-t[i+1 + (j*3 + 3)].start, stored_js_str + t[i+1 + (j*3 + 3)].start);
                    yArr[j] = atof(buf);
                }
                result = true;
            }
        }
    }
    return result;
}
bool extractResponse_Obstacles(float ***obs){
    int i;
    bool result = false;
    return result;
    
}
void extractResponse_Targets(float *result){
    
}
void extractResponse_SafeRegions(float *result){
    
}
void extractResponse_R2_Location(float x, float y, float orientation){
    
}



/* *****************************************************************************
 End of File
 */
