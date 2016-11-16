#include "myqueue.h"

void initializeQueue(Queue* q) {
    q->front = 0;
    q->rear = -1;
    q->itemCount = 0;
}
Queue_t peek(Queue* q) {
   return q->array[q->front];
}

bool isEmpty(Queue* q) {
   return q->itemCount == 0;
}

bool isFull(Queue* q) {
   return q->itemCount == MAX;
}

int size(Queue* q) {
   return q->itemCount;
}  

void insertData(Queue* q, Queue_t data) {

   if(!isFull(q)) {
	
      if(q->rear == MAX-1) {
         q->rear = -1;            
      }       
      q->rear++;
      q->array[q->rear] = data;
      q->itemCount++;
   }
}

 Queue_t removeData(Queue* q) {
   Queue_t data = q->array[(q->front)];
   q->front += 1;
	
   if(q->front == MAX) {
      q->front = 0;
   }
	
   q->itemCount--;
   return data;  
}

void clearQueue(Queue* q) {
    q->front = 0;
    q->itemCount = 0;
}