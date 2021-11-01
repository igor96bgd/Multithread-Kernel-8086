#ifndef _prioque_h_
#define _prioque_h_

#include "pcb.h"
#include "semaphor.h"
class PCB;

class PrioQueue {
private:
	class Elem {
	public:
		PCB* pcb;
		Time sleepCnt;
		Elem* next;
		Elem(PCB* _pcb, Time _sleepCnt);
	};
	Elem *head;
//	Elem * tail;
public:
	PrioQueue();
	~PrioQueue();
	void add(PCB* pcb, Time sleepCnt);
	Time getMinValue();
	PCB* remove();
	PCB* removeById(ID id);
	int empty();
	void decreaseSleepTime();
	int contains(PCB* pcb);
	int wakeUp(KernelSem* sem);
	//void printAll();
};

#endif
