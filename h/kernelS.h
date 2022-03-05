
#ifndef KERNELS_H_
#define KERNELS_H_

#include "list.h"
#include "timeList.h"

class KernelSem {
public:
	int value;
	list* blockedList;
	timeList* waitingList;
	void signal();
	int wait(unsigned int time);
	int val() const;
	KernelSem(int init);
	~KernelSem();


};



#endif
