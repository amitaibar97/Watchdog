
#include <assert.h> /*assert*/
#include <stdlib.h>/*malloc*/
#include <stdio.h>/*size_t*/
#include "dlist.h"



/**********************************************************************************************/

typedef struct dlist_node
{
    void *data;
    struct dlist_node *next;
    struct dlist_node *prev;
}dlist_node_t;

struct dlist
{
    dlist_node_t head;
    dlist_node_t tail;
};


/**********************************************************************************************/
static dlist_iter_t DListGetEndByIter(dlist_iter_t iter)
{
	assert (NULL != iter);
	while (NULL != iter->next)
	{
		iter = iter->next;
	}
	
	return iter;
}

/**********************************************************************************************/
dlist_t *DListCreate(void)
{
	dlist_t *list = malloc(sizeof(dlist_t));
	if (NULL == list)
	{
		return NULL;
	}
	
	list->head.prev = NULL;
	list->head.data = NULL;
	list->head.next = &list->tail;
	
	list->tail.prev = &list->head;
	list->tail.data = NULL;
	list->tail.next = NULL;
	
	return list;
}

/**********************************************************************************************/

void DListDestroy(dlist_t *list)
{
	dlist_node_t *current_node = NULL;
	dlist_node_t *temp = NULL;
	assert(NULL != list);
	current_node = list->head.next;
	
	while (&list->tail != current_node)
	{
		temp = current_node;
		current_node = current_node->next;
		free(temp);
	}
	
	free(list);
}

/**********************************************************************************************/

dlist_iter_t DListInsert(dlist_iter_t where, void *data)
{
	dlist_node_t *new_node = NULL;
	
	assert(NULL != where);
	assert(NULL != data);
	
	new_node = malloc(sizeof(dlist_node_t));
	
	if (NULL == new_node) /*checks if malloc failed*/ 
	{
		return DListGetEndByIter(where); /*return end of list*/
	}
	
	where->prev->next = new_node;
	new_node->prev = where->prev;
	
	new_node->data = data;
	new_node->next = where;
	
	where->prev = new_node;
	
	return new_node;
	
}

/**********************************************************************************************/

size_t DListCount(const dlist_t *list)
{
	size_t counter = 0;
	dlist_node_t *runner = NULL;
	assert(NULL != list);
	
	runner = list->head.next;
	
	while (&list->tail != runner)
	{
		runner = runner->next;
		++counter;
	}
	
	return counter;
}

/**********************************************************************************************/
dlist_iter_t DListNext(dlist_iter_t iter)
{	
	assert(NULL != iter);
	return iter->next;
}

/**********************************************************************************************/

dlist_iter_t DListPrev(dlist_iter_t iter)
{
	assert(NULL != iter);
	return iter->prev;
}

/**********************************************************************************************/

dlist_iter_t DListRemove(dlist_iter_t iter)
{
	dlist_iter_t keeper = NULL;
	assert(NULL != iter);
	
	keeper = iter->next;
	
	if (iter->next == NULL) 
	{
		return iter;
	}
	
	iter->prev->next = iter->next;
	iter->next->prev = iter->prev;
	
	free(iter);
	
	return keeper;
	
}

/**********************************************************************************************/

dlist_iter_t DListGetBegin(const dlist_t *list)
{
	assert(NULL != list);
	
	return (list->head.next);
}

/**********************************************************************************************/

dlist_iter_t DListGetEnd(const dlist_t *list)
{
	assert(NULL != list);
	
	return (list->tail.prev->next);
}

/**********************************************************************************************/

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, match_func_t match_func, const void *param)
{
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	
	while (from != to)
	{
		if (1 == match_func(from->data, param)) /*match found*/
		{
			return from;
		}
		
		from = from->next;
	}
	
	return to;
}

/**********************************************************************************************/
int DListForEach(dlist_iter_t from, dlist_iter_t to, action_func_t action_func, void *param)
{
	assert(NULL != from);
	assert(NULL != to);
	
	
	while (from != to)
	{
		if (1 == action_func(from->data, param)) /*action failed*/
		{
			return 1;
		}
		
		from = from->next;
	}
	
	return 0;
}

/**********************************************************************************************/
int DListIsEmpty(dlist_t *list)
{
	assert(NULL != list);
	
	return (list->head.next == &list->tail);
}

/**********************************************************************************************/

int DListIsEqual(dlist_iter_t left_iter, dlist_iter_t right_iter)
{
	assert(NULL != left_iter);
	assert(NULL != right_iter);
	
	return left_iter == right_iter;
}

/**********************************************************************************************/

void *DListGetData(dlist_iter_t iter)
{
	assert(NULL != iter);
	return iter->data;
}

/**********************************************************************************************/


dlist_iter_t DListPushFront(dlist_t *dlist, void *data)
{
	assert(NULL != dlist);
	assert(NULL != data);
	return DListInsert(dlist->head.next, data);
	
}

/**********************************************************************************************/

dlist_iter_t DListPushEnd(dlist_t *dlist, void *data)
{
	assert(NULL != dlist);
	return DListInsert(&(dlist->tail), data);
}

/**********************************************************************************************/

void *DListPopFront(dlist_t *dlist)
{
	void *data = NULL;
	assert (NULL != dlist);
	data = DListGetBegin(dlist)->data;
	DListRemove(DListGetBegin(dlist));
	
	return data;
}


/**********************************************************************************************/

void *DListPopEnd(dlist_t *dlist)
{
	void *data = NULL;
	dlist_iter_t keep_end = NULL;
	
	assert (NULL != dlist);
	keep_end = DListGetEnd(dlist)->prev;
	data = keep_end->data;
	DListRemove(keep_end);
	
	return data;
}

/**********************************************************************************************/

dlist_iter_t DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
	assert(NULL != from);
	assert(NULL != to);
	
	where->prev->next = from;
	from->prev->next = to->next;
	
	to->next = where;
	where->prev = to;
	
	
	return where;
}

/**********************************************************************************************/

int MultiFind(dlist_iter_t from, dlist_iter_t to, match_func_t match_func , const void *param, dlist_t *output_list)
{
	assert(NULL != output_list);
	assert(NULL != from);
	assert(NULL != to);
	
	while (!(DListIsEqual(from, to)))
	{
		if (0 == match_func(from->data, param))
		{
			if (NULL == DListPushEnd(output_list, from->data))
			{
				return 1;
			}
		}
		
		from = from->next;
	}
	
	return 0;
}
