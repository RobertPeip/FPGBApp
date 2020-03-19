#pragma once

#include "types.h"
#include "GBRegs.h"

class SingleTimer
{
public:
	UInt16 irpmask;

	GBReg CNT_L;
	GBReg Prescaler;
	GBReg Count_up;
	GBReg Timer_IRQ_Enable;
	GBReg Timer_Start_Stop;

	Int32 value;
	Int32 prescalevalue;
	Int32 reload;
	bool on;
	bool startnow;
	bool stopnow;
	bool irp_on;
	bool countup;
	int prescale;

	UInt16 retval;

	SingleTimer();
	SingleTimer(UInt16 irpmask, GBReg CNT_L, GBReg Prescaler, GBReg Count_up, GBReg Timer_IRQ_Enable, GBReg Timer_Start_Stop);
};

class TIMER
{
public:
	SingleTimer timers[4];

	void reset();
	void set_reload(int index);
	void set_settings(int index);
	void work();
};
extern TIMER Timer;