#include "event.h"
#include "kernelev.h"
#include "pcb.h"
#include "schedule.h"

Event::Event(IVTNo ivtno){
	asm{ pushf;cli;}
	myImpl=new KernelEv(ivtno);
	asm popf;
};

void Event::wait(){
	myImpl->wait();
};
void Event::signal(){
	myImpl->signal();
};

Event::~Event(){
	asm{ pushf;cli;}
	delete myImpl;
	asm popf;
};


