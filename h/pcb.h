/*
 * pcb.h
 *
 *  Created on: Jan 5, 2022
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
#include "list.h"

#include "thread.h"
enum State {ready,blocked,finished,notstarted,waiting};
class list;
class PCB {
public:
	static volatile PCB* running;
	static PCB* idlePCB;
	static list* allPCBs;
	static PCB* mainPCB;

	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned* stack;
	unsigned quantum;
	State state;
	Thread* myThread;
	list* waiting;
	ID myID;
	int unblockedBySignal;

	PCB(Thread* thread, StackSize stackSize, Time timeSlice);
	PCB();
	PCB(int);
	static void wrapper();
	static void idleWrapper();
	~PCB();
};


#endif /* PCB_H_ */
