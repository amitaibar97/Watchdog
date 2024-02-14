#ifndef __UID_H__
#define __UID_H__

#include <time.h>


typedef struct uid
{
	size_t counter;
	time_t time;
	size_t pid;
	unsigned char ip[14];  
	
}ilrd_uid_t;

extern const ilrd_uid_t UIDBadUID;


/*
*
*	Creates a UID instance.
*
*	Arguments: none.
*
*	Return: If successful, returns an instance of the UID struct. Otherwise, returns a bad UID. 
*		
*
*	Time complexity: O(1)
*	Space complexity: O(1)
*
*/
ilrd_uid_t UIDCreate(void);
/*
*	Checks if two UID instances are the same.
*
*	Arguments: Two instances of UID struct.
*
*	Return: Boolean value - 1 if same, otherwise 0.
*		
*
*	Time complexity: O(1)
*	Space complexity: O(1)
*
*/
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2);


#endif /* __UID_H__ */
