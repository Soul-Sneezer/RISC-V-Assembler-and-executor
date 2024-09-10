#include <stdio.h>
#include "heap.h"

static QueueElem* pqParent(PriorityQueue* q, int32_t n)
{
	if (n == 1)
		return NULL;

	return &q->elements[n / 2];
}

static QueueElem* pqLeftChild(PriorityQueue* q, int32_t n)
{
	return &q->elements[2 * n + 1];
}

static QueueElem* pqRightChild(PriorityQueue* q, int32_t n)
{
	return &q->elements[2 * n + 2];
}

static void pqSwap(PriorityQueue* q, QueueElem* e1, QueueElem* e2)
{

}

static void bubbleUp(PriorityQueue* q, QueueElem* p)
{
	if (pqParent(q, p->index) == NULL)
		return;

	if (pqParent(q, p->index)->value > p->value)
	{
		pqSwap(q, p, pqParent(q, p->index));
		bubbleUp(q, pqParent(q, p->index));
	}
}

void pqInsert(PriorityQueue *q, QueueElem e)
{
	if (q->size >= PQ_SIZE)
	{
		fprintf(stderr, " Warning: priority queue overflow! \n");
	}
	else
	{
		q->size = q->size + 1;
		q->elements[q->size] = e;
		bubbleUp(q, &q->elements[q->size]);
	}
}
