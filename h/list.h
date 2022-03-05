/*
 * list.h
 *
 *  Created on: Jan 5, 2022
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_

#include <pcb.h>
#include "thread.h"

class Elem {
public:
 friend class PCB;
	PCB* pcb;
	Elem* next;
	Elem(PCB* pcb);
};

class list{
public:
	Elem* head;
	void add(PCB* pcb);
	void remove(PCB* pcb);
	Thread* getThreadById(ID id);
	list();
	int isEmpty();
	void releaseAllWaiting();
	~list();
	void unblockTop();

};


#endif /* LIST_H_ */
