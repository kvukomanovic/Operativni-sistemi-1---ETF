#include "ivtentry.h"
#include "kernelev.h"
#include <dos.h>
#include "bios.h"

IVTEntry* IVTEntry::ivtes[256]={0};

IVTEntry::IVTEntry(IVTNo No, pInterrupt p){
	this->No=No;
	ivtes[No]=this;
	myImpl=0;
	newInterrupt=p;
};

void IVTEntry::setMyImpl(KernelEv* kernelev){
	asm{ pushf;cli;}
	myImpl=kernelev;
	asm popf;
};
void IVTEntry::removeMyImpl(){
	asm{ pushf;cli;}
	myImpl=0;
	asm popf;
};

void IVTEntry::callOld(){
	if (oldInterrupt){
	asm{ pushf;cli;}
	(*oldInterrupt)();
	asm popf;
	}
};

IVTEntry::~IVTEntry(){
	asm{ pushf;cli;}
/*#ifndef BCC_BLOCK_IGNORE
	setvect(No,oldInterrupt);
#endif
*/
	lock();
	//(*oldInterrupt)();

	//signal();
		ivtes[No]=0;
		myImpl=0;
	oldInterrupt=0;
	unlock();
	asm popf;

};

void IVTEntry::signal(){
	if (myImpl!=0) myImpl->signal();
};
void IVTEntry::restore(){
	asm{ pushf;cli;}
	(*oldInterrupt)();

	#ifndef BCC_BLOCK_IGNORE
		setvect(No,oldInterrupt);
	#endif
//	(*oldInterrupt)();
	asm popf;

};
void IVTEntry::init(){
	asm{ pushf;cli;}
	#ifndef BCC_BLOCK_IGNORE
		oldInterrupt= getvect(No);
		setvect(No,newInterrupt);
	#endif
		asm popf;
};
