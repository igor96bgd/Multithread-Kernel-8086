#include "queue.h"
#include "system.h"

Queue::Queue() {
	lock;
	head = 0;
	tail = 0;
	unlock;
}

Queue::~Queue() {
	lock;
	Elem* temp;
	while(head != 0){
		temp = head;
		head = head->next;
		delete temp;
	}
	head = 0;
	tail = 0;
	unlock;
}

Queue::Elem::Elem(PCB* _pcb) {
	lock;
	pcb = _pcb;
	next = 0;
	unlock;
}

void Queue::add(PCB* pcb) {
	lock;
	Elem* temp = new Elem(pcb);
	if (head == 0) {
		head = temp;
		tail = head;
	} else {
		tail->next = temp;
		tail = temp;
	}
	unlock;
}

PCB* Queue::remove() {
lock;
	if (head == 0){
		unlock;
		return 0;
	}
	Elem* temp = head;
	head = head->next;
	if (head == 0)
		tail = 0;
	PCB* pcb = temp->pcb;
	delete temp;
unlock;
	return pcb;
}

PCB* Queue::getByID(ID id){
lock;
	if(head == 0){
		unlock;
		return 0;
	}
	PCB* pcb = 0;
	Elem* temp = head;
	while(temp != 0){
		if(temp->pcb->myId == id){
			pcb = temp->pcb;
			break;
		}
		temp = temp->next;
	}
unlock;
	return pcb;
}

PCB* Queue::removeByID(ID id) {
lock;
	if (head == 0){
		unlock;
		return 0;
	}
	PCB* pcb = 0;
	if (head->pcb->myId == id) {
		Elem* temp = head;
		head = head->next;
		if (head == 0)
			tail = 0;
		pcb = temp->pcb;
		delete temp;
	} else {
		Elem* prev = head;
		while (prev->next != 0 && prev->next->pcb->myId != id)
			prev = prev->next;
		if (prev == tail) {
			unlock;
			return 0;
		}
		else if (prev->next == tail) {
			Elem* temp = tail;
			tail = prev;
			tail->next = 0;
			pcb = temp->pcb;
			delete temp;
		} else {
			Elem* temp = prev->next;
			prev->next = temp->next;
			pcb = temp->pcb;
			delete temp;
		}
	}
	unlock;
	return pcb;
}
/* FALI EXTERN INT SYNCPRINTF*/
void Queue::printAll(){
	lock;
	Elem* temp = head;
	while(temp != 0){
		//syncPrintf(" %d",temp->pcb->myId);
		temp = temp->next;
	}
	//syncPrintf("\n");
	unlock;
}

int Queue::contains(ID id){
lock;
	Elem* temp = head;
	while(temp != 0){
		if(temp->pcb->myId == id){
			unlock;
			return 1;
		}
		temp = temp->next;
	}

unlock;
	return 0;
}
