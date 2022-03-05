#ifndef KERNELEV_H_
#define KERNELEV_H_
#include "pcb.h"
#include "event.h"

class KernelEv{
public:
	PCB* myThread;
	PCB* blocked;
	volatile int value;
	IVTNo No;
	KernelEv(IVTNo ivtno);
	~KernelEv();
	void wait();
	void signal();
	void setvectt();
};


#endif /* KERNELEV_H_ */
