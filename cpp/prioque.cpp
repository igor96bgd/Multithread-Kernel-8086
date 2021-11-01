#include "prioque.h"
#include "system.h"
#include "schedule.h"
#include "kersem.h"

extern int syncPrintf(const char *format, ...);

PrioQueue::PrioQueue() {
	lock;
	head = 0;
	unlock;
}

PrioQueue::~PrioQueue() {
	lock;
	Elem* temp;
	while (head != 0) {
		temp = head;
		head = head->next;
		delete temp;
	}
	head = 0;
	unlock;
}

PrioQueue::Elem::Elem(PCB* _pcb, Time _sleepCnt) {
	lock;
	pcb = _pcb;
	sleepCnt = _sleepCnt;
	next = 0;
	unlock;
}

void PrioQueue::add(PCB* pcb, Time sleepCnt) {
	lock;
	//syncPrintf("dodat sa slpCnt = %d\n", sleepCnt);
	Elem* temp = new Elem(pcb, sleepCnt);
	if (head == 0) {
		head = temp;
	} else if (sleepCnt < head->sleepCnt) {
		temp->next = head;
		head = temp;
	} else {
		Elem* prev = head;
		while (prev->next != 0 && prev->next->sleepCnt <= sleepCnt)
			prev = prev->next;
		temp->next = prev->next;
		prev->next = temp;
	}
	unlock;
}

PCB* PrioQueue::remove() {
	lock;
	PCB* pcb = 0;
	if (head != 0) {
		Elem* temp = head;
		head = head->next;
		pcb = temp->pcb;
		delete temp;
	}
	unlock;
	return pcb;
}

PCB* PrioQueue::removeById(ID id) {
	lock;
	if (head == 0) {
		unlock;
		return 0;
	}
	PCB* pcb = 0;
	if (head->pcb->myId == id) {
		Elem* temp = head;
		head = head->next;
		pcb = temp->pcb;
		delete temp;
	} else {
		Elem* prev = head;
		while (prev->next != 0 && prev->next->pcb->myId != id)
			prev = prev->next;
		if (prev->next == 0) {
			unlock;
			return 0;
		}
		Elem* temp = prev->next;
		prev->next = temp->next;
		pcb = temp->pcb;
		delete temp;
	}
	unlock;
	return pcb;
}

int PrioQueue::empty() {
	lock;
	int ret = 1;
	if (head != 0)
		ret = 0;
	unlock;
	return ret;
}

Time PrioQueue::getMinValue() {
	lock;
	Time ret = 0;
	if (head != 0)
		ret = head->sleepCnt;
	unlock;
	return ret;
}

void PrioQueue::decreaseSleepTime() {
	lock;
	Elem *temp = head;
	while (temp != 0) {
		temp->sleepCnt--;
		temp = temp->next;
	}
	//syncPrintf("umanjen slpCnt\n");
	/*if(head != 0){
	 syncPrintf("prvi na %d\n", head->sleepCnt);
	 }*/
	unlock;
}

int PrioQueue::contains(PCB* pcb) {
	lock;
	int ret = 0;
	Elem* temp = head;
	while (temp != 0) {
		if (temp->pcb->myId == pcb->myId) {
			ret = 1;
			break;
		}
		temp = temp->next;
	}
	unlock;
	return ret;
}

int PrioQueue::wakeUp(KernelSem* sem) {
	lock;
	int cnt = 0;
	while (head != 0 && head->sleepCnt <= 0) {
		Elem* temp = head;
		head = head->next;
		temp->pcb->state = PCB::READY;
		cnt++;
		if(sem->blockedQueue->contains(temp->pcb->myId)){
			sem->blockedQueue->removeByID(temp->pcb->myId);
		}
		Scheduler::put(temp->pcb);
		delete temp;
	}
unlock;
return cnt;
}
