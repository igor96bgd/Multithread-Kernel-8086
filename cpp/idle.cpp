#include "idle.h"
#include "pcb.h"
#include "system.h"

void Idle::start(){
lock;
	myPCB->state = PCB::READY;
	myPCB->createStack();
unlock;
}

void Idle::run(){
	while(1);
}
