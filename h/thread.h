/*
 * thread.h
 *
 *  Created on: Jan 5, 2022
 *      Author: OS1
 */

#ifndef THREAD_H_
#define THREAD_H_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

class PCB; // Kernel's implementation of a user's thread

class Thread {
public:
	static volatile unsigned lockFlag;
	static ID globalID;
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);
protected:
	friend class PCB;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice =
			defaultTimeSlice);
	virtual void run() {}
private:
	PCB* myPCB;
	ID myID;
};

void dispatch ();


#endif /* THREAD_H_ */
