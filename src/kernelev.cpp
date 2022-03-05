#include "kernelev.h"
#include "pcb.h"
#include "ivtentry.h"
#include "schedule.h"
#include "thread.h"

KernelEv::KernelEv(IVTNo ivtno){
	lock();
	asm{ pushf;cli;}
	myThread=(PCB*)PCB::running;
	IVTEntry::ivtes[ivtno]->setMyImpl(this);
	IVTEntry::ivtes[ivtno]->init();
	blocked=0;
	value=0;
	No=ivtno;

	asm popf;
	unlock();
};

void KernelEv::wait(){
	lock();
	if (PCB::running!=myThread) { unlock(); return;}
	asm{pushf;cli};
	if (value==0){
	PCB::running->state=waiting;
	blocked=(PCB*)PCB::running;
	asm popf;
	unlock();
	dispatch();
	} else {
		value=0;
		asm popf;
		unlock();
	//	asm sti;
	}

};

void KernelEv::signal(){
	if (blocked==0) value=1;
	else {
		blocked->state=ready;
		Scheduler::put(blocked);
		blocked=0;
	}

};

KernelEv::~KernelEv(){
	asm { pushf; cli;}
	/*if (blocked!=0){
		//Thread* t=Thread::getThreadById(blocked->myID);
		//t->waitToComplete();
		//blocked->state=ready;
		//Scheduler::put(blocked);
	}*/
	//asm cli;
	IVTEntry::ivtes[No]->removeMyImpl();
	IVTEntry::ivtes[No]->restore();
	asm popf;
};
