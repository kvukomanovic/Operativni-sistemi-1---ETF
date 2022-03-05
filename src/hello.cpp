/*
 * hello.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: OS1
 */

#include <iostream.h>
#include <dos.h>
#include <stdio.h>
//#include "test10.h"
#include "sysFunc.h"

extern int userMain(int argc,char* argv[]);

int main(int argc, char* argv[])
{
	init();
	int n;
	n=userMain(argc,argv);

	restore();
	return n;
return 0;
}

