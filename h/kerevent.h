#ifndef _kerevent_h_
#define _kerevent_h_


typedef unsigned char IVTNo;

class PCB;
class Scheduler;

// njemu sve public
class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
private:
	PCB* myOwner;
	IVTNo ivtNo;
	int value;
	int ownerBlocked;
};

#endif
