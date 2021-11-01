#ifndef _list_h_
#define _list_h_

#include "semaphor.h"

class Semaphore;

class List{
	class Elem {
	public:
		Semaphore* sem;
		Elem* next;
		Elem(Semaphore* _sem);
	};
	Elem *head;
public:
	List();
	~List();
	void add(Semaphore* sem);
	Semaphore* getById(int id);
	Semaphore* removeById(int id);
	Semaphore* get(int pos);
	int lenght();
};


#endif
