#include "Timer.h"
#include "IRP.h"
#include "CPU.h"
#include "gameboy.h"
#include "SoundDMA.h"

TIMER Timer;

SingleTimer::SingleTimer()
{
}

SingleTimer::SingleTimer(UInt16 irpmask, GBReg CNT_L, GBReg Prescaler, GBReg Count_up, GBReg Timer_IRQ_Enable, GBReg Timer_Start_Stop)
{
	this->irpmask = irpmask;

	this->CNT_L = CNT_L;
	this->Prescaler = Prescaler;
	this->Count_up = Count_up;
	this->Timer_IRQ_Enable = Timer_IRQ_Enable;
	this->Timer_Start_Stop = Timer_Start_Stop;

	on = false;
	startnow = false;
	stopnow = false;
	irp_on = false;
	reload = 0;
	value = -1;
	prescale = 0x10000;
	retval = 0;
	prescalevalue = 0;
	countup = false;
}

void TIMER::reset()
{
	timers[3] = SingleTimer(IRP.IRPMASK_Timer_3, GBRegs.Sect_timer.TM3CNT_L, GBRegs.Sect_timer.TM3CNT_H_Prescaler, GBRegs.Sect_timer.TM3CNT_H_Count_up, GBRegs.Sect_timer.TM3CNT_H_Timer_IRQ_Enable, GBRegs.Sect_timer.TM3CNT_H_Timer_Start_Stop);
	timers[2] = SingleTimer(IRP.IRPMASK_Timer_2, GBRegs.Sect_timer.TM2CNT_L, GBRegs.Sect_timer.TM2CNT_H_Prescaler, GBRegs.Sect_timer.TM2CNT_H_Count_up, GBRegs.Sect_timer.TM2CNT_H_Timer_IRQ_Enable, GBRegs.Sect_timer.TM2CNT_H_Timer_Start_Stop);
	timers[1] = SingleTimer(IRP.IRPMASK_Timer_1, GBRegs.Sect_timer.TM1CNT_L, GBRegs.Sect_timer.TM1CNT_H_Prescaler, GBRegs.Sect_timer.TM1CNT_H_Count_up, GBRegs.Sect_timer.TM1CNT_H_Timer_IRQ_Enable, GBRegs.Sect_timer.TM1CNT_H_Timer_Start_Stop);
	timers[0] = SingleTimer(IRP.IRPMASK_Timer_0, GBRegs.Sect_timer.TM0CNT_L, GBRegs.Sect_timer.TM0CNT_H_Prescaler, GBRegs.Sect_timer.TM0CNT_H_Count_up, GBRegs.Sect_timer.TM0CNT_H_Timer_IRQ_Enable, GBRegs.Sect_timer.TM0CNT_H_Timer_Start_Stop);
}

void TIMER::set_reload(int index)
{
	timers[index].reload = (int)timers[index].CNT_L.read();
}

void TIMER::set_settings(int index)
{
	if (timers[index].Timer_Start_Stop.on() && !timers[index].on && !gameboy.loading_state)
	{
		timers[index].startnow = true;
	}
	else if (timers[index].on && !timers[index].Timer_Start_Stop.on() && !gameboy.loading_state)
	{
		timers[index].stopnow = true;
	}
	timers[index].on = timers[index].Timer_Start_Stop.on();
	if (timers[index].on)
	{
		timers[index].irp_on = timers[index].Timer_IRQ_Enable.on();
		timers[index].countup = timers[index].Count_up.on();
		switch (timers[index].Prescaler.read())
		{
		case 0: timers[index].prescale = 1; break;
		case 1: timers[index].prescale = 64; break;
		case 2: timers[index].prescale = 256; break;
		case 3: timers[index].prescale = 1024; break;
		}
	}
}

void TIMER::work()
{
	// must save here, as dma may reset to zero
	int cputicks = CPU.newticks;

	for (int i = 0; i < 4; i++)
	{
		if (timers[i].startnow)
		{
			timers[i].startnow = false;
			timers[i].value = timers[i].reload;
			timers[i].retval = (UInt16)timers[i].value;
		}
		else if (timers[i].on || timers[i].stopnow)
		{
			timers[i].stopnow = false;

			if (!timers[i].countup || i == 0)
			{
				if (timers[i].prescale == 1)
				{
					timers[i].value += cputicks;
				}
				else
				{
					timers[i].prescalevalue += cputicks;
					while (timers[i].prescalevalue >= timers[i].prescale)
					{
						timers[i].prescalevalue -= timers[i].prescale;
						timers[i].value += 1;
					}
				}
			}

			while (timers[i].value >= 0x10000)
			{
				timers[i].value -= 0x10000;
				timers[i].value += timers[i].reload;

				if (i < 2)
				{
					SoundDMA.timer_overflow(i);
				}

				if (i < 3 && timers[i + 1].countup)
				{
					timers[i + 1].value++;
					timers[i + 1].retval = (UInt16)timers[i].value;
				}

				if (timers[i].irp_on)
				{
					IRP.set_irp_bit(timers[i].irpmask);
				}
			}

			if (!timers[i].countup)
			{
				timers[i].retval = (UInt16)timers[i].value;
			}
		}
	}
}