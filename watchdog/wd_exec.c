#define _XOPEN_SOURCE 700
#define _DEFAULT_SOURCE
#include "watchdog.h"
#include <signal.h>
#include <stdlib.h> /*setenv*/
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char **argv, const char **env)
{
    #ifndef NDEBUG
    printf("WD opened\n");
    #endif

    setenv("I_AM", "WD", 1);
    
    WDStart(argc, argv, env);


    return 0;

 }   
    