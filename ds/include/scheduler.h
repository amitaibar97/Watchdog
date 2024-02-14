#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stddef.h>
#include <time.h>
#include "uid.h"
#include "priority_queue.h"

typedef enum operation_status 
{
  OP_DONE,
  OP_CONTINUE,
  OP_REMOVED,
  OP_ERROR
} op_status_t;

typedef enum sched_status
{
    SUCCESS,
    MEMORY_ERR, /* Returned by SchedRun when memory allocation failed */
    FUNC_ERR, /* Returned by SchedRun when a task's operation failed */
    STOPPED, /* Returned by SchedRun when stopped explicitly */
    NO_MORE_TASKS, /* Returned by SchedRun when there are no more tasks to preform */
    NOT_FOUND /* Returned by SchedRemove when task was not found */
} sched_status_t;

typedef struct scheduler sched_t;

/*
struct scheduler
{
	pq_t *pq;
	int to_stop; 
	task_t *current_task;
	int to_remove_current;
};
*/

/*
*	creates a new Scheduler
*
*	Arguments: None		
*
*	Return: a new Scheduler pointer if successful
*				otherwise return NULL.
*
*	Time complexity: O(1) best/average/worst
*	Space complexity: O(1) best/average/worst
*/
sched_t *SchedCreate(void);

/*
*	frees all memory allocated for the Scheduler
*
*	Arguments:
*		sched - a Scheduler pointer. must be a valid address.
*
*	Return: void.
*
*	Time complexity: O(n) best/average/worst
*	Space complexity: O(1) best/average/worst
*/
void SchedDestroy(sched_t *sched);

/*
*	Add a Task to the Scheduler, sorted by priority.
	
*
*	Arguments:
*		delay - initial time to run the task in seconds.
*		interval - a cyclist value for rerun the task.
*		oper_func - pointer to operational function. Must be valid.
*		operation_func_param - The parameter that we wish to execute in operational function. Can be NULL.
*		clean_func - pointer to cleanup function. Must be valid.
*		clean_func_param - The parameter that we wish to execute in cleanup function. Can be NULL.

*	Return: UID - success,
*			NULL- failure.
*
*	Time complexity: O(1) best, O(n) - average/worst
*	Space complexity: O(1) best/average/worst
*/
ilrd_uid_t SchedAdd(sched_t *sched,
			   size_t delay,
			   size_t interval_in_sec,
			   int (*oper_func)(void *param),
			   void *operation_func_params,
			   void *clean_func_params,
			   void (*clean_func)(void *param));


/*
*    Remove the match task of the given scheduale.
*
*    Arguments:
*		sched - a Scheduler pointer. must be a valid address.
*		uid - Uniqe Identification of the task.
*
*	 Return: 0 - success,
*			1- failure.
*
*    Time complexity: O(n) best/average/worst
*    Space complexity: O(1) best/average/worst
*/
int SchedRemove(sched_t *sched, ilrd_uid_t uid);

/*
*    Start to execute the tasks.
*
*    Arguments:
*		sched - a Scheduler pointer. must be a valid address.
*
*	 Return: 	
*		 	 0 - success
*			 1 - memory allocation error
*			 2 - function error
			 3 - stopped		
*			
*    Time complexity: O(n) best/average/worst
*    Space complexity: O(1) best/average/worst
*/
int SchedRun(sched_t *sched);

/*
*    Stop to execute tasks.
*
*    Arguments:
*		sched - a Scheduler pointer. must be a valid address.
*			
*    Time complexity: O(n) best/average/worst
*    Space complexity: O(1) best/average/worst
*/
void SchedStop(sched_t *sched);

/*
*	Return the number of current tasks in the Scheduler.
*
*	Arguments:
*		sched - a Scheduler pointer. must be a valid address.
*
*	Return: number of tasks in the Scheduler.
*
*	Time complexity: O(n) best/average/worst
*	Space complexity: O(1) best/average/worst
*/
size_t SchedSize(const sched_t *sched);

/*
*	Clears Scheduler from tasks. Scheduler remains valid but empty.
*
*	Arguments:
*		sched - a Scheduler pointer. must be a valid address.
*
*	Return: void.
*
*	Time complexity: O(n) best/average/worst
*	Space complexity: O(1) best/average/worst
*/
void SchedClear(sched_t *sched);

/*
*	Checks if the Scheduler is empty
*
*	Arguments:
*		sched - a Scheduler pointer. must be a valid address.
*
*	Return: 1 - if the Scheduler is empty, 0 - otherwise
*
*	Time complexity: O(1) best/average/worst
*	Space complexity: O(1) best/average/worst
*/
int SchedIsEmpty(const sched_t *sched);


#endif   /* __SCHEDULER_H__ */


