#pragma once

#include "types.h"

//#define prefetch_off;

class BUSTiming
{
public:
	const byte gamepakRamWaitState[4] = { 4, 3, 2, 8 };
	const byte gamepakWaitState[4] = { 4, 3, 2, 8 };
	const byte gamepakWaitState0[2] = { 2, 1 };
	const byte gamepakWaitState1[2] = { 4, 1 };
	const byte gamepakWaitState2[2] = { 8, 1 };

	bool busPrefetchEnable;
	int busPrefetchCount;

	byte memoryWait[16];
	byte memoryWait32[16];
	byte memoryWaitSeq[16];
	byte memoryWaitSeq32[16];

	void reset();
	void update(UInt16 value);
	int dataTicksAccess16(UInt32 address, int cycleadd); // DATA 8/16bits NON SEQ
	int dataTicksAccess32(UInt32 address, int cycleadd); // DATA 32bits NON SE
	int dataTicksAccessSeq32(UInt32 address, int cycleadd); // DATA 32bits SEQ
	int codeTicksAccess16(UInt32 address); // THUMB NON SEQ
	int codeTicksAccess32(UInt32 address); // ARM NON SEQ
	int codeTicksAccessSeq16(UInt32 address); // THUMB SEQ
	int codeTicksAccessSeq32(UInt32 address); // ARM SEQ
};
extern BUSTiming BusTiming;