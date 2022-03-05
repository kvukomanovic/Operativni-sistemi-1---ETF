/*
 * list.cpp

 *
 *  Created on: Jan 11, 2022
 *      Author: OS1
 */
#include "list.h"
#include "schedule.h"
#include "sysFunc.h"

Elem::Elem(PCB* pcb){
	this->pcb=pcb;
	next=0;
};
void list::add(PCB* pcb){ //dodaje na kraj liste
	Elem* newElem=new Elem(pcb);
	pcb->unblockedBySignal=0;
	if (head==0) head=newElem;
	else {
		Elem* pom=head;
		while(pom->next!=0){
			pom=pom->next;
		}
		pom->next=newElem;
	}
};

void list::remove(PCB* pcb){
	Elem* pom=head,*prev=0;
	while(pom!=0 && pom->pcb!=pcb){ prev=pom; pom=pom->next; }
	if (pom==0) return;
	if (prev==0) head=head->next;
	else {prev->next=pom->next; }
	delete pom;

};
list::list(){
	head=0;
};
Thread* list::getThreadById(ID id){
	lock();
 Elem* pom=head;
 while (pom!=0 && pom->pcb->myThread->getId()!=id){pom=pom->next;}
 if (pom==0){unlock(); return 0;}
 else {unlock();return pom->pcb->myThread;}
};

int list::isEmpty(){
	if (head==0) return 1; else return 0;
};
void list::releaseAllWaiting(){
	lock();
	if (isEmpty()) {unlock(); return;}
	Elem* pom=head,*prev=0;
	while(pom!=0){
		pom->pcb->state=ready;
		prev=pom;
		pom=pom->next;
		Scheduler::put(prev->pcb);
		delete prev;
	}
	head=0;
	unlock();
};
list::~list(){
	releaseAllWaiting();
};

void list::unblockTop(){
	asm{pushf;cli;}
	if (head==0) {asm sti; return;}
	head->pcb->state=ready;
	head->pcb->unblockedBySignal=1;
	Scheduler::put(head->pcb);
	Elem* prev=head;
	head=head->next;
	delete prev;
	asm popf;
};



