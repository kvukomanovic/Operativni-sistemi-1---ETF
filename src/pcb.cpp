/*
 * pcb.cpp
 *
 *  Created on: Jan 5, 2022
 *      Author: OS1
 */

#include <dos.h>
#include "pcb.h"
#include "list.h"
#include <DOS.H>

#include "sysFunc.h"

volatile PCB* PCB::running=0;
list* PCB::allPCBs=new list();
PCB* PCB::mainPCB=0;
PCB* PCB::idlePCB=0;


PCB::PCB(Thread* thread, StackSize stackSize, Time timeSlice){
	unsigned* st=new unsigned[stackSize];
	st[stackSize-1]=FP_SEG(thread);
	st[stackSize-2]=FP_OFF(thread);
	st[stackSize-5]=0x200;
	st[stackSize-6]=FP_SEG(wrapper);
	st[stackSize-7]=FP_OFF(wrapper);
	ss=FP_SEG(st+stackSize-16);
	sp=FP_OFF(st+stackSize-16);
	bp=sp;
	st[stackSize-16]=bp;
	stack=st;
	quantum=timeSlice;
	state=notstarted;
	myThread=thread;
	waiting=new list();
	allPCBs->add(this);
	myID=thread->getId();
	unblockedBySignal=0;
};
void PCB::wrapper(){
	PCB::running->myThread->run();
	lock();
	if (!PCB::running->waiting->isEmpty()){
		PCB::running->waiting->releaseAllWaiting();
	}
	PCB::running->state=finished;
	unlock();
	dispatch();
};

PCB::PCB(){
	waiting=new list();
	stack=0;
	ss=0;sp=0;
	state=ready;
	quantum=1;bp=0;
	allPCBs->add(this);
	myID=0;
	unblockedBySignal=0;
}
PCB::PCB(int i){
	if (i!=-1) return;
	unsigned stackSize=4096/sizeof(unsigned);
	unsigned* st=new unsigned[stackSize];

		st[stackSize-5]=0x200;
		st[stackSize-6]=FP_SEG(idleWrapper);
		st[stackSize-7]=FP_OFF(idleWrapper);
		ss=FP_SEG(st+stackSize-16);
		sp=FP_OFF(st+stackSize-16);
		bp=sp;
		stack=st;
		quantum=1;
		state=ready;
		myThread=0;
		waiting=0;
		myID=-1;
};

void PCB::idleWrapper(){
	while(1){	}
};

PCB::~PCB(){

	delete [] stack;
	delete stack;
	delete waiting;
	allPCBs->remove(this);
};
