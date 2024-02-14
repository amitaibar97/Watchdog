#include <stdlib.h>/*malloc*/
#include <stdio.h> /*size_t*/  
#include <assert.h> /*assert*/
#include "priority_queue.h"


enum status
{
	SUCCESS,
	FAILURE
};


struct priority_queue 
{
	sorted_list_t *priority_queue;
};

pq_t *PQCreate(pq_compare_t cmp)
{
	pq_t *pq = NULL;
	
	assert(NULL !=cmp);
	
	pq = (pq_t *)malloc(sizeof(pq_t));
	if (NULL == pq)
	{
		return NULL;
	}
	
	pq->priority_queue = SortedListCreate(cmp);
	if (NULL == pq->priority_queue)
	{
		free(pq);
		return NULL;
	}
	
	return pq;
	
}

void PQDestroy(pq_t *pq)
{
	assert(NULL != pq);
	
	SortedListDestroy(pq->priority_queue);
	pq->priority_queue = NULL;
	free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
	sorted_list_iter_t iter;
	
	assert(NULL != pq);
	
	iter = SortedListInsert(pq->priority_queue, data); 
	if(SortedListIsEqual(SortedListEnd(pq->priority_queue),iter))
	{
		return FAILURE;
	} 
	
	return SUCCESS;
}

void *PQDequeue(pq_t *pq)
{
	assert(NULL != pq);
	
	return SortedListPopBack(pq->priority_queue);
	
}

void *PQPeek(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SortedListGetData(SortedListPrev(SortedListEnd(pq->priority_queue)));
}

int PQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SortedListIsEmpty(pq->priority_queue);
}

size_t PQGetSize(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SortedListSize(pq->priority_queue);
}

void PQClear(pq_t *pq)
{
	assert(NULL != pq);
	
	while(!SortedListIsEmpty(pq->priority_queue))
	{
		SortedListPopBack(pq->priority_queue);
	}
}

void *PQErase(pq_t *pq, pq_is_match_t func, void *param)
{
	void *data = NULL;
	sorted_list_iter_t iter;
	
	assert(NULL != pq);
	assert(NULL != func);
	
	
	iter = SortedListFindIf(SortedListBegin(pq->priority_queue), SortedListEnd(pq->priority_queue), func, param);
	
	if (SortedListIsEqual(SortedListEnd(pq->priority_queue),iter))
	{
		return NULL;
	}
	data =  SortedListGetData(iter);
	
	SortedListRemove(iter);
	
	return data;
}



