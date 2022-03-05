#include "semaphor.h"
#include "semList.h"
#include "sysFunc.h"

Semaphore::Semaphore(int init){
	lock();
	asm{pushf;cli;}
	myImpl=new KernelSem(init);
	asm popf;
	unlock();
};
Semaphore::~Semaphore(){
	lock();
	asm{pushf;cli;}
	delete myImpl;
	asm popf;
	unlock();
};

void Semaphore::signal(){
	lock();
	myImpl->signal();
	unlock();
};

int Semaphore::wait(Time time){
	//lock();
	return myImpl->wait(time);
	//unlock();
};
int Semaphore::val() const {
	return myImpl->val();
};
