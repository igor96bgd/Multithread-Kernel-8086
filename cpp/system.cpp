#include "system.h"
#include "schedule.h"
#include <dos.h>

#include "idle.h"
#include "pcb.h"
#include "thread.h"
#include "queue.h"
#include "semaphor.h"
extern int syncPrintf(const char *format, ...);
volatile int System::explicit_dispatch = 0;
IntrRoutine System::oldRoutine = 0;
//void interrupt (*System::oldRoutine)(...) = 0;

void System::dispatch(){
lock;
	explicit_dispatch = 1;
	timer();
	explicit_dispatch = 0;
unlock;
}

void System::wrapper(){
	PCB::runningThread->thread->run();
lock;
	PCB::runningThread->state = PCB::DONE;
	PCB* temp = PCB::runningThread->waitingQueue->remove();
	while(temp != 0){
		temp->state = PCB::READY;
		Scheduler::put(temp);
		temp = PCB::runningThread->waitingQueue->remove();
	}
	System::dispatch();
}

void System::init(){
lock;
	oldRoutine = System::replaceRoutine(0x08, timer);
/*#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(0x08);
	setvect(0x08, timer);
#endif*/
	Thread* sys = new Thread();
	sys->myPCB->state = PCB::READY;
	PCB::sysThread = sys->myPCB;
	PCB::runningThread = (volatile PCB*)PCB::sysThread;

	Idle* idle = new Idle();
	PCB::idleThread = idle->myPCB;
	idle->start();
unlock;
}

int System::terminate(){
	if(PCB::runningThread != PCB::sysThread)
		return 1;
lock;
	System::replaceRoutine(0x08, oldRoutine);
/*#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, oldRoutine);
#endif*/
	delete PCB::idleThread->thread;
	delete PCB::sysThread->thread;
unlock;
	return 0;
}

void interrupt System::timer(){
	if(explicit_dispatch == 0){
		tick();
		(*oldRoutine)(); // Mozda treba uokviriti sa lock/unlock
		Semaphore::sleepUpdate();
	}
#ifndef BCC_BLOCK_IGNORE
	asm { cli };
#endif


	if(explicit_dispatch == 0){
		PCB::runningThread->timeCnt++;
	}
	if(explicit_dispatch == 0 && (PCB::runningThread->timeSlice == 0 || (PCB::runningThread->timeCnt < PCB::runningThread->timeSlice))){
		return;
	}
	explicit_dispatch = 0;
	static volatile PCB* nextThread;
	nextThread = (volatile PCB*)Scheduler::get();

	if(PCB::runningThread->state == PCB::READY && PCB::runningThread != PCB::idleThread){
		Scheduler::put((PCB*)PCB::runningThread);
	}
	static volatile unsigned nextSS,nextSP;
	static volatile unsigned oldSS, oldSP;
	if(nextThread == 0){//nema spremnih niti u scheduleru, idem u idle
		nextThread = (volatile PCB*)PCB::idleThread; // mozda uraditi cast u static volatile
	}

#ifndef BCC_BLOCK_IGNORE
asm{
	mov oldSS, ss;
	mov oldSP, sp;
}
#endif
	PCB::runningThread->ss = oldSS;
	PCB::runningThread->sp = oldSP;
	nextSS = nextThread->ss;
	nextSP = nextThread->sp;
	PCB::runningThread = nextThread;
#ifndef BCC_BLOCK_IGNORE
asm{
	mov sp, nextSP;
	mov ss, nextSS;

}
#endif
PCB::runningThread->timeCnt = 0;

}

IntrRoutine System::replaceRoutine(IVTno entry, IntrRoutine routine){
lock;
	unsigned routineCS, routineIP;
#ifndef BCC_BLOCK_IGNORE
	routineCS = FP_SEG(routine);
	routineIP = FP_OFF(routine);
#endif
	unsigned oldRoutineCS, oldRoutineIP;
	IntrRoutine oldRoutine;
	entry*=4;
#ifndef BCC_BLOCK_IGNORE
	asm {
	 push es
	 push ax
	 push bx
	 mov ax,0
	 mov es,ax
	 mov bx, word ptr entry
	 //pamti staru
	 mov ax, word ptr es:bx+2
	 mov word ptr oldRoutineCS, ax
	 mov ax, word ptr es:bx
	 mov word ptr oldRoutineIP, ax

	//postavlja novu
	 mov ax, word ptr routineCS
	 mov word ptr es:bx+2, ax
	 mov ax, word ptr routineIP
	 mov word ptr es:bx, ax

	 pop bx
	 pop ax
	 pop es
	 }
	oldRoutine = (IntrRoutine)MK_FP(oldRoutineCS, oldRoutineIP);
#endif
unlock;
	return oldRoutine;
}


void tick(){}
