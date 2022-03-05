#include "timeList.h"
#include "schedule.h"
#include "pcb.h"
#include "sysFunc.h"


class ElemT {
public:
 friend class PCB;
	PCB* pcb;
	ElemT* next;
	unsigned int time;
	ElemT(PCB* pcb);
};

ElemT::ElemT(PCB* pcb){
	this->pcb=pcb;
};

timeList::timeList(){
	head=0;
};

int timeList::isEmpty(){
	if (head==0) return 1; else return 0;
};

void timeList::addWithTime(PCB* pcb, unsigned int time){
	lock();
	asm pushf;
	asm cli;
	ElemT* newElem=new ElemT(pcb);
	PCB::running->unblockedBySignal=0;
	//pcb->unblockedBySignal=0;
		if (head==0) {head=newElem; newElem->time=time; newElem->next=0;}
		else if (head->time<=time){
			ElemT* pom=head->next,*prev=head; int t=head->time;
			while(pom!=0 && t+pom->time<=time){
				t+=pom->time;
				prev=pom;
				pom=pom->next;
			}
			newElem->time=time-t;
			newElem->next=prev->next;
			prev->next=newElem;
			if (pom!=0) pom->time-=newElem->time;
		}
		else if (head->time>time){
			newElem->time=time;
			newElem->next=head;
			head->time-=time;
			head=newElem;
		}
		asm popf;
		unlock();
};
void timeList::updateTimeAndRelease(){
	if (isEmpty()==1) return;
	ElemT* pom=head,*prev=0;
	pom->time=pom->time-1;
	while(pom!=0 && pom->time==0) {
		pom->pcb->state=ready;
		Scheduler::put(pom->pcb);
		pom->pcb->unblockedBySignal=0;
		prev=pom;
		pom=pom->next;
		delete prev;
	}
	head=pom;
};

void timeList::unblockOne(){
	asm{
		pushf
		cli
	};
	if (!isEmpty()){
		ElemT* pom=head;
		head=head->next;
		if (head!=0) head->time+=pom->time;
		pom->pcb->state=ready;
		Scheduler::put(pom->pcb);
		pom->pcb->unblockedBySignal=1;
		delete pom;
	}
	/**/
/*	if (!isEmpty()){
		ElemT* pom=head,*prev=0;
		while(pom->next!=0){
			prev=pom;
			pom=pom->next;
		}
		if (prev!=0){
			prev->next=pom->next;
		} else head=0;
		pom->pcb->state=ready;
				Scheduler::put(pom->pcb);
				pom->pcb->unblockedBySignal=1;
				delete pom;

	}*/
	/*if (head==0) return;
	if(head->next == 0) {
	      head->pcb->state=ready;
	      Scheduler::put(head->pcb);
	      head->pcb->unblockedBySignal=1;
	      delete head;
	      head=0;
	    } else {
	      ElemT* temp = head;
	      while(temp->next->next != 0)
	        temp = temp->next;
	      ElemT* lastNode = temp->next;
	      temp->next = 0;
	      lastNode->pcb->state=ready;
	     	      Scheduler::put(lastNode->pcb);
	     	     lastNode->pcb->unblockedBySignal=1;
	     	      delete lastNode;
	    }*/

	asm popf;

};
/*void list::releaseAllWaiting(){
	Elem* pom=head,*prev=0;
	while(pom!=0){
		pom->pcb->state=ready;
		Scheduler::put(pom->pcb);
		prev=pom;
		pom=pom->next;
		//pom->next=0;
		delete prev;
	}
	head=0;
};*/

timeList::~timeList(){
	lock();
	ElemT *pom=head,*prev=0;
	while (pom!=0){
		prev=pom;
		pom=pom->next;
		delete prev;
	}
	unlock();
};

