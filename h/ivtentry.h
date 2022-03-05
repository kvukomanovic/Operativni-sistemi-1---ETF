#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"
#include "sysFunc.h"

#define PREPAREENTRY(ivtNo , flag) \
	void interrupt intr##ivtNo(...); \
	IVTEntry entry##ivtNo(ivtNo, intr##ivtNo); \
	void interrupt intr##ivtNo(...) \
	{ \
		entry##ivtNo.signal(); \
		if (flag==1) \
			entry##ivtNo.callOld();\
			dispatch();\
	}

typedef void interrupt (*pInterrupt)(...);

class IVTEntry{
public:
	static IVTEntry* ivtes[];
	IVTNo No;
	KernelEv* myImpl;
	pInterrupt oldInterrupt;
	pInterrupt newInterrupt;
	void signal();
	IVTEntry(IVTNo No,pInterrupt p);
	~IVTEntry();
	void setMyImpl(KernelEv* kernelev);
	void removeMyImpl();
	void callOld();
	void restore();
	void init();
};


#endif /* IVTENTRY_H_ */
