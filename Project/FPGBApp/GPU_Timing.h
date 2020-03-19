#pragma once

#include "types.h"

enum class GPUState
{
	VISIBLE,
	HBLANK,
	VBLANK,
	VBLANKHBLANK
};

class GPUTiming
{
public:
	const int BITPOS_V_Blank_flag = 0;
	const int BITPOS_H_Blank_flag = 1;
	const int BITPOS_V_Counter_flag = 2;
	const int BITPOS_V_Blank_IRQ_Enable = 3;
	const int BITPOS_H_Blank_IRQ_Enable = 4;
	const int BITPOS_V_Counter_IRQ_Enable = 5;
	const int BITPOS_V_Count_Setting = 8;

	Int32 cycles;
	byte line;
	GPUState gpustate;

	UInt16 old_dispstat;

	void reset();
	void dispstat_write();
	void work();
	void restart_line();

private:
	void nextline();
};
extern GPUTiming GPU_Timing;