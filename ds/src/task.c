#include <assert.h>/*assert*/
#include <time.h> /*time*/
#include <stdlib.h>

#include "task.h"
#include "uid.h"

#define SUCCESS 0

struct task
{
    time_t execute_time;
    size_t interval_in_sec;
    ilrd_uid_t uid;
    int (*oper_func)(void *param);
    void *operation_func_param;
    void *clean_func_param;
    void (*clean_func)(void *param);
};


task_t *TaskCreate(size_t delay,
			   	   size_t interval_in_sec,
			   	   int (*oper_func)(void *param),
				   void *operation_func_params,
				   void *clean_func_params,
				   void (*clean_func)(void *param))
{
	task_t *task = (task_t *)malloc(sizeof(task_t));
	if (NULL == task)
	{
		return NULL;
	}
	
	task->uid = UIDCreate();
	if (UIDIsSame(UIDBadUID, task->uid))
	{
		free(task);
		return NULL;
	}
	
	task->execute_time = time(NULL)+ delay;
	task->interval_in_sec = interval_in_sec;
	task->oper_func = oper_func;
	task->operation_func_param = operation_func_params;
	task->clean_func = clean_func;
	task->clean_func_param = clean_func_params;
	
	return task;
}
				   
			   
void TaskDestroy(task_t *task)
{
	assert(NULL != task);
	
	free(task);
}


int TaskRun(task_t *task)
{
	int status = SUCCESS;
	
	assert(NULL != task);
	
	status = task->oper_func(task->operation_func_param);
	
	task->clean_func(task->clean_func_param);
	
	
	return status;
}


int TaskIsMatch(ilrd_uid_t uid, const task_t *task)
{
	assert(NULL != task);
	
	return UIDIsSame(uid, task->uid);
}

void TaskUpdateTimeToRun(task_t *task)
{
	assert(NULL != task);
	
	task->execute_time = time(NULL) + task->interval_in_sec;
}

time_t TaskGetTimeToRun(const task_t *task)
{
	assert(NULL != task);
	
	return task->execute_time;
}

ilrd_uid_t TaskGetUID(const task_t *task)
{
	assert(NULL != task);
	
	return task->uid;
}

int TaskIsBefore(const task_t *task1, const task_t *task2)
{
	assert(NULL != task1);
	assert(NULL != task2);
	
	return (0 > difftime(task1->execute_time, task2->execute_time));
	
}


