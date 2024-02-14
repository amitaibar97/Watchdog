#include <unistd.h> /*getpid*/
#include <ifaddrs.h> /*getifaddr*/
#include <arpa/inet.h> /*inet_ntoa*/
#include <string.h> /*strcpy*/
#include <pthread.h> /*lock*/
#include <assert.h>/*assert*/
#include <time.h> /*time*/

#include "uid.h"


const ilrd_uid_t UIDBadUID = {0};
static size_t counter = 0;

/*=================================================================*/

static int GetIP(unsigned char *p)
{
	struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    
	assert(NULL != p);
	
    if (-1 == getifaddrs (&ifap))
    {
    	return -1;
    }
    
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) 
        {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            strcpy((char *)p, inet_ntoa(sa->sin_addr));
            break;
        }
    }
    
    freeifaddrs(ifap);
    return 0;
}


/*=================================================================*/
  
ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t UID = {0};
	pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

	if (0 != GetIP(UID.ip))
	{
		return UIDBadUID;
	}
	
	pthread_mutex_lock(&counter_lock);
	++counter;
	UID.counter = counter;
    pthread_mutex_unlock(&counter_lock);
	
	UID.time = time(NULL);
	UID.pid = (size_t)getpid();
	
	return UID;
}

/*=================================================================*/

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	if (uid1.counter != uid2.counter)
	{
		return 0;
	}
	
	if (0 != strcmp((char *)uid1.ip, (char *)uid2.ip))
	{
		return 0;
	}
	
	if (uid1.time != uid2.time)
	{
		return 0;
	}
	
	if (uid1.pid != uid2.pid)
	{
		return 0;
	}
	
	return 1;
}

