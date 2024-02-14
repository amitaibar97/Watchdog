
#include "linked_list.h"
#include "queue.h"
#include <stdio.h> 
#include <stdlib.h>
#include <assert.h> /*assert*/


struct queue
{
	slist_t *queue;
	
};


queue_t *QCreate(void);

void QDestroy(queue_t *queue);

int QEnqueue(queue_t *queue, void *data);

void QDequeue(queue_t *queue);

void *QPeek(queue_t *queue);

int QIsEmpty(queue_t *queue);

size_t QGetSize(queue_t *queue);

void QAppend(queue_t *dest, queue_t *src);



/***********************************************************************************/
queue_t *QCreate(void)
{
	queue_t *queue = malloc(sizeof(queue_t));
	assert(NULL != queue);
	
	queue->queue = SListCreate();
	return queue;
}


/***********************************************************************************/

void QDestroy(queue_t *queue)
{
	assert(NULL != queue);
	
	SListDestroy(queue->queue);
	free(queue);
}

/***********************************************************************************/

int QEnqueue(queue_t *queue, void *data)
{
	slist_iter_t iter = NULL;
	assert(NULL != queue);
	iter = SListInsert(SListGetEnd(queue->queue), data);
	if (SListIsEqual(SListGetEnd(queue->queue), iter))
	{
		return 1;
	}
	
	return 0; 
	
}

/***********************************************************************************/

void QDequeue(queue_t *queue)
{
	assert(NULL != queue);
	
	SListRemove(SListGetBegin(queue->queue));
}

/***********************************************************************************/

void *QPeek(queue_t *queue)
{
	assert(NULL != queue);
	
	return SListGetData(SListGetBegin(queue->queue));
}

/***********************************************************************************/

int QIsEmpty(queue_t *queue)
{
	assert(NULL != queue);
	
	if (0 == (int)(SListCount(queue->queue))) /*cast?*/
	{
		return 1;
	}
	
	return 0;
}

/***********************************************************************************/

size_t QGetSize(queue_t *queue)
{
	assert(NULL != queue);
	return SListCount(queue->queue);
}

/***********************************************************************************/

void QAppend(queue_t *dest, queue_t *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	
	SListAppend(dest->queue, src->queue);
}

/***********************************************************************************/




