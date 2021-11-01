#ifndef _pcb_h_
#define _pcb_h_

#include "queue.h"
#include "thread.h"

typedef int State;

class System;
class Queue;

class PCB{
public:
	static ID globalId;
	ID myId;
	StackSize stackSize;
	unsigned char *stack;
	volatile Time timeSlice;
	volatile Time timeCnt;
	Thread* thread;
	Queue* waitingQueue;
	static Queue* globalQueue;
	unsigned ss, sp;
	static const State NEW, READY, BLOCKED, DONE;
	State state;
	static volatile PCB* runningThread;
	static PCB* idleThread;
	static PCB* sysThread;
	int signalType;



	PCB(Thread* thread, StackSize stackSize, Time timeSlice);
	void createStack();
	~PCB();
};

#endif
