#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__
#include <unistd.h>

enum status
{
    WD_SUCCESS,
    WD_ERR,
    WD_FAILURE,
    WD_FORK_ERR
};

	 /*	Invoke this function to create a watchdog process that will keep the
	 *	calling process alive by re-executing it in case of a crash. The
	 *	re-execution of the program will begin from the first instruction of the
	 *	program.
	 *	Arguments:
	 *		-argc: 	The number of command-line arguments to be passed to the
	 *				calling process at re-execution, including the file name.
	 *		-argv:	The null-terminated array of strings to be used as command-
	 *				line arguments to the process at re-execution. By convention
	 *				argv[0] is the file name of the executable binary.
	 *		-env:	The null-terminated string array of environmental variables
	 *				to be passed to the calling process at re-execution.
	 *	Return value: status of the function activation.
	 */
	 
int WDStart(int argc, const char **argv, const char **env);


	 /*	Cancel the watchdog process invoked by MMI and stop re-execution of the
	 *	calling process.
	 *		-wd_pid:	The PID of the concurrently running watchdog process.
	 *
	 *	Note:	The canceling of the watchdog process works by invoking SIGUSR2.
	 *			The disposition of this signal is to terminate a process. 
	 */
	 
int WDStop(void);













#endif /*__WATCHDOG_H__*/
