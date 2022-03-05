#include "sysFunc.h"
#include "pcb.h"
#include "schedule.h"
#include "timeList.h"
#include "iostream.h"

volatile int lockFlag;
volatile int context_on_demand=0;
volatile unsigned tsp,tss,tbp;
volatile unsigned counter=1;
volatile unsigned continuous=0;
void tick();

#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include "semList.h"
int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	lock();
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock();
		return res;
};
void interrupt timer(){
	if (context_on_demand==0) {
		if (!continuous) counter--;
	    myImplList::updateTimeAndRelease();
		tick();
		asm int 60h;
	}
	if ((counter==0 && continuous==0) || context_on_demand==1){

		 if (!lockFlag){
			context_on_demand=0;
			asm{
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp=tsp;
			PCB::running->ss=tss;
			PCB::running->bp=tbp;

			if (PCB::running->state == ready && PCB::running!=PCB::idlePCB ) Scheduler::put((PCB*)PCB::running);
			PCB::running=Scheduler::get();
			if (PCB::running==0) PCB::running=PCB::idlePCB;

			tsp=PCB::running->sp;
			tss=PCB::running->ss;
			tbp=PCB::running->bp;
			counter=PCB::running->quantum;
			if (counter==0) continuous=1;
			else continuous=0;

			asm{
				mov ss, tss
				mov sp, tsp
				mov bp, tbp
			}
		} else context_on_demand=1;
    }

	//if (!context_on_demand) {asm int 60h;}
};

void dispatch(){
	asm{ pushf;cli;}
	context_on_demand=1;
	timer();
	asm popf;
};

volatile unsigned oldTimerOFF,oldTimerSEG;

void init(){
	PCB::mainPCB= new PCB();
	PCB::running=PCB::mainPCB;
	PCB::idlePCB=new PCB(-1);

	asm{
			cli
			push es
			push ax

			mov ax,0   //  ; inicijalizuje rutinu za tajmer
			mov es,ax

			mov ax, word ptr es:0022h //; pamti staru rutinu
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0022h, seg timer	 //postavlja
			mov word ptr es:0020h, offset timer //novu rutinu

			mov ax, oldTimerSEG	 //	postavlja staru rutinu
			mov word ptr es:0182h, ax //; na int 60h
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es
			sti
		}


};

void restore(){
	asm {
			cli
			push es
			push ax

			mov ax,0
			mov es,ax


			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
			sti
		}
	delete PCB::idlePCB->stack;
	delete PCB::idlePCB;
	delete PCB::mainPCB;
};
void lock(){
	lockFlag++;
};

void unlock(){
	--lockFlag;
	if(context_on_demand && lockFlag==0) dispatch();
};
//void lockAsm{asm{pushf; cli;}};
//void unlockAsm{asm{popf;}};
