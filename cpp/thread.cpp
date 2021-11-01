#include "thread.h"

#include "pcb.h"
#include "schedule.h"
#include "system.h"

extern int syncPrintf(const char *format, ...);
Thread::Thread(StackSize stackSize, Time timeSlice){
lock;
	if(stackSize > maxStackSize)
		stackSize = maxStackSize;
	if(stackSize < minStackSize)
		stackSize = minStackSize;
	myPCB = new PCB(this, stackSize, timeSlice);
unlock;
}

Thread::~Thread(){
lock;
	//waitToComplete();
	delete myPCB;
unlock;
}

ID Thread::getId(){
lock;
	ID id = myPCB->myId;
unlock;
	return id;
}

ID Thread::getRunningId(){
lock;
	ID id = PCB::runningThread->myId;
unlock;
	return id;
}

Thread* Thread::getThreadById(ID id){
lock;
	PCB* pcb = PCB::globalQueue->getByID(id);
	Thread* thr = 0;
	if(pcb != 0){
		thr = pcb->thread;
	}
unlock;

	return thr;
}

void Thread::start(){
lock;
	myPCB->state = PCB::READY;
	myPCB->createStack();
	Scheduler::put(myPCB);
unlock;
}

void Thread::waitToComplete(){
lock;
	if(this->myPCB->state == PCB::DONE){
		unlock;
		return;
	}
	if(this->myPCB == (PCB*)PCB::runningThread){
		unlock;
		return;
	}
	if(this->myPCB == PCB::sysThread){
		unlock;
		return;
	}
	if(this->myPCB == PCB::idleThread){
		unlock;
		return;
	}
	PCB::runningThread->state = PCB::BLOCKED;
	myPCB->waitingQueue->add((PCB*)(PCB::runningThread));
	System::dispatch();
unlock;
}

void dispatch(){
lock;
	System::dispatch();
unlock;
}

