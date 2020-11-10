#include "GPU_Timing.h"
#include "GBRegs.h"
#include "DMA.h"
#include "GPU.h"
#include "CPU.h"
#include "IRP.h"

GPUTiming GPU_Timing;

void GPUTiming::reset()
{
	cycles = 0;
	line = 0;
	gpustate = GPUState::VISIBLE;
	old_dispstat = 0;
}

void GPUTiming::dispstat_write()
{
	UInt16 new_val = (UInt16)GBRegs.Sect_display.DISPSTAT.read();
	new_val &= 0xFF38;
	old_dispstat &= 0x00C7;
	new_val |= old_dispstat;
	GBRegs.Sect_display.DISPSTAT.write(new_val);
}

void GPUTiming::work()
{
	cycles += CPU.newticks;

	bool runagain = true;

	while (runagain)
	{
		runagain = false;

		switch (gpustate)
		{
		case GPUState::VISIBLE:
			if (cycles >= 1008) // 960 is drawing time
			{
				runagain = true;
				cycles -= 1008;
				gpustate = GPUState::HBLANK;
				GBRegs.Sect_display.DISPSTAT_H_Blank_flag.write(1);
				DMA.new_hblank = true;
				if (GBRegs.Sect_display.DISPSTAT_H_Blank_IRQ_Enable.on())
				{
					IRP.set_irp_bit(IRP.IRPMASK_LCD_H_Blank);
				}
				old_dispstat = GBRegs.data[4];

				GPU.once_per_hblank();
				GPU.next_line(line);
			}
			break;

		case GPUState::HBLANK:
			if (cycles >= 224) // 272
			{
				runagain = true;
				cycles -= 224;
				nextline();

				GBRegs.Sect_display.DISPSTAT_H_Blank_flag.write(0);
				DMA.new_hblank = false;
				if (line < 160)
				{
					gpustate = GPUState::VISIBLE;
				}
				else
				{
					gpustate = GPUState::VBLANK;
					GPU.refpoint_update_all();
					//Cheats.apply_cheats();
					GBRegs.Sect_display.DISPSTAT_V_Blank_flag.write(1);
					DMA.new_vblank = true;
					if (GBRegs.Sect_display.DISPSTAT_V_Blank_IRQ_Enable.on())
					{
						IRP.set_irp_bit(IRP.IRPMASK_LCD_V_Blank);
					}
				}
				old_dispstat = GBRegs.data[4];
			}
			break;

		case GPUState::VBLANK:
			if (cycles >= 1008)
			{
				runagain = true;
				cycles -= 1008;
				gpustate = GPUState::VBLANKHBLANK;
				GBRegs.Sect_display.DISPSTAT_H_Blank_flag.write(1);
				//DMA.new_hblank = true; //!!! don't do here!
				if (GBRegs.Sect_display.DISPSTAT_H_Blank_IRQ_Enable.on())
				{
					IRP.set_irp_bit(IRP.IRPMASK_LCD_H_Blank); // Note that no H-Blank interrupts are generated within V-Blank period. Really?
				}
				old_dispstat = GBRegs.data[4];
			}
			break;

		case GPUState::VBLANKHBLANK:
			if (cycles >= 224)
			{
				runagain = true;
				cycles -= 224;
				nextline();
				GBRegs.Sect_display.DISPSTAT_H_Blank_flag.write(0);
				DMA.new_hblank = false;
				GPU.once_per_hblank();
				if (line == 0)
				{
					gpustate = GPUState::VISIBLE;
					//GPU.next_line(line);
					GBRegs.Sect_display.DISPSTAT_V_Blank_flag.write(0);
					DMA.new_vblank = false;
				}
				else
				{
					gpustate = GPUState::VBLANK;
					if (line == 227)
					{
						GBRegs.Sect_display.DISPSTAT_V_Blank_flag.write(0);
					}
				}
				old_dispstat = GBRegs.data[4];
			}
			break;

		}
	}
}

void GPUTiming::nextline()
{
	line++;
	if (line == 228)
	{
		line = 0;
	}
	GBRegs.Sect_display.VCOUNT.write(line);

	if (line == 162)
	{
		if (DMA.DMAs[3].dMA_Start_Timing == 3)
		{
			DMA.DMAs[3].dMA_Enable = false;
			DMA.DMAs[3].DMA_Enable.write(0);
			DMA.DMAs[3].waiting = false;
		}
	}
	else if(line >= 2 && DMA.DMAs[3].waiting)
	{
		DMA.check_run(3, true);
	}

	if (line == GBRegs.Sect_display.DISPSTAT_V_Count_Setting.read())
	{
		if (GBRegs.Sect_display.DISPSTAT_V_Counter_IRQ_Enable.on())
		{
			IRP.set_irp_bit(IRP.IRPMASK_LCD_V_Counter_Match);
		}
		GBRegs.Sect_display.DISPSTAT_V_Counter_flag.write(1);
	}
	else
	{
		GBRegs.Sect_display.DISPSTAT_V_Counter_flag.write(0);
	}
}

void GPUTiming::restart_line()
{
	if (!GBRegs.Sect_display.DISPSTAT_V_Blank_flag.on())
	{
		// really required
		//  line--;
		//  nextline();
		//  gpustate = GPUState::VISIBLE;
		//  GBRegs.Sect_display.DISPSTAT_V_Blank_flag.write(0);
		//  GBRegs.Sect_display.DISPSTAT_H_Blank_flag.write(0);
	}
}