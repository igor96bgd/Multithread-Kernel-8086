// File: semaphor.h
#ifndef _semaphor_h_
#define _semaphor_h_

#include "list.h"

class List;

typedef unsigned int Time;
class KernelSem;
class Semaphore {
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();
	virtual int wait(Time maxTimeToWait);
	virtual void signal();
	int val() const; // Returns the current value of the semaphore
	int getId();
	static List *globSemList;
	void decreaseSleepTime();
	void wakeUp();
	static void sleepUpdate();
private:
	KernelSem* myImpl;
};
#endif
