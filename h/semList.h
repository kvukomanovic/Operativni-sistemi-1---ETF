/*
 * semList.h
 *
 *  Created on: Jan 30, 2022
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_


#include "semaphor.h"

class ElemS{
public:
	KernelSem *sem;
	ElemS* next;
	ElemS(KernelSem* sem):sem(sem),next(0){};
};

class myImplList{
public:
	static ElemS* head;
	static void add(KernelSem* sem);
	static void remove(KernelSem* sem);
	static void updateTimeAndRelease();
};


#endif /* SEMLIST_H_ */
