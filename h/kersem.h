#ifndef _kersem_h_
#define _kersem_h_

#include "semaphor.h"
#include "queue.h"
#include "prioque.h"

class KernelSem{
public:
	static int globalId;
	int myId;
	int value;
	Queue* blockedQueue;
	PrioQueue* sleepQueue;

	KernelSem(int init);
	virtual ~KernelSem();
	virtual int wait(Time maxTimeToWait);
	virtual void signal();
	int val() const;
	void decreaseSleepTime();
	void wakeUp();
};





#endif
