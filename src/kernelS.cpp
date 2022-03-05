#include "kernelS.h"
#include "sysFunc.h"
#include "list.h"
#include "timeList.h"
#include "semList.h"
#include "iostream.h"
KernelSem::KernelSem(int init){
	lock();
	asm{pushf;cli;}
	value=init;
	blockedList=new list();
	waitingList=new timeList();
	myImplList::add(this);
	asm popf;
	unlock();
};

void KernelSem::signal(){

	asm{
	pushf
	cli
};
	lock();
	value++;
	//asm cli;
	if (!blockedList->isEmpty()) blockedList->unblockTop();
	else if (!waitingList->isEmpty()){ waitingList->unblockOne(); }
//	asm sti;
	unlock();
	asm popf;

};

int KernelSem:: val() const {
	return value;
};

int KernelSem::wait(unsigned int n){
	lock();
	asm{ pushf;cli;}
	int i;
	value--;
	if (value<0) {
		if (n==0) {
			PCB::running->state=blocked;
			blockedList->add((PCB*)PCB::running);
			//asm sti;
			asm popf;
			unlock();
			dispatch();
		} else {
			PCB::running->state=waiting;
			waitingList->addWithTime((PCB*)PCB::running,n);
			asm popf;
			unlock();
			//asm sti;
			dispatch();
		}
	}
	else {
		PCB::running->unblockedBySignal=1;
		//i=1;
		//asm sti;
		asm popf;
		unlock();
		//return 1;
	}
	asm pushf; asm cli;
	if (PCB::running->unblockedBySignal==0) value++;
	asm popf;
	return PCB::running->unblockedBySignal;
};


/*PCB::running->state=blocked;
					   blockedList->add((PCB*)PCB::running);
					   unlock();
					   dispatch();
					   return 1; //ako je odblokirana pomocu signal, vraca vrednost 1
		}
	else {
		unlock();
		return PCB::running->unblockedBySignal;}

	} else {
		 PCB::running->state=waiting;
		 timeList::addWithTime((PCB*)PCB::running,n);
		 unlock();
		 dispatch();
		 return 0;
	}*/
KernelSem::~KernelSem(){
	lock();
	asm{ pushf;cli;}
	while (value<0) signal();
	delete blockedList;
	delete waitingList;
	myImplList::remove(this);
	asm popf;
	unlock();
};

