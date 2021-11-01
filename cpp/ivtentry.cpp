#include "ivtentry.h"
#include "system.h"
#include "kerevent.h"
#include <dos.h>

IVTEntry* IVTEntry::table[256] = {0};

IVTEntry::IVTEntry(IVTNo _entryNo, InterruptRoutine newRoutine){
lock;
	entryNo = _entryNo;
	oldRoutine = 0;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(entryNo);
	setvect(entryNo, newRoutine);
#endif
	myEvent = 0;
	IVTEntry::table[entryNo] = this;
unlock;
}

IVTEntry::~IVTEntry(){
lock;
	IVTEntry::table[entryNo] = 0;
#ifndef BCC_BLOCK_IGNORE
	setvect(entryNo, oldRoutine);
#endif
unlock;
}

void IVTEntry::signal(){
lock;
	if(myEvent != 0)
		myEvent->signal();
unlock;
}

void IVTEntry::callOld(){
lock;
	oldRoutine();
unlock;
}

void IVTEntry::setEvent(KernelEv* event){
lock;
	if(myEvent == 0)
		myEvent = event;
unlock;
}

void IVTEntry::clearEvent(){
lock;
	myEvent = 0;
unlock;
}

IVTEntry* IVTEntry::getEntry(IVTNo entry){
lock;
	IVTEntry* ivte = 0;
	if(IVTEntry::table[entry] != 0)
		ivte = IVTEntry::table[entry];
unlock;
	return ivte;
}
