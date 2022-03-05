/*
 * sysFunc.h
 *
 *  Created on: Jan 30, 2022
 *      Author: OS1
 */

#ifndef SYSFUNC_H_
#define SYSFUNC_H_


void interrupt timer();
void dispatch();
void init();
void restore();
void lock();
void unlock();
int syncPrintf(const char *format, ...);

#endif /* SYSFUNC_H_ */
