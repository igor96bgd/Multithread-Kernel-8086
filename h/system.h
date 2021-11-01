#ifndef _system_h_
#define _system_h_

#include <dos.h>
#include "thread.h"
#include "semaphor.h"

#define lock asm pushf;\
             asm cli;

#define unlock asm popf;

typedef void (interrupt *IntrRoutine)();
typedef unsigned int IVTno;

class PCB;
class Thread;

class System{
private:
	static volatile int explicit_dispatch;

	static void wrapper();
	static void interrupt timer();
	static void interrupt (*oldRoutine)();
	static IntrRoutine replaceRoutine(IVTno entry, IntrRoutine routine);
public:
	static void dispatch();
	static void init();
	static int terminate();

	friend class PCB;

};

extern void tick();

extern int syncPrintf(const char *format, ...);

#endif
