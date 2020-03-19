#pragma once

#include "types.h"
#include "GBRegs.h"

class SingleDMA
{
public:
	UInt16 irpmask;

	GBReg SAD;
	GBReg DAD;
	GBReg CNT_L;

	GBReg Dest_Addr_Control;
	GBReg Source_Adr_Control;
	GBReg DMA_Repeat;
	GBReg DMA_Transfer_Type;
	GBReg Game_Pak_DRQ;
	GBReg DMA_Start_Timing;
	GBReg IRQ_on;
	GBReg DMA_Enable;

	bool hasDRQ = false;

	byte dest_Addr_Control;
	byte source_Adr_Control;
	bool dMA_Repeat;
	bool dMA_Transfer_Type;
	bool game_Pak_DRQ;
	byte dMA_Start_Timing;
	bool iRQ_on;
	bool dMA_Enable;

	bool running;
	bool waiting;
	bool first;
	UInt32 addr_source;
	UInt32 addr_target;
	UInt32 count;
	UInt32 fullcount;
	int totalTicks;
	int waitTicks;
	bool skipdebugout;

	SingleDMA();
	SingleDMA(UInt16 irpmask,
		GBReg SAD, GBReg DAD, GBReg CNT_L,
		GBReg Dest_Addr_Control, GBReg Source_Adr_Control, GBReg DMA_Repeat, GBReg DMA_Transfer_Type,
		GBReg Game_Pak_DRQ, GBReg DMA_Start_Timing, GBReg IRQ_on, GBReg DMA_Enable, bool hasDRQ);
};

class Dma
{
public:
	SingleDMA DMAs[4];

	bool dma_active = false;
	bool delayed = false;

	UInt32 debug_dmatranfers = 0;
	int cpuDmaCount = 0;

	bool new_hblank = false;
	bool new_vblank = false;

	UInt32 last_dma_value = 0;
	int last_dma_index= 0;

	void reset();
	void set_settings(int index);
	void check_run(int index);
	void work();
	void request_audio(uint audioindex);
};
extern Dma DMA;