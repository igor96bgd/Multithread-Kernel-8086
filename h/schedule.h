#ifndef _schedule_h_
#define _schedule_h_

// File: schedule.h
class PCB;
class Scheduler {
public:
	static void put(PCB*);
	static PCB* get();
};

#endif
