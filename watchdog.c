#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE /*set_env*/

#include <unistd.h>    /*fork*/
#include <stdlib.h>    /*exit*/
#include <stdio.h>     /*sprintf*/
#include <pthread.h>   /*phread_create*/
#include <signal.h>    /*kill*/
#include <stdatomic.h> /*atomic_fetch_add*/
#include <semaphore.h> /*sem_wait*/
#include <string.h>    /*strcmp*/
#include <fcntl.h>     /*O_CREAT*/

#include "watchdog.h"
#include "scheduler.h"


#ifndef NDEBUG
#define PRINTF_DEBUG(...) (printf(__VA_ARGS__))
#else
#define PRINTF_DEBUG(...)
#endif

enum to_die
{
    OFF,
    ON
};

#define SENDING_INTERVAL 1
#define CHECKING_INTERVAL 5
#define TASK3_SIGNALS_NUM 5
#define SEM_LEN 20
#define CONFIG_FILE "wd_config_file.txt"
#define MAX_LINE_LEN 100

/*************************************************************************************************************/

const char **global_argv = NULL;
pid_t global_pid = 0;
static atomic_size_t counter = 0;
static int to_die_flag = OFF;
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
static int SignalHandlersInit(void);

static void *PThreadFunc(void *arg);

/*************************************************************************************************************/

static void *PThreadFunc(void *send_to_pid)
{

    if (1 == SchedInit(send_to_pid))
    {

        PRINTF_DEBUG("sched init failed\n");
    }

    PRINTF_DEBUG("%s : scheduler ready\n", getenv("I_AM"));

    sem_wait(is_process_ready);
    if (SUCCESS != SchedRun(new_process_sched))
    {

        PRINTF_DEBUG("task not success\n");
    }
    return NULL;
}

/*************************************************************************************************************/

int WDStart(int argc, const char **argv, const char **env)
{
    char sem_name[SEM_LEN] = {0};
    if (1 == SignalHandlersInit())
    {

        PRINTF_DEBUG("%d : signal handlers init problem", getpid());

        return WD_ERR;
    }

    enviroment = getenv("ALREADY_ACTIVATED");

    global_argv = argv;

    if (NULL == enviroment)
    {
        size_t unique_named_semaphore = 0;
        
        setenv("ALREADY_ACTIVATED", "1", 1);

        PRINTF_DEBUG("\tfirst time\n");

        unique_named_semaphore = (rand() % 100000 + 1) % getpid() + getpid();
        sprintf(sem_name, "%lu", unique_named_semaphore);
        setenv("SEM_NAME", sem_name, 1);
        is_process_ready = sem_open(sem_name, O_CREAT, 0644, 0);

        if (0 != setenv("I_AM", "CLIENT", 1))
        {

            PRINTF_DEBUG("failed\n");

            return 1;
        }

        address_pid = fork();

        if (address_pid < 0)
        {
            return WD_ERR;
        }

        /*parent*/
        if (address_pid > 0)
        {

            printf("now %d active\n", getpid());

            pthread_create(&new_thread, NULL, PThreadFunc, &address_pid);
        }
        /*child*/
        if (address_pid == 0)
        {

            PRINTF_DEBUG("opening wd.out\n");

            execv("./wd_exec.out", (char **)global_argv); /*open wd.out as a new proccess*/
        }
    }

    else
    {
        address_pid = getppid();
        strcpy(sem_name, getenv("SEM_NAME"));
        is_process_ready = sem_open(sem_name, O_CREAT, 0644, 0);

        PRINTF_DEBUG("\tnot first time\n");

        PRINTF_DEBUG("i am %s, my pid is: %d\n", getenv("I_AM"), getpid());

        if (0 == strcmp(getenv("I_AM"), "WD"))
        {
            if (0 != SchedInit(&address_pid))
            {

                PRINTF_DEBUG("failed to init sched\n");

                return WD_FAILURE;
            }

            PRINTF_DEBUG("%s : scheduler ready\n", getenv("I_AM"));

            PRINTF_DEBUG("wd start\n");

            sem_post(is_process_ready);
            SchedRun(new_process_sched);
            SchedDestroy(new_process_sched);
        }

        else
        {

            PRINTF_DEBUG("i am  %s,  parent pid is: %d\n", getenv("I_AM"), getppid());

            setenv("I_AM", "CLIENT", 1);
            pthread_create(&new_thread, NULL, PThreadFunc, &address_pid);
        }
    }

    return WD_SUCCESS;
}

int WDStop(void)
{
    char sem_name[SEM_LEN] = {0};
    SchedRemove(new_process_sched, task2_uid);

    while (to_die_flag == OFF)
    {
        if (-1 == kill(address_pid, SIGUSR2))
        {
            perror("kill");
            return WD_ERR;
        }
        sleep(1);
    }

    SchedStop(new_process_sched);

    PRINTF_DEBUG("destroy sched\n");

    if (pthread_join(new_thread, NULL))
    {
        fprintf(stderr, "[client] Failed to finish WD thread\n");
        return (WD_ERR);
    }
    SchedDestroy(new_process_sched);
    strcpy(sem_name, getenv("SEM_NAME"));

    sem_unlink(sem_name);
    sem_close(is_process_ready);

    PRINTF_DEBUG("terminated\n");

    return WD_SUCCESS;
}

static int SchedInit(void *send_to)
{
    new_process_sched = SchedCreate();
    size_t sending_interval = 0;
    size_t checking_interval = 0;
    FILE *fp = NULL;
    char line[MAX_LINE_LEN] = {'\0'};

    /*config file code*/

    fp = fopen(CONFIG_FILE, "r");
    if (NULL != fp)
    {
        fgets(line, sizeof(line), fp);
        fscanf(fp, "%lu", &sending_interval);
        fgets(line, sizeof(line), fp);
        fgets(line, sizeof(line), fp);
        fscanf(fp, "%lu", &checking_interval);

        PRINTF_DEBUG("%s : sending interval is : %lu\n checking interval is : %lu\n", getenv("I_AM"), sending_interval, checking_interval);

        fclose(fp);
    }

    task1_uid = SchedAdd(new_process_sched, 0, sending_interval, Task1OperFunc, send_to, NULL, TaskCleanFunc);
    if (UIDIsSame(UIDBadUID, task1_uid))
    {
        SchedDestroy(new_process_sched);
        return 1;
    }

    task2_uid = SchedAdd(new_process_sched, checking_interval, checking_interval, Task2OperFunc, NULL, NULL, TaskCleanFunc);
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
        perror("sigaction1 failed\n");
        return 1;
    }

    if (-1 == sigaction(SIGUSR2, &sa2, NULL))
    {
        perror("sigaction1 failed\n");
        return 1;
    }

    return SUCCESS;
}

static int Task1OperFunc(void *param)
{

    PRINTF_DEBUG("%s(%d) :i'm alive, send to %d\n", getenv("I_AM"), getpid(), *(pid_t *)param);

    if (-1 == kill(*(pid_t *)param, SIGUSR1))
    {

        PRINTF_DEBUG("%d :signal failed\n", getpid());

        return OP_ERROR;
    }

    return OP_CONTINUE;
}

static int Task2OperFunc(void *param)
{

    if (counter != 0)
    {
        counter = 0;

        PRINTF_DEBUG("task2 - %s :make counter 0\n", getenv("I_AM"));

        return OP_CONTINUE;
    }

    else if (0 == strcmp(getenv("I_AM"), "CLIENT"))
    {

        address_pid = fork();
        if (address_pid < 0)
        {
            perror("fork error\n");
            return WD_ERR;
        }

        if (0 == address_pid)
        {

            PRINTF_DEBUG("%s :revive WD proccess\n", getenv("I_AM"));

            execv("./wd_exec.out", (char **)global_argv);
        }

        else
        {
            sem_wait(is_process_ready);
        }
    }
    else
    {

        PRINTF_DEBUG("%s :revive WD proccess\n", getenv("I_AM"));

        sem_close(is_process_ready);
        sem_unlink("wd1");
        SchedDestroy(new_process_sched);
        unsetenv("ALREADY_ACTIVATED");
        execv(global_argv[0], (char **)global_argv);
    }

    return OP_CONTINUE;
}

static int Task3OperFunc(void *param)
{
    size_t i = 0;
    if (to_die_flag != OFF)
    {
        SchedStop(new_process_sched);

        PRINTF_DEBUG("task3 :%s to die flag is up\n", getenv("I_AM"));

        for (i = 0; i < TASK3_SIGNALS_NUM; ++i)
        {
            if (-1 == kill(*(pid_t *)param, SIGUSR2))
            {
                break;
            }
            sleep(1);
        }

        PRINTF_DEBUG("%s : terminated\n", getenv("I_AM"));

        return OP_DONE;
    }

    return OP_CONTINUE;
}

static void SignalHandler1(int sig)
{
    (void)sig;
    atomic_fetch_add(&counter, 1);

    PRINTF_DEBUG("signal handler1, %s :got messege, life counter : %lu\n", getenv("I_AM"), counter);
}

static void SignalHandler2(int sig)
{
    (void)sig;

    to_die_flag = ON;
}

static void TaskCleanFunc(void *param)
{
    (void)(param);
}
