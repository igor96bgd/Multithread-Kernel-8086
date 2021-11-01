#include "semaphor.h"
#include "system.h"
#include "kersem.h"

List* Semaphore::globSemList = new List();

Semaphore::Semaphore(int init){
lock;
	myImpl = new KernelSem(init);
	globSemList->add(this);
unlock;
}

Semaphore::~Semaphore(){
lock;
	//delete globSemList;
	Semaphore::globSemList->removeById(myImpl->myId);
	delete myImpl;
unlock;
}

void Semaphore::signal(){
lock;
	myImpl->signal();
unlock;
}

int Semaphore::wait(Time maxTimeToWait){
lock;
	int type = myImpl->wait(maxTimeToWait);
unlock;
	return type;
}

int Semaphore::val() const{
lock;
	int val = myImpl->val();
unlock;
	return val;
}

int Semaphore::getId(){
	return myImpl->myId;
}

void Semaphore::decreaseSleepTime(){
lock;
	myImpl->decreaseSleepTime();
unlock;
}

void Semaphore::wakeUp(){
lock;
	myImpl->wakeUp();
unlock;
}

void Semaphore::sleepUpdate(){
lock;
	if(Semaphore::globSemList == 0){
		unlock;
		return;
	}
	int lenght = Semaphore::globSemList->lenght();
	for(int i = 0; i< lenght; i++){
		Semaphore::globSemList->get(i+1)->decreaseSleepTime();
		Semaphore::globSemList->get(i+1)->wakeUp();
	}
unlock;
}
