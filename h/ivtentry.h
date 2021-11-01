#ifndef _ivtentry_h_
#define _ivtentry_h_

typedef unsigned char IVTNo;
typedef void interrupt (*InterruptRoutine)(...);

class KernelEv;

// njemu je sve public
class IVTEntry{
private:
	IVTNo entryNo;
	InterruptRoutine oldRoutine;
	KernelEv* myEvent;
public:
	IVTEntry(IVTNo _entryNo, InterruptRoutine newRoutine);
	~IVTEntry();
	void signal();
	void callOld();
	void setEvent(KernelEv* event);
	void clearEvent();
	static IVTEntry* getEntry(IVTNo entry);
	static IVTEntry* table[256];
};

#define PREPAREENTRY(entryNo, callingOld)\
	void interrupt intr##entryNo(...);\
	IVTEntry entry##entryNo(entryNo, intr##entryNo);\
	void interrupt intr##entryNo(...){\
		entry##entryNo.signal();\
		if(callingOld == 1)\
			entry##entryNo.callOld();\
	}


#endif
