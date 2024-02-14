#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h>

typedef struct queue queue_t;
 
 /*

struct queue
{
	slist_t *queue
};
*/


/*
*
*	creates a new Queue
*
*	Arguments:
*		void.
*
*	Return: a new Queue if successful
*				otherwise return NULL.
*
*	Time complexity: O(1) best/average/worst
*	Space complexity: O(1) best/average/worst
*
*/
queue_t *QCreate(void);

/*
*
*	frees all memory allocated for the Queue
*
*	Arguments:
*		list - Queue to destroy. must be a valid address.
*
*	Return: void.
*
*	Time complexity: O(n) best/average/worst
*	Space complexity: O(1) best/average/worst
*
*/
void QDestroy(queue_t *queue);

/*
*
*	add a value to the end of the Queue
*
*	Arguments:
*		queue - pointer to the queue to change. must be vaild.
*		data - The value that we wish to add to the end of the Queue.
*
*	Return: 0 - success,
*			1- failure.
*
*	Time complexity: O(1) best/average, O(n) - worst
*	Space complexity: O(1) best/average/worst
*
*/
int QEnqueue(queue_t *queue, void *data);

/*
*
*	remove a value from the front of the Queue
*
*	Arguments:
*		queue - pointer to the queue to change. must be vaild.
*
*	Return: void (always success).
*
*	Time complexity: O(1) best/average/worst
*	Space complexity: O(1) best/average/worst
*
*/
void QDequeue(queue_t *queue);

/*
*
*	return the value at the front of the Queue.
*
*	Arguments:
*		Queue - pointer to the queue to check. must be vaild.
*
*	Return: A pointer to the value at the front of the Queue.
*
*	Time complexity: O(1) best/average/worst
*	Space complexity: O(1) best/average/worst
*
*/
void *QPeek(queue_t *queue);

/*
*
*	Checks if the Queue is empty
*
*	Arguments:
*		queue - pointer to the queue to check. must be vaild.
*
*	Return: 1 - if the queue is empty, 0 - otherwise
*
*	Time complexity: O(1) best/average/worst
*	Space complexity: O(1) best/average/worst
*
*/
int QIsEmpty(queue_t *queue);

/*
*
*	Return the number of current values in the Queue 
*
*	Arguments:
*		queue - pointer to the queue to check. must be vaild.
*
*	Return: number of values in the queue 
*
*	Time complexity: O(n) best/average/worst
*	Space complexity: O(1) best/average/worst
*
*/
 size_t QGetSize(queue_t *queue);

/*
*
*	Append src to dest.
*
*	Arguments:
*		dest - Queue to which the function append src. must be vaild.
*		src - Queue to append to dest. must be vaild.
*
*	Return: void.
*
*	Time complexity: O(1) best/average/worst
*	Space complexity: O(1) best/average/worst
*
*/
void QAppend(queue_t *dest, queue_t *src);





#endif /* __QUEUE_H__ */




