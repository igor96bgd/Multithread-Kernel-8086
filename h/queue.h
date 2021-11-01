#ifndef _queue_h_
#define _queue_h_

#include "pcb.h"
#include "thread.h"
class PCB;

class Queue {
private:
	class Elem {
	public:
		PCB* pcb;
		Elem* next;
		Elem(PCB* _pcb);
	};
	Elem *head;
	Elem * tail;
public:
	Queue();
	~Queue();
	void add(PCB* pcb);
	PCB* remove();
	PCB* removeByID(ID id);
	PCB* getByID(ID id);
	void printAll();
	int contains(ID id);
};

#endif
