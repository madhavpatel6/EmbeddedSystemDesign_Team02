/*
 * In here we are going to define all the JSON objects
 * that we are going to send and recieve and the 
 * templates used to build them
 */


#ifndef JSON_OBJECTS_H
#define JSON_OBJECTS_H

#include "mjson.h"


static int count;
static double data;
static bool boolflag;

static const struct json_attr_t simp_obj[] = {
    {"count",   t_integer, .addr.integer = &count},
    {"data",   t_real, .addr.real = &data,},
    {"boolflag",   t_boolean, .addr.boolean = &boolflag,},
    {NULL},
};

static char* simp_obj_builder = "{\"data\":%f,\"boolflag\":%s,\"count\":%d}";




#endif