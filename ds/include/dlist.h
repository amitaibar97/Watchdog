#ifndef __D_LIST_H__
#define __D_LIST_H__

#include <stdio.h>/*size_t*/

typedef struct dlist_node *dlist_iter_t;
typedef struct dlist dlist_t;

/*
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
*/

/*
*    Compare function between data and parm.
*
*    Arguments:
*        data - the data to do compare.
*        param - the param to do compare.
*
*    Return: integer less than, equal to, or greater than zero if data is found, respectively, to be less than, to match, or be greater than param.
*
*/
typedef int (*match_func_t)(const void *data, const void *param);


/*
*    Do action to data.
*
*    Arguments:
*        data - the data to do action.
*        param - the param to do action to data.
*
*    Return: 0 if succeed, otherwise 1.
*/
typedef int (*action_func_t)(void *data, void *param);

/*
*
*    Creates a new doubly Linked List
*
*    Arguments:
*        void.
*
*    Return: a pointer to the new doubly Linked List if successful
*                otherwise return NULL.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_t *DListCreate(void);

/*
*
*    Frees all memory allocated for the Doubly Linked List
*
*    Arguments:
*        list - Pointer to the doubly Linked List to destroy. must be a valid address.
*
*    Return: void (Allwayes succeed).
*
*    Time complexity: O(n) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
void DListDestroy(dlist_t *list);

/*
*
*    Insert a data to the list, before the iterator where. in case of failure
*    the list remain unchanged.
*
*    Arguments:
*        where - iterator to insert date before.
*        data - The data that we wish to insert to the list.
*
*    Return: the new iterator that created if successful or end upon fail
*
*    Time complexity: O(1) best/average O(n) worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_iter_t DListInsert(dlist_iter_t where, void *data);

/*
*
*    Get the next iterator.
*
*    Arguments:
*        iter - current iterator. must be a valid iterator.
*
*    Return: the next valid iterator or end if it is the last iterator.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_iter_t DListNext(dlist_iter_t iter);


/*
*
*    Get the prev iterator.
*
*    Arguments:
*        iter - current iterator. must be a valid iterator.
*
*    Return: the prev valid iterator or end if it is the last iterator.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_iter_t DListPrev(dlist_iter_t iter);
/*
*
*    Remove data from the list.
*    causes iterator invalidation.
*
*    Arguments:
*        iter - iterator to remove from list. must be a valid iterator.
*
*    Return: the next iterator or end if it is the last valid iterator.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_iter_t DListRemove(dlist_iter_t iter);

/*
*
*    Get data from iterator.
*
*    Arguments:
*        iter - iterator to get data from. must be a valid iterator.
*
*    Return: the data in iterator on NULL if it is invalid iterator.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
void *DListGetData(dlist_iter_t iter);


/*
*
*    Count the number of data that is stored in the list.
*
*    Arguments:
*        list - Doubly Linked List that count data from. must be a valid list.
*
*    Return: how much data stored in list.
*
*    Time complexity: O(n) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
size_t DListCount(const dlist_t *list);

/*
*    get the first iterator in the list.
*
*    Arguments:
*        list - Doubly Linked List. must be a valid list.
*
*    Return: the first iterator or end if the list is empty.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_iter_t DListGetBegin(const dlist_t *list);

/*
*    Get the last iterator in the list.
*
*    Arguments:
*        list - Doubly Linked List. must be a valid list.
*
*    Return: the end in list, NOTICE: the last iterator is invalid iterator.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_iter_t DListGetEnd(const dlist_t *list);


/*
*    Find iterator in range.
*
*    Arguments:
*        from - iterator from where(including) start to search. must be a valid iterator.
*        to - iterator where(excluding) stop to search.
*        match_func - compare between data function. must be a valid pointer.
*        param - the param to compare between data.
*
*    Return: iterator in the range between from and to that data compare to parm if there exist.
*                otherwise return to.
*
*    Time complexity: O(1) best O(n) average/worst.
*    Space complexity: O(1) best/average/worst.
*
*/
dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, match_func_t match_func, const void *param);


/*
*    Do action for each iterator in the range.
*
*    Arguments:
*        from - iterator from where(including) start to call the function. must be a valid iterator.
*        to - iterator where(excluding) stop to call the function.
*        action_func - action to do for each data in iterator. must be a valid pointer.
*        param - the param to do action to each data in iterator.
*
*    Return: action func status.
*
*    Time complexity: O(1) best O(n) average/worst.
*    Space complexity: O(1) best/average/worst.
*
*/
int DListForEach(dlist_iter_t from, dlist_iter_t to, action_func_t action_func, void *param);

/*
*    Compare between two iterators.
*
*    Arguments:
*        left_iter - one iterator.
*        right_iter - other iterator.
*
*    Return: 1 if left_iter equal to right_iter otherwise 0.
*
*    Time complexity: O(1) best/average/worst.
*    Space complexity: O(1) best/average/worst.
*
*/
int DListIsEqual(dlist_iter_t left_iter, dlist_iter_t right_iter);


/*
*    Checking if the list is empty.
*
*    Arguments:
*        dlist - the doubly linked list.
*
*    Return: 1 if the list is empty otherwise 0.
*
*    Time complexity: O(1) best/average/worst.
*    Space complexity: O(1) best/average/worst.
*
*/
int DListIsEmpty(dlist_t *dlist);

/*
*
*    Insert data to the begining of the list. in case of failure
*    the list remain unchanged.
*
*    Arguments:
*        dlist - the doubly list.
*        data - The data that we wish to insert to the list.
*
*    Return: the new iterator that created if successful or end upon fail
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_iter_t DListPushFront(dlist_t *dlist, void *data);


/*
*
*    insert data to the end of the list. in case of failure
*    the list remain unchanged.
*
*    Arguments:
*        dlist - the doubly list.
*        data - The data that we wish to insert to the list.
*
*    Return: the new iterator that created if successful or end upon fail
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_iter_t DListPushEnd(dlist_t *dlist, void *data);


/*
*
*    Remove data from the begining of the list.
*
*    Arguments:
*        dlist - the doubly list, must be valid not empty.
*
*    Return: The data.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
void *DListPopFront(dlist_t *dlist);



/*
*
*    Remove data from the end of the list.
*
*    Arguments:
*        dlist - the double list, must be valid not empty.
*
*    Return: The data.
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
void *DListPopEnd(dlist_t *dlist);

/*
*
*    Move data between from and to, before where.
*
*    Arguments:
*        from - start of range (inclusive). must be a valid iterator.
*        to - end of range (exclusive) must be valid.
*        where - dest.
*
*    Return: where
*
*    Time complexity: O(1) best/average/worst
*    Space complexity: O(1) best/average/worst
*
*/
dlist_iter_t DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where);


/*
*
*    Find data that matches the match func and store in the output list.
*
*    Arguments:
*        from - iterator from where(including) start to call the function. must be a valid iterator.
*        to - iterator where(excluding) stop to call the function.
*        match_func - match the data in iterator. must be a valid pointer.
*        param - the param to do action to each data in iterator.
*        output_list - the output list, the list is must be valid, empty list.
*
*    Return: on success 0 , otherwise 1;
*
*    Time complexity: O(n) best/average/worst
*    Space complexity: O(n) best/average/worst
*
*/
int MultiFind(dlist_iter_t from, dlist_iter_t to, match_func_t match_func , const void *param, dlist_t *output_list);




#endif /*__D_LIST__*/
