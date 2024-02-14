#include <stdio.h>
#include "watchdog.h"
#include <pthread.h>
#include <unistd.h>


#define TIME_TO_RUN 30
#define TIME_TO_DIE 5

int main(int argc, const char **argv, const char **env)
{
    
    int status = WDStart(argc, argv, env);
    size_t i = 0;
    for(i = 0; i < TIME_TO_RUN; ++i)
    {
        sleep(1);
    }
    WDStop();
    for(i = 0; i < TIME_TO_DIE; ++i)
    {
        sleep(1);
    }

   
    return status;
}