#include "pcb.h"

#include <stdlib.h>
#include <dos.h>

#include "system.h"
const State PCB::NEW = 1;
const State PCB::READY = 2;
const State PCB::BLOCKED = 3;
const State PCB::DONE = 4;

ID PCB::globalId = -1;

volatile PCB* PCB::runningThread = 0;
PCB* PCB::idleThread = 0;
PCB* PCB::sysThread = 0;

Queue* PCB::globalQueue = new Queue();

PCB::PCB(Thread* thread, StackSize stackSize, Time timeSlice){
lock;
	myId = globalId++;
	this->stackSize = stackSize;
	stack = 0;
	this->timeSlice = timeSlice;
	timeCnt = 0;
	this->thread = thread;
	waitingQueue = new Queue();
	ss = sp = 0;
	state = PCB::NEW;
	signalType = 0;
	if(myId>0){
		globalQueue->add(this);
	}
unlock;
}

PCB::~PCB(){
lock;
	globalQueue->removeByID(this->myId);
	delete stack;
	delete waitingQueue;
unlock;
}

void PCB::createStack(){
lock;
	//static volatile unsigned char *tempStack;
	stack = new unsigned char[stackSize];
	//stack = (unsigned char *)tempStack;
	static volatile unsigned oldSS, oldSP;
	static volatile unsigned tempSS, tempSP;
	static volatile unsigned tempCS, tempIP;
#ifndef BCC_BLOCK_IGNORE
	tempSS = FP_SEG(stack + stackSize);
	tempSP = FP_OFF(stack + stackSize);
	tempCS = FP_SEG(&(System::wrapper));
	tempIP = FP_OFF(&(System::wrapper));

asm{
		mov oldSS, ss
		mov oldSP, sp
		mov ss, tempSS
		mov sp, tempSP
		pushf
		pop ax
		or ax, 0000001000000000b
		push ax
		mov ax, tempCS
		push ax
		mov ax, tempIP
		push ax
		mov ax, 0
		push ax
		push bx
		push cx
		push dx
		push es
		push ds
		push si
		push di
		push bp
		mov tempSP, sp
		mov ss, oldSS
		mov sp, oldSP
}
#endif
ss = tempSS;
sp = tempSP;

unlock;
}


