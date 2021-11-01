#ifndef _idle_h_
#define _idle_h_

#include "thread.h"

//class Thread;

class Idle : Thread{
public:
	void start();
	virtual void run();
};


#endif
