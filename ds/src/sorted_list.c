
#include <stdio.h> /*size_t*/
#include <assert.h> /*assert*/

#include "sorted_list.h"


/****************************************************************************************/

struct sorted_list
{
	dlist_t *dlist;
	sorted_list_cmp_t cmp;
};

/*==================================Static Functions====================================*/

static dlist_iter_t SortedToDListIter(sorted_list_iter_t iter)
{
	return iter.internal_iter;
}

/****************************************************************************************/
static sorted_list_iter_t DListToSortedIter_2(dlist_iter_t diter, sorted_list_iter_t siter)
{
	assert(NULL != diter);

	siter.internal_iter = diter;

	return siter;
}

/****************************************************************************************/

static sorted_list_iter_t DListToSortedIter(dlist_iter_t iter, sorted_list_t *list)
{
	sorted_list_iter_t sorted = {NULL};
	
	assert(NULL != list);
	
	sorted.internal_iter = iter;
	
	
	#ifndef NDEBUG
	
	sorted.list = list;
	
	#endif /*NDEBUG*/
	
	(void)list;
	
	return sorted;
}

/****************************************************************************************/

typedef struct insert
{
	sorted_list_cmp_t cmp;
	void *data_to_insert;
}insert_t;


/****************************************************************************************/

static int insert_func(const void *data1, const void *data2)
{
	sorted_list_cmp_t cmp = ((insert_t *)data2)->cmp;
	void *data_to_insert = ((insert_t *)data2)->data_to_insert;
	
	return (0 <= cmp(data1, data_to_insert));
}

/****************************************************************************************/
static int find_func(const void *data1, const void *data2)
{
	sorted_list_cmp_t cmp = ((insert_t *)data2)->cmp;
	void *data_to_insert = ((insert_t *)data2)->data_to_insert;
	
	return (0 == cmp(data1,data_to_insert));
}
	
/*==================================Global Functions====================================*/

sorted_list_t *SortedListCreate(sorted_list_cmp_t func)
{
	sorted_list_t *sorted_list = NULL;
	
	assert(NULL != func);
	
	sorted_list = malloc(sizeof(sorted_list_t));
	
	if (NULL == sorted_list)
	{
		return NULL;
	}
	
	sorted_list->dlist = DListCreate();
	if(NULL == sorted_list->dlist)
	{
		free(sorted_list);
		return NULL;
	}
	
	sorted_list->cmp = func;
	
	return sorted_list;
}

/****************************************************************************************/

void SortedListDestroy(sorted_list_t *list)
{
	assert(NULL != list);
	
	DListDestroy(list->dlist);
	free(list);
}

/****************************************************************************************/

sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter)
{
	dlist_iter_t new_iter = SortedToDListIter(iter);
	
	new_iter = DListRemove(new_iter);
	
	
	return DListToSortedIter_2(new_iter, iter);
	
}

	
/****************************************************************************************/

sorted_list_iter_t SortedListInsert(sorted_list_t *list, void *data_to_insert)
{
	dlist_iter_t start = NULL;
	dlist_iter_t end = NULL;
	dlist_iter_t where = NULL;
	
	insert_t insert_struct = {0};
	
	assert(NULL != list);
	assert(NULL != data_to_insert);
	
	insert_struct.cmp = list->cmp;
	insert_struct.data_to_insert = data_to_insert;
	
	start = DListGetBegin(list->dlist);
	end = DListGetEnd(list->dlist);
	
	where = DListFind(start, end, insert_func, &insert_struct);
	 
	where = DListInsert(where, data_to_insert);
	
	return DListToSortedIter(where, list);
	
}

/****************************************************************************************/

sorted_list_iter_t SortedListFind(sorted_list_t *list, sorted_list_iter_t from, sorted_list_iter_t to, const void *to_find)
{
	dlist_iter_t dfrom = NULL;
	dlist_iter_t dto = NULL;
	dlist_iter_t dfound = NULL;
	
	insert_t insert_struct = {0};
	
	assert(NULL != to_find);
	assert(from.list == to.list); /*make sure from and to are in the same list*/
	
	insert_struct.cmp = list->cmp;
	insert_struct.data_to_insert = (void *)to_find;
	
	dfrom = SortedToDListIter(from);
	dto = SortedToDListIter(to);
	
	dfound = DListFind(dfrom, dto, find_func, &insert_struct);
	
	return DListToSortedIter(dfound, list);
	
	
}

/****************************************************************************************/

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from, sorted_list_iter_t to, match_func_t func, const void *param)
{
	dlist_iter_t dfrom = NULL;
	dlist_iter_t dto = NULL;
	/*dlist_iter_t dfound= NULL;*/
	
	assert(NULL != func);
	assert(NULL != param);
	
	assert(from.list == to.list); /*make sure from and to are in the same list*/
	
	dfrom = SortedToDListIter(from);
	dto = SortedToDListIter(to);	
	
	from.internal_iter = DListFind(dfrom, dto, func, param);
	
	
	return from;
	
}	

/****************************************************************************************/

void *SortedListGetData(sorted_list_iter_t cur)
{
	dlist_iter_t iter = SortedToDListIter(cur);
	
	return DListGetData(iter);
}

/****************************************************************************************/


void *SortedListPopFront(sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListPopFront(list->dlist);
}

/****************************************************************************************/

void *SortedListPopBack(sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListPopEnd(list->dlist);
}

/****************************************************************************************/

size_t SortedListSize(const sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListCount(list->dlist);
}

/****************************************************************************************/

int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to, action_func_t func, void *func_param)
{
	dlist_iter_t dfrom = NULL;
	dlist_iter_t dto = NULL;
	
	assert(NULL != func);
	assert(NULL != func_param);
	
	assert(from.list == to.list); /*make sure from and to are in the same list*/
	
	dfrom = from.internal_iter;
	dto = to.internal_iter;
	
	return DListForEach(dfrom, dto, func, func_param);
	
}

/****************************************************************************************/

sorted_list_iter_t SortedListBegin(const sorted_list_t *list)
{
	dlist_iter_t diter = NULL;
	
	assert(NULL != list);
	
	diter = DListGetBegin(list->dlist);
	
	return DListToSortedIter(diter, (sorted_list_t *)list);
}

/****************************************************************************************/

sorted_list_iter_t SortedListEnd(const sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListToSortedIter(DListGetEnd(list->dlist), (sorted_list_t *)list);
}

/****************************************************************************************/

sorted_list_iter_t SortedListNext(sorted_list_iter_t cur)
{
	dlist_iter_t diter = SortedToDListIter(cur);
	
	diter = DListNext(diter);
	
	return DListToSortedIter_2(diter, cur);
}

/****************************************************************************************/

sorted_list_iter_t SortedListPrev(sorted_list_iter_t cur)
{
	
	dlist_iter_t diter = SortedToDListIter(cur);
	
	diter = DListPrev(diter);
	
	return DListToSortedIter_2(diter, cur);
}

/****************************************************************************************/

int SortedListIsEmpty(const sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListIsEmpty((list->dlist));
}

/****************************************************************************************/

void SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
	dlist_iter_t from_src = NULL;
	dlist_iter_t to_src = NULL;
	dlist_iter_t end_src = NULL;
	dlist_iter_t end_dest = NULL;
	
	dlist_iter_t where_dest = NULL;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	where_dest = DListGetBegin(dest->dlist);
	end_src = DListGetEnd(src->dlist);
	end_dest = DListGetEnd(dest->dlist);
	
	while (!SortedListIsEmpty(src))
	{ 
		from_src = DListGetBegin(src->dlist);
		to_src = DListNext(from_src);
		while ((!DListIsEqual(where_dest, end_dest )) && (0 <= (dest->cmp(DListGetData(where_dest), DListGetData(from_src)))))
		{
		
		where_dest = DListNext(where_dest);
			
		}
		
		if (DListIsEqual(where_dest, end_dest))
		{
			DListSplice(from_src, end_src, where_dest);
			return;
		}
		
	
		while ((!DListIsEqual(to_src, end_src)) && dest->cmp(DListGetData(to_src), DListGetData(where_dest)) > 0)
		{
				to_src = DListNext(to_src);	
		}
		
	DListSplice(from_src, to_src, where_dest);
	
	}	
		
}

/****************************************************************************************/

int SortedListIsEqual(sorted_list_iter_t it1, sorted_list_iter_t it2)
{
	
	return DListIsEqual(SortedToDListIter(it1), SortedToDListIter(it2));
}
	

