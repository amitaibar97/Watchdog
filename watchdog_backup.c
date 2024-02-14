#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE

#include <unistd.h>    /*fork*/
#include <stdlib.h>    /*exit*/
#include <stdio.h>     /*sprintf*/
#include <pthread.h>   /*phread_create*/
#include <signal.h>    /*kill*/
#include <stdatomic.h> /*atomic_fetch_add*/
#include <semaphore.h> /*sem_wait*/
#include <string.h>    /*strcmp*/
#include <fcntl.h>

#include "watchdog.h"
#include "scheduler.h"

#define SENDING_INTERVAL 1
#define CHECKING_INTERVAL 5

const char **global_argv = NULL;
pid_t global_pid = 0;
static atomic_size_t counter = 0;
static int to_die_flag = 0;
sem_t *is_process_ready;

ilrd_uid_t task1_uid = {0};
ilrd_uid_t task2_uid = {0};
ilrd_uid_t task3_uid = {0};

pthread_t new_thread;
sched_t *new_process_sched = NULL;
static char *enviroment = NULL;
char *process_to_revive = NULL;
pid_t address_pid = 0;

/*************************************************************************************************************/
static int Task1OperFunc(void *param);
static int Task2OperFunc(void *param);
static int Task3OperFunc(void *param);
static void TaskCleanFunc(void *param);

static void SignalHandler1(int sig);
static void SignalHandler2(int sig);

static int SchedInit(void *send_to);
static int SignalHandlersInit();

static void *PThreadFunc(void *arg);

/*************************************************************************************************************/

static void *PThreadFunc(void *send_to_pid)
{

    if (1 == SchedInit(send_to_pid))
    {
        printf("sched init failed\n");
    }
    printf("%s : scheduler ready\n", getenv("I_AM"));

    sem_wait(is_process_ready);
    if (0 != SchedRun(new_process_sched))
    {
        printf("task not success\n");
    }
    return NULL;
}

/*************************************************************************************************************/

int WDStart(int argc, const char **argv, const char **env)
{

    if (1 == SignalHandlersInit())
    {
        printf("%d : signal handlers init problem", getpid());
        return WD_ERR;
    }

    is_process_ready = sem_open("wd1", O_CREAT, 0644, 0);

    enviroment = getenv("ALREADY_ACTIVATED");

    global_argv = argv;

    if (NULL == enviroment)
    {
        pid_t wd_pid = 0;
        setenv("ALREADY_ACTIVATED", "1", 1);
        printf("\tfirst time\n");

        if (0 != setenv("I_AM", "CLIENT", 1))
        {
            printf("failed\n");
            return 1;
        }

        wd_pid = fork();

        if (wd_pid < 0)
        {
            return WD_ERR;
        }

        /*parent*/
        if (wd_pid > 0)
        {
            printf("now %d active\n", getpid());
            address_pid = wd_pid;
            pthread_create(&new_thread, NULL, PThreadFunc, &address_pid);
        }
        /*child*/
        if (wd_pid == 0)
        {
            printf("opening wd.out\n");
            execv("./wd_exec.out", (char **)global_argv); /*open wd.out as a new proccess*/
        }
    }

    else
    {
        pid_t parent_pid = getppid();

        printf("\tnot first time\n");
        printf("i am %s, parent pid is: %d\n", getenv("I_AM"), getppid());

        if (0 == strcmp(getenv("I_AM"), "WD"))
        {
            if (0 != SchedInit(&parent_pid))
            {
                printf("failed to init sched\n");
            }
            printf("%s : scheduler ready\n", getenv("I_AM"));

            /* sleep(1); */
            printf("wd start\n");
            sem_post(is_process_ready);
            return SchedRun(new_process_sched);
            SchedDestroy(new_process_sched);
        }

        else
        {
            pid_t pid_to_send = getppid();
            address_pid = pid_to_send;
            printf("i am  %s,  parent pid is: %d\n", getenv("I_AM"), getppid());
            setenv("I_AM", "CLIENT", 1);
            pthread_create(&new_thread, NULL, PThreadFunc, &pid_to_send);
        }
    }

    return WD_SUCCESS;
}

int WDStop(void)
{
    SchedRemove(new_process_sched, task2_uid);

    while (to_die_flag == 0)
    {
        if (-1 == kill(address_pid, SIGUSR2))
        {
            perror("kill");
            return WD_ERR;
        }
        sleep(1);
    }

    SchedStop(new_process_sched);
    printf("destroy sched\n");
    /* pthread_cancel(new_thread); */

    if (pthread_join(new_thread, NULL))
    {
        fprintf(stderr, "[client] Failed to finish WD thread\n");
        return (WD_ERR);
    }
    SchedDestroy(new_process_sched);
    sem_unlink("wd1");
    sem_close(is_process_ready);
    printf("terminated wd\n");
    return WD_SUCCESS;
}

static int SchedInit(void *send_to)
{
    new_process_sched = SchedCreate();

    task1_uid = SchedAdd(new_process_sched, 0, SENDING_INTERVAL, Task1OperFunc, send_to, NULL, TaskCleanFunc);
    if (UIDIsSame(UIDBadUID, task1_uid))
    {
        SchedDestroy(new_process_sched);
        return 1;
    }

    task2_uid = SchedAdd(new_process_sched, CHECKING_INTERVAL, CHECKING_INTERVAL, Task2OperFunc, NULL, NULL, TaskCleanFunc);
    if (UIDIsSame(UIDBadUID, task2_uid))
    {
        SchedDestroy(new_process_sched);
        return 1;
    }

    if (0 == strcmp(getenv("I_AM"), "WD"))
    {
        task3_uid = SchedAdd(new_process_sched, 0, CHECKING_INTERVAL, Task3OperFunc, send_to, NULL, TaskCleanFunc);
        if (UIDIsSame(UIDBadUID, task3_uid))
        {
            SchedDestroy(new_process_sched);
            return 1;
        }
    }
    return WD_SUCCESS;
}

static int SignalHandlersInit()
{
    struct sigaction sa1;
    struct sigaction sa2;

    sa1.sa_handler = SignalHandler1;
    sa2.sa_handler = SignalHandler2;

    sigemptyset(&sa1.sa_mask);
    sa1.sa_flags = 0;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;

    if (-1 == sigaction(SIGUSR1, &sa1, NULL))
    {
        printf("sigaction1 failed\n");
        return 1;
    }

    if (-1 == sigaction(SIGUSR2, &sa2, NULL))
    {
        printf("sigaction1 failed\n");
        return 1;
    }

    return SUCCESS;
}

static int Task1OperFunc(void *param)
{

    printf("%s :i'm alive\n", getenv("I_AM"));
    if (-1 == kill(*(pid_t *)param, SIGUSR1))
    {
        printf("%d :signal failed\n", getpid());
        return OP_ERROR;
    }

    return OP_CONTINUE;
}

static int Task2OperFunc(void *param)
{
    if (counter != 0)
    {
        counter = 0;
        printf("task2 - %s :make counter 0\n", getenv("I_AM"));
        return OP_CONTINUE;
    }

    /*  else
    {
        printf("other process is not responding\n");

        global_pid = fork();


        printf("%d Opening new process because no signal was recived = %s\n", getpid(), getenv("I_AM"));
        if (global_pid == 0)
        {


            if (0 == strcmp(getenv("I_AM"), "WD"))
            {
                printf("WD opened a new process\n");
                execvp(global_argv[0],(char **)global_argv);
            }


            else if (0 == strcmp(getenv("I_AM"), "CLIENT"))
            {
                printf("the process opened a new WD\n");
                execvp("wd_exec.out", (char **)global_argv);
            }
        }
        else
        {
            sem_wait(is_process_ready);
        }
         */

    return OP_DONE;
}

static int Task3OperFunc(void *param)
{
    size_t i = 0;
    if (to_die_flag != 0)
    {
        SchedStop(new_process_sched);
        printf("task3 :%s to die flag is up\n", getenv("I_AM"));
        for (i = 0; i < 5; ++i)
        {
            if (-1 == kill(*(pid_t *)param, SIGUSR2))
            {
                break;
            }
            sleep(1);
        }
        
        printf("%s : terminated\n", getenv("I_AM"));
        return OP_DONE;
    }

    return OP_CONTINUE;
}

static void SignalHandler1(int sig)
{
    (void)sig;
    atomic_fetch_add(&counter, 1);

    printf("signal handler1, %s :got messege, life counter : %lu\n", getenv("I_AM"), counter);
}

static void SignalHandler2(int sig)
{
    (void)sig;

    to_die_flag = 1;
}

static void TaskCleanFunc(void *param)
{
    (void)(param);
}