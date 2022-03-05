/*
 * semaphor.h
 *
 *  Created on: Jan 28, 2022
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_
#include "kernelS.h"

typedef unsigned int Time;

class KernelSem;
class Semaphore {
public:
 Semaphore (int init=1);
virtual ~Semaphore ();
 virtual int wait (Time maxTimeToWait);
 virtual void signal();
 int val () const; // Returns the current value of the semaphore
private:
 KernelSem* myImpl;
};



#endif


