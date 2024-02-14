#include <stdlib.h> /*free*/
#include <time.h> /*timediff*/
#include <assert.h> /*assert*/
#include <unistd.h>/*sleep*/
#include "task.h"
#include "scheduler.h"


static int sorter(const void *data_1, const void *data_2)
{
	return difftime(TaskGetTimeToRun((task_t *)data_2),TaskGetTimeToRun((task_t *)data_1));
}

static int is_match(const void *data, const void *param)
{
	return UIDIsSame(TaskGetUID(data), *(ilrd_uid_t *)param);
}

struct scheduler
{
	pq_t *pq;
	int to_stop; 
	task_t *current_task;
	int to_remove_current;
};


sched_t *SchedCreate(void)
{
	sched_t *scheduler = (sched_t *)malloc(sizeof(sched_t));
	if (NULL == scheduler)
	{
		return NULL;
	}
	
	scheduler->pq = PQCreate(sorter); 
	if (NULL == scheduler->pq)
	{
		free(scheduler);
		return NULL;
	}
	
	scheduler->to_stop = 0;
	scheduler->current_task = NULL;
	scheduler->to_remove_current = 0;
	
	return scheduler;
	
}

void SchedDestroy(sched_t *sched)
{
	assert(NULL != sched);
	
	SchedClear(sched);
		
	PQDestroy(sched->pq);
	
	sched->pq = NULL;
	
	free(sched);
	
}

ilrd_uid_t SchedAdd(sched_t *sched,
			   size_t delay,
			   size_t interval_in_sec,
			   int (*oper_func)(void *param),
			   void *operation_func_params,
			   void *clean_func_params,
			   void (*clean_func)(void *param))
{
	task_t *task = NULL;
	
	assert(NULL != sched);
	assert(NULL != oper_func);
	assert(NULL != clean_func);
	
	task = TaskCreate(delay, interval_in_sec, oper_func, operation_func_params,clean_func_params, clean_func);  
	if (NULL == task)
	{
		return UIDBadUID;
	}
	
	if (SUCCESS != PQEnqueue(sched->pq, task))
	{
		free(task);
		return UIDBadUID;
	}
			
	return TaskGetUID(task);
}

int SchedRun(sched_t *sched)
{
	/* int task_status = -10;
	task_t *cur_task = NULL;
	
	assert(NULL != sched);
	
	while(!PQIsEmpty(sched->pq) && 0 == sched->to_stop)
	{
		cur_task = PQDequeue(sched->pq);
		sched->current_task = cur_task;
		
		while(time(NULL) < TaskGetTimeToRun(cur_task))
		{
			sleep(1);
		}
		
		task_status = TaskRun(cur_task);
		
		switch(task_status)
		{
			case OP_ERROR:
			case OP_DONE:
								
					TaskDestroy(cur_task);
					sched->to_remove_current = 0;
					break;	

			case OP_CONTINUE:					
					TaskUpdateTimeToRun(cur_task);
					if(1 == PQEnqueue(sched->pq, cur_task))
					{
						TaskDestroy(cur_task);
						sched->current_task = NULL;
						return(MEMORY_ERR);
					}
					break;

		}

		sched->current_task = NULL;
		
		if(1 == sched->to_stop)
		{
			sched->to_stop = 0;
			return STOPPED;
		}
	}
	
	return NO_MORE_TASKS;
} */
	time_t diff = 0;
	int answer = 0; 
	time_t time_to_run = 0;
	task_t *task = NULL;
	assert(NULL != sched);
	
	while (1 != sched->to_stop && !SchedIsEmpty(sched))
	{
		sched->current_task = PQDequeue(sched->pq);
		task = sched->current_task;
		
		time_to_run = TaskGetTimeToRun(sched->current_task);
		while (time(NULL) < time_to_run)
		{
			diff = difftime(time_to_run, time(NULL));
			sleep(diff);
		} 
		
		answer = TaskRun(task);
		
		switch(answer)
		{
			
			case (OP_CONTINUE):
				
				if (1 == sched->to_remove_current)
				{
					TaskDestroy(task);
					sched->current_task = NULL;
					break;
				}
				
				if (1 == sched->to_stop)
				{
					TaskDestroy(task);
					sched->current_task = NULL;
					return STOPPED;
				}
				
				TaskUpdateTimeToRun(task);
				
				if (1 == PQEnqueue(sched->pq, task)) 
				{
					TaskDestroy(task);
					sched->current_task = NULL;
					return MEMORY_ERR;
				}
				
				break;
			
			default:
			
				TaskDestroy(task);
				sched->current_task = NULL;
					
				break;
						
		}
		
		
		sched->to_remove_current = 0;
	}
	sched->to_stop = 0;
	
	return NO_MORE_TASKS; 	
}


int SchedRemove(sched_t *sched, ilrd_uid_t uid)
{
	task_t *task = NULL;
	
	assert(NULL != sched);
	
	task = PQErase(sched->pq, is_match, &uid);
	if (NULL == task)
	{
		if (NULL != sched->current_task)
		{
			if (TaskIsMatch(uid, sched->current_task))
			{
				
				sched->to_remove_current = 1;
				return SUCCESS;
			}
			return NOT_FOUND;
		}
		
		return NOT_FOUND;
	}
	
	TaskDestroy(task);
	return SUCCESS;
}

void SchedStop(sched_t *sched)
{
	assert(NULL != sched);
	
	sched->to_stop = 1;
}

size_t SchedSize(const sched_t *sched)
{
	assert(NULL != sched);
	
	if (NULL != sched->current_task)
	{
		return PQGetSize(sched->pq) + 1;
	}
	
	return PQGetSize(sched->pq);
}

void SchedClear(sched_t *sched)
{
	assert(NULL != sched);
	
	if (NULL != sched->current_task)
	{
		TaskDestroy(sched->current_task);
	}
	
	while (!PQIsEmpty(sched->pq))
	{
		TaskDestroy(PQDequeue(sched->pq));
	}	
	
	sched->current_task = NULL; 
	
}

int SchedIsEmpty(const sched_t *sched)
{
	assert(NULL != sched);
	
	return (PQIsEmpty(sched->pq) && NULL != sched->current_task);
}
