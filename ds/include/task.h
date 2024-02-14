#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h>
#include <time.h>
#include "uid.h"


typedef struct task task_t;
/*
struct task
{
    time_t execute_time;
    size_t interval_in_sec;
    ilrd_uid_t uid;
    int (*oper_func)(void *param);
    void *operation_func_param;
    void *clean_func_param;
    void (*clean_func)(void *param);
};*/

/*
*	creates a new task
*
*	Arguments: 
*		delay - initial time to run the task in seconds.
*		interval - a cyclist value for rerun the task.
*		oper_func - pointer to operational function. Must be NULL.
*		operation_func_param - The parameter that we wish to execute in operational function. Can be NULL.
*		clean_func - pointer to cleanup function. Must be valid.
*		clean_func_param - The parameter that we wish to execute in cleanup function. Can be NULL.	
*
*	Return: a new task pointer if successful
*				otherwise return NULL.
*
*	Time complexity: O(1) best/average/worst
*	Space complexity: O(1) best/average/worst
*/
task_t *TaskCreate(size_t delay,
			   	   size_t interval_in_sec,
			   	   int (*oper_func)(void *param),
				   void *operation_func_params,
				   void *clean_func_params,
				   void (*clean_func)(void *param));



/*
*	frees all memory allocated for the task
*
*	Arguments:
*		task - a task pointer. must be a valid address.
*
*	Return: void.
*
*	Time complexity: O(n) best/average/worst
*	Space complexity: O(1) best/average/worst
*/
void TaskDestroy(task_t *task);

/*
*	Execute task
*
*	Arguments:
*		task - a task pointer. must be a valid address.
*
*	 Return: 	
*		 	 0 - success
*			 1 - memory allocation error
*			 2 - function error
*
*	Time complexity: O(n) best/average/worst
*	Space complexity: O(1) best/average/worst
*/
int TaskRun(task_t *task);

/*
*    Checks if the given uid match to the given task.
*
*    Arguments:
*        uid - a valid Uniqe Identification
*		 task - a task pointer. must be a valid address.
*
*    Return: 1 if match, otherwise 0.
*
*    Time complexity: O(1) best/average/worst.
*    Space complexity: O(1) best/average/worst.
*/
int TaskIsMatch(ilrd_uid_t uid, const task_t *task);

/*
*    Update the execute time of the given task.
*
*    Arguments:
*		 		task - a task pointer. must be a valid address.
*
*    Return: None.
*
*    Time complexity: O(1) best/average/worst.
*    Space complexity: O(1) best/average/worst.
*/
void TaskUpdateTimeToRun(task_t *task);

/*
*    Getting the execute time of the given task.
*
*    Arguments:
*		 		task - a task pointer. must be a valid address.
*
*    Return: execute time.
*
*    Time complexity: O(1) best/average/worst.
*    Space complexity: O(1) best/average/worst.
*/
time_t TaskGetTimeToRun(const task_t *task);

/*
*    Getting the Uniqe Identification of the given task.
*
*    Arguments:
*		 		task - a task pointer. must be a valid address.
*
*    Return: uid.
*
*    Time complexity: O(1) best/average/worst.
*    Space complexity: O(1) best/average/worst.
*/
ilrd_uid_t TaskGetUID(const task_t *task);

/*
*    Checks if task1 is before the given task.
*
*    Arguments:
*        task1 - a task pointer. must be a valid address.
*		 task2 - a task pointer. must be a valid address.
*
*    Return: 1 if before, otherwise 0.
*
*    Time complexity: O(1) best/average/worst.
*    Space complexity: O(1) best/average/worst.
*/
int TaskIsBefore(const task_t *task1, const task_t *task2);

#endif   /* __TASK_H__ */



