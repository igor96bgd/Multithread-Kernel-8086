#include "kersem.h"
#include "system.h"
#include "schedule.h"
#include "pcb.h"

int KernelSem::globalId = 0;
extern int syncPrintf(const char *format, ...);

KernelSem::KernelSem(int init){
lock;
	if(init < 0)
		init = 0;
	value = init;
	myId = ++globalId;
	blockedQueue = new Queue();
	sleepQueue = new PrioQueue();
unlock;
}

KernelSem::~KernelSem(){
	delete blockedQueue;
	delete sleepQueue;
}

void KernelSem::signal(){
lock;
	value++;
	if(value <= 0){
		PCB* blockedPCB = blockedQueue->remove();
		if(blockedPCB == 0){
			unlock;
			return;
		}
		if(sleepQueue->contains(blockedPCB) == 1){
			sleepQueue->removeById(blockedPCB->myId);
		}
		blockedPCB->state = PCB::READY;
		blockedPCB->signalType = 1;
		//syncPrintf("Nit %d signalizirana (semafor %d).\n",blockedPCB->myId, this->myId);
		//syncPrintf("Trenutno blokirane niti (nakon signala)(semafor %d):", this->myId);
		//blockedQueue->printAll();
		Scheduler::put(blockedPCB);
	}
unlock;
}

int KernelSem::wait(Time maxTimeToWait){
lock;
	int type;
	value--;
	if(value >= 0){
		unlock;
		return 0;
	}
	blockedQueue->add((PCB*)PCB::runningThread);
	PCB::runningThread->state = PCB::BLOCKED;
	if(maxTimeToWait > 0){
		sleepQueue->add((PCB*)PCB::runningThread, maxTimeToWait);
	}
	//syncPrintf("Nit %d blokirana na wait(%d) (semafor %d).\n",PCB::runningThread->myId, maxTimeToWait, this->myId);
	//syncPrintf("Trenutno blokirane niti (nakon waita) (semafor %d):", this->myId);
	//blockedQueue->printAll();
	dispatch();
	type = PCB::runningThread->signalType;
	PCB::runningThread->signalType = 0;
unlock;
	return type;
}

int KernelSem::val() const{
	int val;
lock;
	val = value;
unlock;
	return val;
}

void KernelSem::decreaseSleepTime(){
lock;
	sleepQueue->decreaseSleepTime();
unlock;
}

void KernelSem::wakeUp(){
lock;
	int cnt = sleepQueue->wakeUp(this);
	value += cnt;
unlock;
}
