#include <algorithm>
using namespace std;

#include "DMA.h"
#include "gameboy.h"
#include "IRP.h"
#include "CPU.h"
#include "Memory.h"
#include "BusTiming.h"
#include "Timer.h"
#include "GPU_Timing.h"

Dma DMA;

SingleDMA::SingleDMA()
{
}

SingleDMA::SingleDMA(UInt16 irpmask,
	GBReg SAD, GBReg DAD, GBReg CNT_L,
	GBReg Dest_Addr_Control, GBReg Source_Adr_Control, GBReg DMA_Repeat, GBReg DMA_Transfer_Type,
	GBReg Game_Pak_DRQ, GBReg DMA_Start_Timing, GBReg IRQ_on, GBReg DMA_Enable, bool hasDRQ)
{
	this->irpmask = irpmask;

	this->SAD = SAD;
	this->DAD = DAD;
	this->CNT_L = CNT_L;

	this->Dest_Addr_Control = Dest_Addr_Control;
	this->Source_Adr_Control = Source_Adr_Control;
	this->DMA_Repeat = DMA_Repeat;
	this->DMA_Transfer_Type = DMA_Transfer_Type;
	this->Game_Pak_DRQ = Game_Pak_DRQ;
	this->DMA_Start_Timing = DMA_Start_Timing;
	this->IRQ_on = IRQ_on;
	this->DMA_Enable = DMA_Enable;

	this->hasDRQ = hasDRQ;

	this->dMA_Enable = false;
	running = false;
	waiting = false;
	waitTicks = 0;
}

void Dma::reset()
{
	new_hblank = false;
	new_vblank = false;

	dma_active = false;

	debug_dmatranfers = 0;

	GBReg empty;

	DMAs[3] = SingleDMA(IRP.IRPMASK_DMA_3,
		GBRegs.Sect_dma.DMA3SAD, GBRegs.Sect_dma.DMA3DAD, GBRegs.Sect_dma.DMA3CNT_L,
		GBRegs.Sect_dma.DMA3CNT_H_Dest_Addr_Control, GBRegs.Sect_dma.DMA3CNT_H_Source_Adr_Control, GBRegs.Sect_dma.DMA3CNT_H_DMA_Repeat, GBRegs.Sect_dma.DMA3CNT_H_DMA_Transfer_Type,
		GBRegs.Sect_dma.DMA3CNT_H_Game_Pak_DRQ, GBRegs.Sect_dma.DMA3CNT_H_DMA_Start_Timing, GBRegs.Sect_dma.DMA3CNT_H_IRQ_on, GBRegs.Sect_dma.DMA3CNT_H_DMA_Enable, true);

	DMAs[2] = SingleDMA(IRP.IRPMASK_DMA_1,
		GBRegs.Sect_dma.DMA2SAD, GBRegs.Sect_dma.DMA2DAD, GBRegs.Sect_dma.DMA2CNT_L,
		GBRegs.Sect_dma.DMA2CNT_H_Dest_Addr_Control, GBRegs.Sect_dma.DMA2CNT_H_Source_Adr_Control, GBRegs.Sect_dma.DMA2CNT_H_DMA_Repeat, GBRegs.Sect_dma.DMA2CNT_H_DMA_Transfer_Type,
		empty, GBRegs.Sect_dma.DMA2CNT_H_DMA_Start_Timing, GBRegs.Sect_dma.DMA2CNT_H_IRQ_on, GBRegs.Sect_dma.DMA2CNT_H_DMA_Enable, false);

	DMAs[1] = SingleDMA(IRP.IRPMASK_DMA_2,
		GBRegs.Sect_dma.DMA1SAD, GBRegs.Sect_dma.DMA1DAD, GBRegs.Sect_dma.DMA1CNT_L,
		GBRegs.Sect_dma.DMA1CNT_H_Dest_Addr_Control, GBRegs.Sect_dma.DMA1CNT_H_Source_Adr_Control, GBRegs.Sect_dma.DMA1CNT_H_DMA_Repeat, GBRegs.Sect_dma.DMA1CNT_H_DMA_Transfer_Type,
		empty, GBRegs.Sect_dma.DMA1CNT_H_DMA_Start_Timing, GBRegs.Sect_dma.DMA1CNT_H_IRQ_on, GBRegs.Sect_dma.DMA1CNT_H_DMA_Enable, false);

	DMAs[0] = SingleDMA(IRP.IRPMASK_DMA_0,
		GBRegs.Sect_dma.DMA0SAD, GBRegs.Sect_dma.DMA0DAD, GBRegs.Sect_dma.DMA0CNT_L,
		GBRegs.Sect_dma.DMA0CNT_H_Dest_Addr_Control, GBRegs.Sect_dma.DMA0CNT_H_Source_Adr_Control, GBRegs.Sect_dma.DMA0CNT_H_DMA_Repeat, GBRegs.Sect_dma.DMA0CNT_H_DMA_Transfer_Type,
		empty, GBRegs.Sect_dma.DMA0CNT_H_DMA_Start_Timing, GBRegs.Sect_dma.DMA0CNT_H_IRQ_on, GBRegs.Sect_dma.DMA0CNT_H_DMA_Enable, false);
}

void Dma::set_settings(int index)
{
	if (gameboy.loading_state)
	{
		return;
	}

	bool old_ena = DMAs[index].dMA_Enable;
	DMAs[index].dMA_Enable = DMAs[index].DMA_Enable.on();

	if (!DMAs[index].dMA_Enable)
	{
		DMAs[index].running = false;
		DMAs[index].waiting = false;
	}

	if (DMAs[index].dMA_Enable && !old_ena)
	{
		DMAs[index].dest_Addr_Control = (byte)DMAs[index].Dest_Addr_Control.read();
		DMAs[index].source_Adr_Control = (byte)DMAs[index].Source_Adr_Control.read();
		DMAs[index].dMA_Repeat = DMAs[index].DMA_Repeat.on();
		DMAs[index].dMA_Transfer_Type = DMAs[index].DMA_Transfer_Type.on();
		if (DMAs[index].hasDRQ)
		{
			DMAs[index].game_Pak_DRQ = DMAs[index].Game_Pak_DRQ.on();
			if (DMAs[index].game_Pak_DRQ)
			{
				//throw new Exception("gamepak drq?");
			}
		}
		else
		{
			DMAs[index].game_Pak_DRQ = false;
		}
		DMAs[index].dMA_Start_Timing = (byte)DMAs[index].DMA_Start_Timing.read();
		DMAs[index].iRQ_on = DMAs[index].IRQ_on.on();

		DMAs[index].addr_source = DMAs[index].SAD.read();
		DMAs[index].addr_target = DMAs[index].DAD.read();

		switch (index)
		{
		case 0: DMAs[index].addr_source &= 0x07FFFFFE; DMAs[index].addr_target &= 0x07FFFFFE; break;
		case 1: DMAs[index].addr_source &= 0x0FFFFFFE; DMAs[index].addr_target &= 0x07FFFFFE; break;
		case 2: DMAs[index].addr_source &= 0x0FFFFFFE; DMAs[index].addr_target &= 0x07FFFFFE; break;
		case 3: DMAs[index].addr_source &= 0x0FFFFFFE; DMAs[index].addr_target &= 0x0FFFFFFE; break;
		}

		if (DMAs[index].dMA_Transfer_Type)
		{
			DMAs[index].addr_source &= 0x0FFFFFFC;
			DMAs[index].addr_target &= 0x0FFFFFFC;
		}

		DMAs[index].count = DMAs[index].CNT_L.read();

		if (DMAs[index].count == 0)
		{
			DMAs[index].count = 0x4000;
			if (index == 3)
			{
				DMAs[index].count = 0x10000;
			}
		}
		DMAs[index].waiting = true;
		check_run(index, false);

		if (DMAs[index].dMA_Start_Timing == 3)
		{
			if (index == 1 || index == 2)
			{
				DMAs[index].count = 4;
				DMAs[index].dest_Addr_Control = 3;
			}
		}
	}
}

void Dma::check_run(int index, bool fromGPU)
{
	if (!fromGPU && DMAs[index].dMA_Start_Timing == 0 ||
		!fromGPU && DMAs[index].dMA_Start_Timing == 1 && new_vblank ||
		!fromGPU && DMAs[index].dMA_Start_Timing == 2 && new_hblank ||
		 fromGPU && DMAs[index].dMA_Start_Timing == 3 && index == 3)
	{
		DMAs[index].waitTicks = 3;
		DMAs[index].waiting = false;
		DMAs[index].first = true;
		DMAs[index].fullcount = DMAs[index].count;
	}
}

void Dma::work()
{
	dma_active = false;
	delayed = false;

	for (int i = 0; i < 4; i++)
	{
		if (DMAs[i].dMA_Enable)
		{
			if (DMAs[i].waiting)
			{
				check_run(i, false);
			}

			if (DMAs[i].waitTicks > 0)
			{
				if (CPU.newticks >= DMAs[i].waitTicks)
				{
					DMAs[i].running = true;
					DMAs[i].waitTicks = 0;
				}
				else
				{
					DMAs[i].waitTicks -= CPU.newticks;
				}
			}

			if (DMAs[i].running)
			{
				// remember for timing
				UInt32 sm = (DMAs[i].addr_source >> 24) & 0xF;
				UInt32 dm = (DMAs[i].addr_target >> 24) & 0xF;

				// eeprom hack
				cpuDmaCount = (int)DMAs[i].fullcount;

				dma_active = true;

				// timer hack -> must count up before requested
				if (DMAs[i].first)
				{
					CPU.newticks += 2;
				}
				if (sm == 0x4 && (DMAs[i].addr_source & 0xFFF) >= GBRegs.Sect_timer.TM0CNT_L.address && (DMAs[i].addr_source & 0xFFF) <= GBRegs.Sect_timer.TM3CNT_L.address)
				{
					Timer.work(true);
				}

				if (DMAs[i].dMA_Transfer_Type)
				{
					UInt32 value;
					if (DMAs[i].addr_source >= 0x02000000)
					{
						value = Memory.read_dword(DMAs[i].addr_source);
						if (!Memory.unreadable)
						{
							last_dma_value = value;
							last_dma_index = i;
							CPU.op_since_dma = 0;
						}
						value = last_dma_value;
					}
					else
					{
						value = last_dma_value;
					}
					Memory.write_dword(DMAs[i].addr_target, value);

					if (DMAs[i].source_Adr_Control == 0 || DMAs[i].source_Adr_Control == 3 || (DMAs[i].addr_source >= 0x08000000 && DMAs[i].addr_source < 0x0E000000)) { DMAs[i].addr_source += 4; }
					else if (DMAs[i].source_Adr_Control == 1) { DMAs[i].addr_source -= 4; }

					if (DMAs[i].dest_Addr_Control == 0 || (DMAs[i].dest_Addr_Control == 3 && DMAs[i].dMA_Start_Timing != 3)) { DMAs[i].addr_target += 4; }
					else if (DMAs[i].dest_Addr_Control == 1) { DMAs[i].addr_target -= 4; }
				}
				else
				{
					UInt16 value;
					if (DMAs[i].addr_source >= 0x02000000)
					{
						UInt32 newvalue = Memory.read_word(DMAs[i].addr_source) & 0xFFFF;
						if (!Memory.unreadable)
						{
							last_dma_value = (UInt32)newvalue | ((UInt32)newvalue << 16);
							last_dma_index = i;
							CPU.op_since_dma = 0;
						}
						value = (UInt16)last_dma_value;
					}
					else
					{
						value = (UInt16)last_dma_value;
					}
					value = (UInt16)last_dma_value;
					Memory.write_word(DMAs[i].addr_target, value);

					if (DMAs[i].source_Adr_Control == 0 || DMAs[i].source_Adr_Control == 3 || (DMAs[i].addr_source >= 0x08000000 && DMAs[i].addr_source < 0x0E000000)) { DMAs[i].addr_source += 2; }
					else if (DMAs[i].source_Adr_Control == 1) { DMAs[i].addr_source -= 2; }

					if (DMAs[i].dest_Addr_Control == 0 || (DMAs[i].dest_Addr_Control == 3 && DMAs[i].dMA_Start_Timing != 3)) { DMAs[i].addr_target += 2; }
					else if (DMAs[i].dest_Addr_Control == 1) { DMAs[i].addr_target -= 2; }
				}

				debug_dmatranfers++;

				DMAs[i].count--;

				// calc timing
				int ticks;
				if (DMAs[i].first)
				{
					if (DMAs[i].dMA_Transfer_Type)
					{
						ticks = 2 + BusTiming.memoryWait32[sm & 15] + BusTiming.memoryWaitSeq32[dm & 15];
					}
					else
					{
						ticks = 2 + BusTiming.memoryWait[sm & 15] + BusTiming.memoryWaitSeq[dm & 15];
					}
					DMAs[i].first = false;
					if ((DMAs[i].addr_source < 0x08000000) && (DMAs[i].addr_target >= 0x08000000))
					{
						ticks += 2;
					}
				}
				else
				{
					if (DMAs[i].dMA_Transfer_Type)
					{
						ticks = 2 + BusTiming.memoryWaitSeq32[sm & 15] + BusTiming.memoryWaitSeq32[dm & 15];
					}
					else
					{
						ticks = 2 + BusTiming.memoryWaitSeq[sm & 15] + BusTiming.memoryWaitSeq[dm & 15];
					}
				}
				CPU.newticks += ticks;

				if (DMAs[i].count == 0)
				{
					DMAs[i].running = false;

					if (DMAs[i].iRQ_on)
					{
						switch (i)
						{
						case 0: IRP.set_irp_bit(IRP.IRPMASK_DMA_0); break;
						case 1: IRP.set_irp_bit(IRP.IRPMASK_DMA_1); break;
						case 2: IRP.set_irp_bit(IRP.IRPMASK_DMA_2); break;
						case 3: IRP.set_irp_bit(IRP.IRPMASK_DMA_3); break;
						}
					}

					if (DMAs[i].dMA_Repeat && DMAs[i].dMA_Start_Timing != 0)
					{
						DMAs[i].waiting = true;
						if (DMAs[i].dMA_Start_Timing == 3 && (i == 1 || i == 2))
						{
							DMAs[i].count = 4;
						}
						else
						{
							DMAs[i].count = DMAs[i].CNT_L.read();
							if (DMAs[i].dest_Addr_Control == 3)
							{
								DMAs[i].addr_target = DMAs[i].DAD.read();
								switch (i)
								{
								case 0: DMAs[i].addr_target &= 0x07FFFFFF; break;
								case 1: DMAs[i].addr_target &= 0x07FFFFFF; break;
								case 2: DMAs[i].addr_target &= 0x07FFFFFF; break;
								case 3: DMAs[i].addr_target &= 0x0FFFFFFF; break;
								}
							}
						}
					}
					else
					{
						DMAs[i].DMA_Enable.write(0);
						DMAs[i].dMA_Enable = false;
					}

#if DEBUG
					//if (CPU.traclist_ptr > 0 && !DMAs[i].skipdebugout)
					//{
					//    CPU.Tracelist[CPU.traclist_ptr - 1].thumbmode = 3;
					//    CPU.Tracelist[CPU.traclist_ptr - 1].memory01 = (UInt32) DMAs[i].totalTicks;
					//    CPU.Tracelist[CPU.traclist_ptr - 1].memory02 = DMAs[i].addr_source_save;
					//}
					DMAs[i].skipdebugout = false;
#endif
				}

				break;
			}
		}
	}

	new_hblank = false;
	new_vblank = false;
}

bool Dma::request_audio(uint audioindex)
{
	for (int i = 1; i < 3; i++)
	{
		if (DMAs[i].dMA_Enable && DMAs[i].waiting && DMAs[i].dMA_Start_Timing == 3)
		{
			if (audioindex + 1 == i)
			{
				DMAs[i].running = true;
				DMAs[i].first = true;
				DMAs[i].fullcount = DMAs[i].count;
				DMAs[i].skipdebugout = true;
				return true;
			}
		}
	}
	return false;
}