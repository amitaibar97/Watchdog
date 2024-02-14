#Watchdog: Process Monitoring System
##Project Description
Watchdog is a software component designed to monitor time-critical tasks by overseeing processes in C and facilitating IPC between them. It ensures the reliability of critical processes by employing a mutual protection mechanism, where each process is monitored by a dedicated watchdog process created via fork(). This watchdog is vigilant, receiving regular SIGUSR1 signals from the monitored process, thanks to a scheduler and various data structures like doubly linked lists, sorted lists, and priority queue which I implemented in C earlier.

##Key Features:

* Mutual Protection: Each process watches over another, stepping in to revive it if necessary.
* Fault Tolerance: Recognizes the slightest sign of activity as an indication of the process being alive, accommodating the potential loss of signals.
##How It Works.

* Monitoring Setup: When a process requires monitoring, it initiates the creation of a watchdog process by the wdStart API.
* Signal Exchange: The monitored process sends SIGUSR1 signals at a constant rate to the watchdog, indicating its active status.
* Vitality Checks: The watchdog periodically checks for these signals. A lack of signals triggers the watchdog to revive the monitored process.
* Termination: Monitoring can be gracefully stopped using the wdStop API by sending SIGUSR2 to the watchdog proccess, ensuring controlled termination of the watchdog's vigilance.
* the pace of sending and measuring the signals by the proccesses can be controlled by an attached config-file which the user can edit according to its needs. 
##Installation and Usage
clone the repo into the computer and use the makefile in oreder to make a dynamic libraries with all the dependencies. there is an option to make a debug of release versions. the debug contains some printing to get some live feedback of the singls, the sender pid and dest pid.



