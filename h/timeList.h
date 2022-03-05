

#ifndef TIMELIST_
#define TIMELIST_

#include "pcb.h"

class ElemT;

class timeList{
public:
	ElemT* head;

	timeList();
	~timeList();
	int isEmpty();
	void addWithTime(PCB* pcb,unsigned int time);
	void updateTimeAndRelease();
	void unblockOne();
};





#endif /* TIMELIST_ */
