/*
 * thread.cpp
 *
 *  Created on: Jan 5, 2022
 *      Author: OS1
 */
#include "thread.h"
#include "pcb.h"
#include "schedule.h"
#include "iostream.h"
#include "sysFunc.h"

ID Thread::globalID=0;

Thread::Thread(StackSize stackSize, Time timeSlice){
	myID=++globalID;
	if (stackSize>65536) stackSize=65536;
	stackSize/=sizeof(unsigned);
	myPCB=new PCB(this,stackSize,timeSlice);
};
Thread::~Thread(){
//	waitToComplete();
	lock();
	delete myPCB;
	unlock();
};

void Thread::start(){
	lock();
	if(myPCB->state == notstarted){
	myPCB->state=ready;
	Scheduler::put(myPCB);
	}
	unlock();
};

void Thread::waitToComplete(){
	asm{ pushf;cli;}
	if ((PCB::running==this->myPCB)
		|| (this->myPCB->state==finished)
		|| (this->myPCB->state==notstarted)) {asm popf; return;}
	this->myPCB->waiting->add((PCB*)PCB::running);
	PCB::running->state=blocked;
	asm popf;
	dispatch();
};
ID Thread::getId(){
	return this->myID;
};
ID Thread::getRunningId(){
	return PCB::running->myID;
};
Thread* Thread::getThreadById(ID id){
 return PCB::allPCBs->getThreadById(id);
};
