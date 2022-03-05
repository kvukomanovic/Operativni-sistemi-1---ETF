#include "semList.h"
#include "sysFunc.h"
#include "dos.h"

ElemS* myImplList::head=0;

void myImplList::add(KernelSem* sem){
	lock();
	asm{ pushf;cli;}
		ElemS* newElem=new ElemS(sem);
		newElem->next=myImplList::head;
		myImplList::head=newElem;
	asm popf;
	unlock();
};

void myImplList::remove(KernelSem* sem){
	lock();
	asm{ pushf;cli;}
		ElemS* pom=myImplList::head,*prev=0;
			while(pom!=0 && pom->sem!=sem){ prev=pom; pom=pom->next; }
			if (pom==0) return;
			if (prev==0) myImplList::head=myImplList::head->next;
			else {prev->next=pom->next; }
			delete pom;
	asm popf;
	unlock();
};

void myImplList::updateTimeAndRelease(){
	if (myImplList::head==0) return;
	ElemS* pom=myImplList::head;
	while(pom!=0){
		pom->sem->waitingList->updateTimeAndRelease();
		pom=pom->next;
	}
};
