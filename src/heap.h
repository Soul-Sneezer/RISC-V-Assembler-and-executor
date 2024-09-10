#ifndef RISKV_HEAP
#define RISKV_HEAP

#include <stdbool.h>
#include <stdint.h>

#define PQ_SIZE 1000

typedef struct
{
	bool valid_index;
	uint32_t value;
	int32_t index;
} QueueElem;


typedef struct
{
	QueueElem elements[PQ_SIZE+1];
	int32_t size;
} PriorityQueue;

void pqInsert(PriorityQueue *q, QueueElem e);
void pqInit(PriorityQueue* q);
void makeHeap(PriorityQueue* q, QueueElem e[], int32_t n);

#endif
