#ifndef __SORTED_LIST_H__
#define __SORTED_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

typedef struct sorted_list sorted_list_t;

/*
struct sorted_list
{
	dlist_t *dlist;
	sorted_list_cmp_t cmp;
};
*/



typedef struct sorted_list_iter
{
	dlist_iter_t internal_iter;
	
	#ifndef NDEBUG

	sorted_list_t *list;
	
	#endif /* NDEBUG */

} sorted_list_iter_t;


/*
*    Compare function between two datas
*
*    Arguments:
*        data1 - a data to compare
*        data2 - a data to compare
*
*    Return: integer less than, equal to, or greater than zero if data is found, respectively, to be less than, equal, or be greater than param.
*
*/
typedef int (*sorted_list_cmp_t)(const void *data1, const void *data2);

/*
*    match function between data and parm
*
*    Arguments:
*        data - the data to do compare
*        param - the param to do compare
*
*    Return: integer less than, equal to, or greater than zero if data is found, respectively, to be less than, to match, or be greater than param.
*
*/
/*
typedef int (*match_func_t)(const void *data, const void *param);
*/
/*
*    do operation
*
*    Arguments:
*        data - the data to apply upon
*        param - the parameter to the operation
*
*    Return: 0 if succeed, otherwise 1
*/
/*
typedef int (*action_func_t)(void *data, void *func_param);
*/
/*
*
*    Creates a new empty sorted linked list
*
*    Arguments:
*		func: the function determining the sorting criteria. can't be NULL.
*
*    Return: a pointer to the new sorted linked list if successful
*                otherwise return NULL.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
sorted_list_t *SortedListCreate(sorted_list_cmp_t func);

/*
*
*    Deallocate all memory allocated for a sorted linked list
*
*    Arguments:
*        list - Pointer to the sorted linked list to destroy. must be valid.
*
*    Return: void (Allwayes succeed).
*
*    Time complexity: O(n) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
void SortedListDestroy(sorted_list_t *list);

/*
*
*    Remove the first occurrence of the given data from the list.
*    Causes iterator invalidation.
*
*    Arguments:
*        iter - iterator to the data to remove from list. must be a valid iterator.
*
*    Return: the next valid iterator.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter);


/*
*
*    Insert data to the list. in case of failure the list remain unchanged.
*
*    Arguments:
*        list - Pointer to the sorted linked list to which we wish to insert. must be valid.
*        data - The data that we wish to insert to the list.
*
*    Return: iterator to the new data if successful or to the list's end upon fail
*
*    Time complexity: O(1) best O(n) average/worst
*    Space complexity: O(1) best/average/worst
*
*/
sorted_list_iter_t SortedListInsert(sorted_list_t *list, void *data_to_insert);

/*
*    Find data in range in a sorted linked list.
*
*    Arguments:
*		list - Pointer to the sorted linked list to which we wish to insert. must be valid.
*       from - iterator from where (including) to start the search. must be a valid iterator. must be in the same list as to.
*       to - iterator to where (excluding) to stop the search. must be a valid iterator. must be in the same list as from.
*       to_find - the data we wish to find.
*
*    Return: iterator to the found data. iterator to 'to' if not found
*
*    Time complexity: O(1) best O(n) average/worst.
*    Space complexity: O(1) best/average/worst.
*
*/
sorted_list_iter_t SortedListFind(sorted_list_t *list, sorted_list_iter_t from, sorted_list_iter_t to, const void *to_find);

/*
*    Find data that matches a specific criteria in a range in a sorted linked list.
*
*    Arguments:
*    	list - Pointer to the sorted linked list to which we wish to insert. must be valid.
*       from - iterator from where (including) to start the search. must be a valid iterator. must be in the same list as to.
*       to - iterator to where (excluding) to stop the search. must be a valid iterator. must be in the same list as from.
*       match_func - the criteria function. can't be NULL.
*       param - the parameter to be used by the criteria function. 
*
*    Return: iterator in the range between from and to that data compare to parm if there exist.
*                otherwise return to.
*
*    Time complexity: O(1) best O(n) average/worst.
*    Space complexity: O(1) best/average/worst.
*
*/
sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from, sorted_list_iter_t to, match_func_t func, const void *param);

/*
*    Get data from an iterator.
*
*    Arguments:
*        iter - iterator to get data from. must be a valid iterator.
*
*    Return: the data in iterator.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
void *SortedListGetData(sorted_list_iter_t cur);

/*
*
*    Removes a data from the begining of the list
*
*    Arguments:
*        list - Pointer to the sorted linked list from which we wish to pop. Must be valid and not empty.
*
*    Return: The data from the data removed
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
void *SortedListPopFront(sorted_list_t *list);

/*
*
*    Removes a data from the end of the list
*
*    Arguments:
*        list - Pointer to the sorted linked list to which we wish to insert. Must be valid and not empty.
*
*    Return: The data removed
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
void *SortedListPopBack(sorted_list_t *list);

/*
*
*    Return the number of data stored in a list.
*
*    Arguments:
*        list - Pointer to the sorted linked list. can't be NULL.
*
*    Return: number of elements in the list.
*
*    Time complexity: O(n) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
size_t SortedListSize(const sorted_list_t *list);

/*
*    Do an operation on each data in a range.
*
*    Arguments:
*        from - iterator to the start of the range (including). Must be a valid
*        to - iterator to the end of the range (excluding). Must be a valid
*        func - action to do for each data in iterator. must be a valid pointer
*        param - the param to do action to each data in iterator
*
*    Return: 0 upun success, the return value of the last call to func upon failure
*
*    Time complexity: O(1) best O(n) average/worst
*    Space complexity: O(1) best/average/worst
*
*/
int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to, action_func_t func, void *func_param);

/*
*    get an iterator to the first data in a list
*
*    Arguments:
*        list - pointer to the sorted linked list. can't be NULL.
*
*    Return: iterator to the first data in a list or to it's end if the list is empty
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
sorted_list_iter_t SortedListBegin(const sorted_list_t *list);

/*
*    get an iterator to the last data in a list
*
*    Arguments:
*        list - pointer to the sorted linked list. can't be NULL.
*
*    Return: iterator to the last data in a list or to END if the list is empty
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
sorted_list_iter_t SortedListEnd(const sorted_list_t *list);

/*
*
*    Get the iterator to the next data
*
*    Arguments:
*        cur - current iterator. Must be a valid iterator.
*
*    Return: iterator to the next data or to the end of the list.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
sorted_list_iter_t SortedListNext(sorted_list_iter_t cur);

/*
*
*    Get the iterator to the previous data
*
*    Arguments:
*        cur - current iterator. Must be a valid iterator that doesn't point to the first data.
*
*    Return: iterator to the previous data in the list
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
sorted_list_iter_t SortedListPrev(sorted_list_iter_t cur);

/*
*    Checking if the list is empty.
*
*    Arguments:
*        list - pointer to the sorted linked list. can't be NULL.
*
*    Return: 1 if the list is empty, otherwise 0
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
int SortedListIsEmpty(const sorted_list_t *list);

/*
*    cuts the data from src and inserts them to dest, while keeping dest sorted
*
*    Arguments:
*       dest - pointer to the list that will hold the merged list. can't be NULL.
*		src - pointer to the list that will be merged into dest. can't be NULL.
*
*    Return: void (always secceeds)
*
*    Time complexity: O(n + m) best/average/worst, when n and m are the sizes of sec and dest
*    Space complexity: O(1) best/average/worst
*
*/
void SortedListMerge(sorted_list_t *dest, sorted_list_t *src);

/*
*    Checks if two iterators point to the same element
*
*    Arguments:
*        iter1 - a valid iterator
*        iter2 - a valid iterator
*
*    Return: 1 if the iterators point at the same element, otherwise 0.
*
*    Time complexity: O(1) best/average/worst.
*    Space complexity: O(1) best/average/worst.
*
*/
int SortedListIsEqual(sorted_list_iter_t it1, sorted_list_iter_t it2);


#endif /* __SORTED_LIST_H__ */
