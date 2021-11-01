#include "list.h"
#include "system.h"

List::List() {
lock;
	head =  0;
unlock;
}

List::~List() {
lock;
	Elem* temp;
	while(head != 0){
		temp = head;
		head = head->next;
		delete temp;
	}
	head = 0;
unlock;
}

List::Elem::Elem(Semaphore* _sem){
lock;
	sem = _sem;
	next = 0;
unlock;
}

void List::add(Semaphore* sem){
lock;
	Elem* temp = new Elem(sem);
	temp->next = head;
	head = temp;
unlock;
}

Semaphore* List::getById(int id){
lock;
	Semaphore* sem = 0;
	Elem* temp = head;
	while(temp != 0){
		if(temp->sem->getId() == id){
			sem = temp->sem;
			break;
		}
		temp = temp->next;
	}
unlock;
	return sem;
}

Semaphore* List::removeById(int id){
lock;
	Semaphore *sem = 0;
	if(head != 0){
		if(head->sem->getId() == id){
			Elem* temp = head;
			head = head->next;
			sem = temp->sem;
			delete temp;
		}
		else{
			Elem* prev = head;
			while(prev->next != 0 && prev->next->sem->getId() != id)
				prev = prev->next;
			if(prev->next == 0){
				unlock;
				return 0;
			}
			Elem* temp = prev->next;
			prev->next = temp->next;
			sem = temp->sem;
			delete temp;
		}
	}
unlock;
	return sem;
}

int List::lenght(){
lock;
	int size = 0;
	Elem* temp = head;
	while(temp != 0){
		size++;
		temp = temp->next;
	}
unlock;
	return size;
}

Semaphore* List::get(int pos){
lock;
	Semaphore* sem = 0;
	int cnt = 0;
	Elem* temp = head;
	while(temp != 0){
		cnt++;
		if(cnt == pos){
			sem = temp->sem;
			break;
		}
		temp = temp->next;
	}
unlock;
	return sem;
}
