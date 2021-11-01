#include "kerevent.h"
#include "system.h"
#include "pcb.h"
#include "ivtentry.h"
#include "schedule.h"


// obrisani lockovi i unlockovi

KernelEv::KernelEv(IVTNo ivtNo){
	this->ivtNo = ivtNo;
	myOwner = (PCB*)PCB::runningThread;
	IVTEntry::table[ivtNo]->setEvent(this);
	value = 0;
	ownerBlocked = 0;
}

KernelEv::~KernelEv(){
	IVTEntry::table[ivtNo]->clearEvent();
}

void KernelEv::wait(){
	if(myOwner != (PCB*)PCB::runningThread){
		return;
	}
	if(value == 1){
		value = 0;
	}
	else{
		ownerBlocked = 1;
		PCB::runningThread->state = PCB::BLOCKED;
		System::dispatch();
	}
}

void KernelEv::signal(){
//ako nema blokiranih niti (stavi na 1)
// u suprotnom odblokirati nit
	if(ownerBlocked == 0){
		value = 1;
	}
	else{
		ownerBlocked = 1;
		myOwner->state = PCB::READY;
		Scheduler::put(myOwner);
	}
}
