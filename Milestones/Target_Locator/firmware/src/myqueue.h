#include <stddef.h>
#include <stdbool.h>
#include "message_controller_thread.h"

#ifndef MYQUEUE_H
#define MYQUEUE_H

#define MAX 50

typedef SensorDataType Queue_t;

typedef struct {
    Queue_t array[MAX];
    int front;
    int rear;
    int itemCount;
} Queue;

void initializeQueue(Queue* q);

Queue_t peek(Queue* q);

bool isEmpty(Queue* q);

bool isFull(Queue* q);

int size(Queue* q);

void insertData(Queue* q, Queue_t data);

Queue_t removeData(Queue* q);

void clearQueue(Queue* q);

#endif
