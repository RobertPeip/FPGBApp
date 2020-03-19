#pragma once
#include <queue>
using namespace std;

#include "types.h"
#include "GBRegs.h"

class SingleSoundDMA
{
public:
	GBReg Enable_RIGHT;
	GBReg Enable_LEFT;
	GBReg Timer_Select;
	GBReg Reset_FIFO;

	byte timerindex;
	std::queue<byte> fifo;
	bool any_on;

	std::queue<byte> outfifo;
	int tickcount;

	SingleSoundDMA();
	SingleSoundDMA(GBReg Enable_RIGHT, GBReg Enable_LEFT, GBReg Timer_Select, GBReg Reset_FIFO);

	void work();
};

class SOUNDDMA
{
public:
	SingleSoundDMA soundDMAs[2];

	void reset();
	void timer_overflow(uint timerindex);
	void write_SOUNDCNT_H();
	void fill_fifo(int index, UInt32 value, bool dwaccess);
};
extern SOUNDDMA SoundDMA;