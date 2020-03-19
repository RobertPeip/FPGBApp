#include <algorithm>
using namespace std;

#include "BusTiming.h"
#include "CPU.h"
#include "GBRegs.h"
#include "GPU_Timing.h"

BUSTiming BusTiming;

void BUSTiming::reset()
{
	byte memoryWait_new[] = { 0, 0, 2, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 0 }; std::copy(std::begin(memoryWait_new), std::end(memoryWait_new), std::begin(memoryWait));
	byte memoryWait32_new[] = { 0, 0, 5, 0, 0, 1, 1, 0, 7, 7, 9, 9, 13, 13, 4, 0 }; std::copy(std::begin(memoryWait32_new), std::end(memoryWait32_new), std::begin(memoryWait32));
	byte memoryWaitSeq_new[] = { 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 4, 4, 8, 8, 4, 0 }; std::copy(std::begin(memoryWaitSeq_new), std::end(memoryWaitSeq_new), std::begin(memoryWaitSeq));
	byte memoryWaitSeq32_new[] = { 0, 0, 5, 0, 0, 1, 1, 0, 5, 5, 9, 9, 17, 17, 4, 0 }; std::copy(std::begin(memoryWaitSeq32_new), std::end(memoryWaitSeq32_new), std::begin(memoryWaitSeq32));

	bool busPrefetchEnable = false;
	int busPrefetchCount = 0;
}

void BUSTiming::update(UInt16 value)
{
	memoryWait[0x0e] = memoryWaitSeq[0x0e] = gamepakRamWaitState[value & 3];

	memoryWait[0x08] = memoryWait[0x09] = gamepakWaitState[(value >> 2) & 3];
	memoryWaitSeq[0x08] = memoryWaitSeq[0x09] = gamepakWaitState0[(value >> 4) & 1];

	memoryWait[0x0a] = memoryWait[0x0b] = gamepakWaitState[(value >> 5) & 3];
	memoryWaitSeq[0x0a] = memoryWaitSeq[0x0b] = gamepakWaitState1[(value >> 7) & 1];

	memoryWait[0x0c] = memoryWait[0x0d] = gamepakWaitState[(value >> 8) & 3];
	memoryWaitSeq[0x0c] = memoryWaitSeq[0x0d] = gamepakWaitState2[(value >> 10) & 1];

	for (int i = 8; i < 15; i++)
	{
		memoryWait32[i] = (byte)(memoryWait[i] + memoryWaitSeq[i] + 1);
		memoryWaitSeq32[i] = (byte)(memoryWaitSeq[i] * 2 + 1);
	}

	if ((value & 0x4000) == 0x4000)
	{
		busPrefetchEnable = true;
	}
	else
	{
		busPrefetchEnable = false;
	}
}

int BUSTiming::dataTicksAccess16(UInt32 address, int cycleadd) // DATA 8/16bits NON SEQ
{
	UInt32 addr = (address >> 24) & 15;
	int value = memoryWait[addr];
#ifdef prefetch_off
	return value;
#endif

	if (addr >= 0x08)
	{
		busPrefetchCount = 0;
	}
	else if (busPrefetchEnable)
	{
		int seq = memoryWaitSeq[(CPU.regs[15] >> 24) & 15];
		int non = memoryWait[(CPU.regs[15] >> 24) & 15];
		int max = memoryWaitSeq[(CPU.regs[15] >> 24) & 15] * 8;
		busPrefetchCount = min(max, busPrefetchCount + value + (non - seq - 1) + cycleadd);
	}

	return value;
}

int BUSTiming::dataTicksAccess32(UInt32 address, int cycleadd) // DATA 32bits NON SE
{
	UInt32 addr = (address >> 24) & 15;
	int value = memoryWait32[addr];
#ifdef prefetch_off
	return value;
#endif

	if (addr == 6 && GPU_Timing.gpustate == GPUState::VISIBLE)
	{
		if (address < 0x06010000 && GPU_Timing.cycles < 980)
		{
			if ((byte)GBRegs.Sect_display.DISPCNT_BG_Mode.read() < 3)
			{
				value += 980 - GPU_Timing.cycles;
			}
		}
	}

	if (addr >= 0x08)
	{
		busPrefetchCount = 0;
	}
	else if (busPrefetchEnable)
	{
		int seq = memoryWaitSeq[(CPU.regs[15] >> 24) & 15];
		int non = memoryWait[(CPU.regs[15] >> 24) & 15];
		int max = memoryWaitSeq[(CPU.regs[15] >> 24) & 15] * 8;
		busPrefetchCount = min(max, busPrefetchCount + value + (non - seq - 1) + cycleadd);
	}

	return value;
}

int BUSTiming::dataTicksAccessSeq32(UInt32 address, int cycleadd) // DATA 32bits SEQ
{
	UInt32 addr = (address >> 24) & 15;
	int value = memoryWaitSeq32[addr];
#ifdef prefetch_off
	return value;
#endif

	if (addr >= 0x08)
	{
		busPrefetchCount = 0;
		//busPrefetch = false;
	}
	else if (busPrefetchEnable)
	{
		int seq = memoryWaitSeq[(CPU.regs[15] >> 24) & 15];
		int non = memoryWait[(CPU.regs[15] >> 24) & 15];
		int max = memoryWaitSeq[(CPU.regs[15] >> 24) & 15] * 8;
		busPrefetchCount = min(max, busPrefetchCount + value + (non - seq - 1) + cycleadd);
	}

	return value;
}

int BUSTiming::codeTicksAccess16(UInt32 address) // THUMB NON SEQ
{
	UInt32 addr = (address >> 24) & 15;
#ifdef prefetch_off
	return memoryWait[addr];
#endif

	int value = memoryWait[addr];
	if ((addr >= 0x08) && (addr <= 0x0D) && busPrefetchCount > 0)
	{
		int subtract = min(busPrefetchCount, value);
		busPrefetchCount -= subtract;
		return (value - subtract);
	}
	else
	{
		busPrefetchCount = 0;
		return memoryWait[addr];
	}
}

int BUSTiming::codeTicksAccess32(UInt32 address) // ARM NON SEQ
{
	UInt32 addr = (address >> 24) & 15;
#ifdef prefetch_off
	return memoryWait32[addr];
#endif

	int value = memoryWait32[addr];
	if ((addr >= 0x08) && (addr <= 0x0D) && busPrefetchCount > 0)
	{
		int subtract = min(busPrefetchCount, value);
		busPrefetchCount -= subtract;
		return (value - subtract);
	}
	else
	{
		busPrefetchCount = 0;
		return memoryWait32[addr];
	}
}

int BUSTiming::codeTicksAccessSeq16(UInt32 address) // THUMB SEQ
{
	UInt32 addr = (address >> 24) & 15;
#ifdef prefetch_off
	return memoryWaitSeq[addr];
#endif

	int value = memoryWaitSeq[addr];
	if ((addr >= 0x08) && (addr <= 0x0D) && busPrefetchCount > 0)
	{
		int subtract = min(busPrefetchCount, value);
		busPrefetchCount -= subtract;
		return (value - subtract);
	}
	else
	{
		busPrefetchCount = 0;
		return memoryWaitSeq[addr];
	}
}

int BUSTiming::codeTicksAccessSeq32(UInt32 address) // ARM SEQ
{
	UInt32 addr = (address >> 24) & 15;
#ifdef prefetch_off
	return memoryWaitSeq32[addr];
#endif

	int value = memoryWaitSeq32[addr];
	if ((addr >= 0x08) && (addr <= 0x0D) && busPrefetchCount > 0)
	{
		int subtract = min(busPrefetchCount, value);
		busPrefetchCount -= subtract;
		return (value - subtract);
	}
	else
	{
		return memoryWaitSeq32[addr];
	}
}

