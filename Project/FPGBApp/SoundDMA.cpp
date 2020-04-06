#include "SoundDMA.h"
#include "CPU.h"
#include "Sound.h"
#include "DMA.h"

SOUNDDMA SoundDMA;

SingleSoundDMA::SingleSoundDMA()
{
}

SingleSoundDMA::SingleSoundDMA(GBReg Enable_RIGHT, GBReg Enable_LEFT, GBReg Timer_Select, GBReg Reset_FIFO)
{
	this->Enable_RIGHT = Enable_RIGHT;
	this->Enable_LEFT = Enable_LEFT;
	this->Timer_Select = Timer_Select;
	this->Reset_FIFO = Reset_FIFO;

	timerindex = 0;
	any_on = false;

	while (fifo.size() > 0) fifo.pop();
	while (playfifo.size() > 0) playfifo.pop();

	outfifo.push(0);
	tickcount = 0;
}

void SingleSoundDMA::work()
{
	tickcount += CPU.newticks;
	while (tickcount >= Sound.SAMPLERATE)
	{
		tickcount -= Sound.SAMPLERATE;
		if (playfifo.size() > 0 && outfifo.size() < 15000)
		{
			outfifo.push(playfifo.front());
		}
	}
}

void SOUNDDMA::reset()
{
	soundDMAs[1] = SingleSoundDMA(GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_B_Enable_RIGHT, GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_B_Enable_LEFT,
		GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_B_Timer_Select, GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_B_Reset_FIFO);

	soundDMAs[0] = SingleSoundDMA(GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_A_Enable_RIGHT, GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_A_Enable_LEFT,
		GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_A_Timer_Select, GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_A_Reset_FIFO);
}

void SOUNDDMA::timer_overflow(uint timerindex)
{
	bool request = false;
	CPU.newticks = 0;
	for (uint i = 0; i < 2; i++)
	{
		if (soundDMAs[i].any_on && soundDMAs[i].timerindex == timerindex)
		{
			if (soundDMAs[i].fifo.size() <= 3)
			{
				request |= DMA.request_audio(i);
			}

			if (soundDMAs[i].playfifo.size() > 0)
			{
				soundDMAs[i].playfifo.pop();
			}
			if (soundDMAs[i].playfifo.size() == 0)
			{
				UInt32 value = 0;
				if (soundDMAs[i].fifo.size() > 0)
				{
					value = soundDMAs[i].fifo.front();
					soundDMAs[i].fifo.pop();
					soundDMAs[i].playfifo.push((byte)value);
					soundDMAs[i].playfifo.push((byte)(value >> 8));
					soundDMAs[i].playfifo.push((byte)(value >> 16));
					soundDMAs[i].playfifo.push((byte)(value >> 24));
				}
			}
		}
	}
	if (request)
	{
		DMA.work();
		DMA.delayed = true;
	}
}

void SOUNDDMA::write_SOUNDCNT_H()
{
	for (int i = 0; i < 2; i++)
	{
		if (soundDMAs[i].Reset_FIFO.on())
		{
			while (!soundDMAs[i].fifo.empty()) soundDMAs[i].fifo.pop();
		}
		soundDMAs[i].timerindex = (byte)soundDMAs[i].Timer_Select.read();
		soundDMAs[i].any_on = soundDMAs[i].Enable_LEFT.on() || soundDMAs[i].Enable_RIGHT.on();
	}

	uint oldval = GBRegs.Sect_sound.SOUNDCNT_H.read();
	GBRegs.Sect_sound.SOUNDCNT_H.write(oldval & 0x770F);


	switch (GBRegs.Sect_sound.SOUNDCNT_H_Sound_1_4_Volume.read())
	{
		case 0: Sound.soundGenerator.volume_1_4 = 0.25f; break;
		case 1: Sound.soundGenerator.volume_1_4 = 0.5f; break;
		case 2: Sound.soundGenerator.volume_1_4 = 1.0f; break;
	}
	if (GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_A_Volume.on()) { Sound.soundGenerator.volume_dma0 = 1.0f; }
	else { Sound.soundGenerator.volume_dma0 = 0.5f; }
	if (GBRegs.Sect_sound.SOUNDCNT_H_DMA_Sound_B_Volume.on()) { Sound.soundGenerator.volume_dma1 = 1.0f; }
	else { Sound.soundGenerator.volume_dma1 = 0.5f; }
}

void SOUNDDMA::fill_fifo(int index, UInt32 value, bool dwaccess)
{
	// real hardware does also clear fifo when writing 8th dword
	soundDMAs[index].fifo.push(value);
}