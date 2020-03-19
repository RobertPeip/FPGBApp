#include "Memory.h"
#include "CPU.h"
#include "DMA.h"
#include "GBRegs.h"
#include "EEProm.h"
#include "Flash.h"
#include "GPU.h"
#include "GPU_Timing.h"
#include "gpio.h"
#include "fileIO.h"
#include "Timer.h"
#include "gameboy.h"
#include "BusTiming.h"
#include "IRP.h"
#include "Sound.h"
#include "SoundDMA.h"
#include "Joypad.h"
#include "Serial.h"
#include "Header.h"

MEMORY Memory;

void MEMORY::reset(string filename)
{
	//string biosname = "gba_bios.bin";
	string biosname = "gba_bios_fast.bin";

	FileIO.readfile(GBRom, biosname, false);

	GameRom_max = FileIO.readfile(GameRom, filename, true);

	EEPROMEnabled = true;
	FlashEnabled = true;
	SramEnabled = true;
	EEPROMSensorEnabled = false;

    Header.autocheck();

	tiltx = 0x3A0;
	tilty = 0x3A0;

	gpio_used = false;
	//gpio.rtcEnable(true);

	load_gameram();
}

void MEMORY::GameRAMSnapshot()
{
	/*
	if (createGameRAMSnapshot)
	{
		int index = 0;
		for (int i = 0; i < Flash.flashSaveMemory.Length; i++)
		{
			GameRamSnapshot[index] = Flash.flashSaveMemory[i];
			index++;
		}
		for (int i = 0; i < EEProm.eepromData.Length; i++)
		{
			GameRamSnapshot[index] = EEProm.eepromData[i];
			index++;
		}

		createGameRAMSnapshot = false;
	}
	*/
}


void MEMORY::load_gameram()
{
	/*
	if (gameboy.filename != null)
	{
		string filename = Path.GetFileNameWithoutExtension(gameboy.filename) + ".sav";
		if (File.Exists(filename))
		{
			FileInfo fileInfo = new FileInfo(filename);
			if (fileInfo.Length == GameRamSnapshot.Length)
			{
				FileStream fileStream = fileInfo.OpenRead();
				fileStream.Read(GameRamSnapshot, 0, GameRamSnapshot.Length);
				fileStream.Close();

				int index = 0;
				for (int i = 0; i < Flash.flashSaveMemory.Length; i++)
				{
					Flash.flashSaveMemory[i] = GameRamSnapshot[index];
					index++;
				}
				for (int i = 0; i < EEProm.eepromData.Length; i++)
				{
					EEProm.eepromData[i] = GameRamSnapshot[index];
					index++;
				}
			}
			else
			{
				MessageBox.Show("Savegame corrupt!");
			}

		}
	}
	*/
}

void MEMORY::save_gameram()
{
	/*
	if (gameboy.filename != null)
	{
		string filename = Path.GetFileNameWithoutExtension(gameboy.filename) + ".sav";
		FileStream fileStream = new FileStream(filename, FileMode.Create);
		fileStream.Write(GameRamSnapshot, 0, GameRamSnapshot.Length);
		fileStream.Close();
	}
	*/
}

byte MEMORY::read_unreadable_byte(UInt32 offset)
{
	byte value;

	if (CPU.thumbmode && ((CPU.regs[15] >> 24) & 15) == 3)
	{
		if ((CPU.regs[15] & 0x3) == 0)
		{
			value = read_byte(CPU.regs[15] - 2 + offset);
		}
		else
		{
			value = read_byte(CPU.regs[15] + offset);
		}
	}
	else
	{
		value = read_byte(CPU.regs[15] + offset);
	}

	unreadable = true;

	return value;
}

UInt16 MEMORY::read_unreadable_word()
{
	UInt16 value;
	if (CPU.thumbmode && ((CPU.regs[15] >> 24) & 15) == 3)
	{
		if ((CPU.regs[15] & 0x3) == 0)
		{
			value = (UInt16)read_word(CPU.regs[15] - 2);
		}
		else
		{
			value = (UInt16)read_word(CPU.regs[15]);
		}
	}
	else
	{
		value = (UInt16)read_word(CPU.regs[15]);
	}

	unreadable = true;

	return value;
}

UInt32 MEMORY::read_unreadable_dword()
{
	if (CPU.op_since_dma < 2)
	{
		return DMA.last_dma_value;
	}

	UInt32 value;

	if (CPU.thumbmode)
	{
		//For THUMB code in 32K - WRAM on GBA, GBA SP, GBA Micro, NDS-Lite(but not NDS):
		//LSW = [$+4], MSW = OldHI   ; for opcodes at 4 - byte aligned locations
		//LSW = OldLO, MSW = [$+4]   ; for opcodes at non - 4 - byte aligned locations
		//OldLO=[$+2], OldHI=[$+2]
		if (((CPU.regs[15] >> 24) & 15) == 3)
		{
			if ((CPU.regs[15] & 0x3) == 0)
			{
				UInt32 retval_low = read_word(CPU.regs[15]) & 0xFFFF;
				UInt32 retval_high = read_word(CPU.regs[15] - 2) & 0xFFFF;
				value = retval_high << 16 | retval_low;
			}
			else
			{
				UInt32 retval_low = read_word(CPU.regs[15] - 2) & 0xFFFF;
				UInt32 retval_high = read_word(CPU.regs[15]) & 0xFFFF;
				value = retval_high << 16 | retval_low;
			}
		}
		else // standard case LSW = [$+4], MSW = [$+4]
		{
			UInt32 retval = read_word(CPU.regs[15]);
			value = retval << 16 | retval;
		}
	}
	else
	{
		value = read_dword(CPU.regs[15]);
	}

	unreadable = true;

	return value;
}

byte MEMORY::read_byte(UInt32 address)
{
	unreadable = false;
	uint adr;

	byte select = (byte)(address >> 24);
	switch (select)
	{
	case 0:
		if ((CPU.regs[15] >> 24) > 0)
		{
			if (address < 0x4000)
			{
				return biosProtected[address & 3];
			}
			else
			{
				return read_unreadable_byte(address & 1);
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				biosProtected[i] = GBRom[((address + 8) & 0x3FFC) + i];
			}
			return GBRom[address & 0x3FFF];
		}

	case 1: return read_unreadable_byte(address & 1);
	case 2: return WRAM_Large[address & 0x03FFFF];
	case 3: return WRAM_Small[address & 0x7FFF];

	case 4:
		if (address < 0x04000400)
		{
			adr = address & 0x3FF;
			byte rwmask = GBRegs.rwmask[adr];

			if (rwmask == 0)
			{
				return read_unreadable_byte(address & 1);
			}
			else
			{
				prepare_read_gbreg(adr);
				byte value = GBRegs.data[adr];
				value &= rwmask;
				return value;
			}
		}
		else
		{
			return read_unreadable_byte(address & 1);
		}

	case 5: return PaletteRAM[address & 0x3FF];

	case 6:
		adr = address & 0x1FFFF;
		if (adr > 0x17FFF) { adr -= 0x8000; }
		return VRAM[adr & 0x1FFFF];

	case 7: return OAMRAM[address & 0x3FF];

	case 8:
	case 9:
	case 0xA:
	case 0xB:
	case 0xC:
		if ((address & 0x01FFFFFF) < GameRom_max)
		{
			return GameRom[address & 0x01FFFFFF];
		}
		else
		{
			if ((address & 1) == 0)
			{
				return (byte)(address / 2);
			}
			else
			{
				return (byte)((address >> 8) / 2);
			}
		}

	case 0xD:
		if (EEPROMEnabled)
		{
			return (byte)EEProm.read();
		}
		else
		{
			return read_unreadable_byte(address & 1);
		}

	case 0xE:
	case 0xF:
		if (has_tilt && address == 0x0E008200)
		{
			return (byte)tiltx;
		}
		if (has_tilt && address == 0x0E008300)
		{
			return (byte)(0x80 | ((tiltx >> 8) & 0xF));
		}
		if (has_tilt && address == 0x0E008400)
		{
			return (byte)tilty;
		}
		if (has_tilt && address == 0x0E008500)
		{
			return (byte)((tilty >> 8) & 0xF);
		}

		if (FlashEnabled | SramEnabled)
		{
			return Flash.flashRead(address);
		}
		else
		{
			return read_unreadable_byte(address);
		}

	default: return read_unreadable_byte(address & 1);

	}
}

UInt32 MEMORY::read_word(UInt32 address)
{
	unreadable = false;

	UInt32 value = 0;
	byte rotate = (byte)(address & 1);
	address = address & 0xFFFFFFFE;
	uint adr;

	byte select = (byte)(address >> 24);
	switch (select)
	{
	case 0:
		if ((CPU.regs[15] >> 24) > 0)
		{
			if (address < 0x4000)
			{
				value = *(UInt16*)&biosProtected[address & 2];
			}
			else
			{
				value = read_unreadable_word();
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				biosProtected[i] = GBRom[((address + 8) & 0x3FFC) + i];
			}
			value = *(UInt16*)&GBRom[address & 0x3FFE];
		}
		break;

	case 1: value = read_unreadable_word(); break;
	case 2: value = *(UInt16*)&WRAM_Large[address & 0x03FFFF]; break;
	case 3: value = *(UInt16*)&WRAM_Small[address & 0x7FFF]; break;

	case 4:
		if (address < 0x04000400)
		{
			adr = address & 0x3FF;

			if (adr == GBRegs.Sect_dma.DMA0CNT_L.address ||
				adr == GBRegs.Sect_dma.DMA1CNT_L.address ||
				adr == GBRegs.Sect_dma.DMA2CNT_L.address ||
				adr == GBRegs.Sect_dma.DMA3CNT_L.address)
			{
				return 0;
			}
			else
			{
				UInt16 rwmask = *(UInt16*)&GBRegs.rwmask[adr & 0x3FFE];

				if (rwmask == 0)
				{
					value = read_unreadable_word();
				}
				else
				{
					prepare_read_gbreg(adr);
					value = *(UInt16*)&GBRegs.data[adr];
					value &= rwmask;
				}
			}
		}
		else
		{
			value = read_unreadable_word();
		}
		break;

	case 5: value = *(UInt16*)&PaletteRAM[address & 0x3FF]; break;

	case 6:
		adr = address & 0x1FFFF;
		if (adr > 0x17FFF) { adr -= 0x8000; }
		value = *(UInt16*)&VRAM[adr];
		break;

	case 7: value = *(UInt16*)&OAMRAM[address & 0x3FF]; break;

	case 8:
	case 9:
	case 0xA:
	case 0xB:
	case 0xC:
		if (gpio_enable && gpio_used && address >= 0x80000c4 && address <= 0x80000c8)
		{
			value = gpio.gpioRead(address);
		}
		else
		{
			if ((address & 0x01FFFFFF) < GameRom_max)
			{
				value = *(UInt16*)&GameRom[address & 0x01FFFFFF];
			}
			else
			{
				value = (address / 2) & 0xFFFF;
			}
		}
		break;

	case 0xD:
		if (EEPROMEnabled)
		{
			value = (UInt32)EEProm.read();
		}
		break;

	case 0xE:
	case 0xF:
		if (FlashEnabled | SramEnabled)
		{
			value = Flash.flashRead(address + rotate) * (UInt32)0x0101;
		}
		else
		{
			value = read_unreadable_word();
		}
		break;

	default: value = read_unreadable_word(); break;
	}

	lastreadvalue = value;

	if (rotate == 0)
	{
		return value;
	}
	else
	{
		value = ((value & 0xFF) << 24) | (value >> 8);
	}

	return value;
}

UInt32 MEMORY::read_dword(UInt32 address)
{
	unreadable = false;

	UInt32 value = 0;
	byte rotate = (byte)(address & 3);
	address = address & 0xFFFFFFFC;
	uint adr;

	byte select = (byte)(address >> 24);
	switch (select)
	{
	case 0:
		if ((CPU.regs[15] >> 24) > 0)
		{
			if (address < 0x4000)
			{
				value = *(UInt32*)&biosProtected[0];
			}
			else
			{
				value = read_unreadable_dword();
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				biosProtected[i] = GBRom[((address + 8) & 0x3FFC) + i];
			}
			value = *(UInt32*)&GBRom[address & 0x3FFC];
		}
		break;

	case 1: value = read_unreadable_dword(); break;
	case 2: value = *(UInt32*)&WRAM_Large[address & 0x03FFFF]; break;
	case 3: value = *(UInt32*)&WRAM_Small[address & 0x7FFF]; break;

	case 4:
		if (address < 0x04000400)
		{
			value = (UInt16)read_word(address) | (read_word(address + 2) << 16);
		}
		else
		{
			value = read_unreadable_dword();
		}
		break;

	case 5: value = *(UInt32*)&PaletteRAM[address & 0x3FF]; break;

	case 6:
		adr = address & 0x1FFFF;
		if (adr > 0x17FFF) { adr -= 0x8000; }
		value = *(UInt32*)&VRAM[adr];
		break;

	case 7: value = *(UInt32*)&OAMRAM[address & 0x3FF]; break;

	case 8:
	case 9:
	case 0xA:
	case 0xB:
	case 0xC:
		if (gpio_enable && gpio_used && address >= 0x80000c4 && address <= 0x80000c8)
		{
			value = gpio.gpioRead(address);
		}
		else
		{
			if ((address & 0x01FFFFFF) < GameRom_max)
			{
				value = *(UInt32*)&GameRom[address & 0x01FFFFFF];
			}
			else
			{
				value = ((address / 2) & 0xFFFF) + ((((address / 2) + 1) & 0xFFFF) << 16);
			}
		}
		break;

	case 0xD:
		if (EEPROMEnabled)
		{
			value = (UInt32)EEProm.read();
		}
		break;

	case 0xE:
	case 0xF:
		if (FlashEnabled | SramEnabled)
		{
			value = Flash.flashRead(address + rotate + blockcmd_lowerbits) * (UInt32)0x01010101;
		}
		else
		{
			value = read_unreadable_dword();
		}
		break;

	default: value = read_unreadable_dword(); break;
	}

	lastreadvalue = value;

	if (rotate == 0)
	{
		return value;
	}
	else
	{
		value = CPU.RotateRight(value, 8 * rotate);
	}

	return value;
}

void MEMORY::write_byte(UInt32 address, byte data)
{
	uint adr;
	byte select = (byte)(address >> 24);
	switch (select)
	{
	case 2: WRAM_Large[address & 0x03FFFF] = (byte)(data & 0xFF); return;
	case 3: WRAM_Small[address & 0x7FFF] = (byte)(data & 0xFF); return;

	case 4:
		if (address < 0x04000400)
		{
			adr = address & 0x3FF;
			GBRegs.data[adr] = data;
			write_gbreg(adr & 0xFFFFFFFE, data, false);
		}
		return;

		// Writing 8bit Data to Video Memory
		// Video Memory(BG, OBJ, OAM, Palette) can be written to in 16bit and 32bit units only.Attempts to write 8bit data(by STRB opcode) won't work:
		// Writes to OBJ(6010000h - 6017FFFh)(or 6014000h - 6017FFFh in Bitmap mode) and to OAM(7000000h - 70003FFh) are ignored, the memory content remains unchanged.
		// Writes to BG(6000000h - 600FFFFh)(or 6000000h - 6013FFFh in Bitmap mode) and to Palette(5000000h - 50003FFh) are writing the new 8bit value to BOTH upper and lower 8bits of the addressed halfword, ie. "[addr AND NOT 1]=data*101h".

	case 5: PaletteRAM[address & 0x3FE] = data; PaletteRAM[(address & 0x3FE) + 1] = data; return;

	case 6:
		adr = address & 0x1FFFE;
		if ((GPU.videomode <= 2 && adr <= 0xFFFF) || GPU.videomode >= 3 && adr <= 0x013FFF)
		{
			if (adr > 0x17FFF) { adr -= 0x8000; }
			VRAM[adr] = data;
			VRAM[adr + 1] = data;
		}
		return;

	case 7: return; // no saving here!

	case 0xD:
		if (EEPROMEnabled)
		{
			EEProm.write(data);
		}
		return;

	case 0xE:
	case 0xF:
		if (!EEPROMEnabled || FlashEnabled || SramEnabled)
		{
			Flash.flashSaveDecide(address, data);
		}
		return;
	}
}

void MEMORY::write_word(UInt32 address, UInt16 data)
{
	byte offset = (byte)(address & 1);
	address = address & 0xFFFFFFFE;
	uint adr;

	byte select = (byte)(address >> 24);
	switch (select)
	{
	case 2:
		adr = address & 0x03FFFF;
		WRAM_Large[adr] = (byte)(data & 0xFF);
		WRAM_Large[adr + 1] = (byte)((data >> 8) & 0xFF);
		return;

	case 3:
		adr = address & 0x7FFF;
		WRAM_Small[adr] = (byte)(data & 0xFF);
		WRAM_Small[adr + 1] = (byte)((data >> 8) & 0xFF);
		return;

	case 4:
		if (address < 0x04000400)
		{
			adr = address & 0x3FF;
			GBRegs.data[adr] = (byte)(data & 0xFF);
			GBRegs.data[adr + 1] = (byte)((data >> 8) & 0xFF);
			write_gbreg(adr, data, false);
		}
		return;

	case 5:
		adr = address & 0x3FF;
		PaletteRAM[adr] = (byte)(data & 0xFF);
		PaletteRAM[adr + 1] = (byte)((data >> 8) & 0xFF);
		return;

	case 6:
		adr = address & 0x1FFFF;
		if (GPU.videomode < 3 || ((address & 0x1C000) != 0x18000))
		{
			if (adr > 0x17FFF) { adr -= 0x8000; }
			VRAM[adr] = (byte)(data & 0xFF);
			VRAM[adr + 1] = (byte)((data >> 8) & 0xFF);
		}
		return;

	case 7:
		adr = address & 0x3FF;
		OAMRAM[adr] = (byte)(data & 0xFF);
		OAMRAM[adr + 1] = (byte)((data >> 8) & 0xFF);
		return;

	case 0x8:
		if (gpio_enable)
		{
			if (address >= 0x80000c4 && address <= 0x80000c8)
			{
				gpio_used = true;
				gpio.gpioWrite(address, (ushort)data);
			}
		}
		return;

	case 0xD:
		if (EEPROMEnabled)
		{
			EEProm.write((byte)data);
		}
		return;

	case 0xE:
	case 0xF:
		if (!EEPROMEnabled || FlashEnabled || SramEnabled)
		{
			Flash.flashSaveDecide(address + offset + blockcmd_lowerbits, (byte)data);
		}
		return;
	}
}

void MEMORY::write_dword(UInt32 address, UInt32 data)
{
	byte offset = (byte)(address & 3);
	address = address & 0xFFFFFFFC;
	uint adr;

	byte select = (byte)(address >> 24);
	switch (select)
	{
	case 2:
		adr = address & 0x03FFFF;
		WRAM_Large[adr] = (byte)(data & 0xFF);
		WRAM_Large[adr + 1] = (byte)((data >> 8) & 0xFF);
		WRAM_Large[adr + 2] = (byte)((data >> 16) & 0xFF);
		WRAM_Large[adr + 3] = (byte)((data >> 24) & 0xFF);
		return;

	case 3:
		adr = address & 0x7FFF;
		WRAM_Small[adr] = (byte)(data & 0xFF);
		WRAM_Small[adr + 1] = (byte)((data >> 8) & 0xFF);
		WRAM_Small[adr + 2] = (byte)((data >> 16) & 0xFF);
		WRAM_Small[adr + 3] = (byte)((data >> 24) & 0xFF);
		return;

	case 4:
		if (address < 0x04000400)
		{
			adr = address & 0x3FF;

			GBRegs.data[adr] = (byte)(data & 0xFF);
			GBRegs.data[adr + 1] = (byte)((data >> 8) & 0xFF);
			GBRegs.data[adr + 2] = (byte)((data >> 16) & 0xFF);
			GBRegs.data[adr + 3] = (byte)((data >> 24) & 0xFF);

			write_gbreg(adr, data, true);
			write_gbreg(adr + 2, data, true);
		}
		return;

	case 5:
		adr = address & 0x3FF;
		PaletteRAM[adr] = (byte)(data & 0xFF);
		PaletteRAM[adr + 1] = (byte)((data >> 8) & 0xFF);
		PaletteRAM[adr + 2] = (byte)((data >> 16) & 0xFF);
		PaletteRAM[adr + 3] = (byte)((data >> 24) & 0xFF);
		return;

	case 6:
		adr = address & 0x1FFFF;
		if (GPU.videomode < 3 || ((address & 0x1C000) != 0x18000))
		{
			if (adr > 0x17FFF) { adr -= 0x8000; }
			VRAM[adr] = (byte)(data & 0xFF);
			VRAM[adr + 1] = (byte)((data >> 8) & 0xFF);
			VRAM[adr + 2] = (byte)((data >> 16) & 0xFF);
			VRAM[adr + 3] = (byte)((data >> 24) & 0xFF);
		}
		return;

	case 7:
		adr = address & 0x3FF;
		OAMRAM[adr] = (byte)(data & 0xFF);
		OAMRAM[adr + 1] = (byte)((data >> 8) & 0xFF);
		OAMRAM[adr + 2] = (byte)((data >> 16) & 0xFF);
		OAMRAM[adr + 3] = (byte)((data >> 24) & 0xFF);
		return;

	case 0x8:
		if (gpio_enable)
		{
			if (address >= 0x80000c4 && address <= 0x80000c8)
			{
				gpio_used = true;
				gpio.gpioWrite(address, (ushort)data);
			}
		}
		return;

	case 0xD:
		if (EEPROMEnabled)
		{
			EEProm.write((byte)data);
		}
		return;

	case 0xE:
	case 0xF:
		if (!EEPROMEnabled || FlashEnabled || SramEnabled)
		{
			Flash.flashSaveDecide(address + offset + blockcmd_lowerbits, (byte)data);
		}
		return;
	}
}

void MEMORY::prepare_read_gbreg(UInt32 adr)
{
	if (adr == GBRegs.Sect_timer.TM0CNT_L.address)
	{
		UInt16 value = Timer.timers[0].retval;
		GBRegs.data[adr] = (byte)(value & 0xFF);
		GBRegs.data[adr + 1] = (byte)((value >> 8) & 0xFF);
	}
	else if (adr == GBRegs.Sect_timer.TM1CNT_L.address)
	{
		UInt16 value = Timer.timers[1].retval;
		GBRegs.data[adr] = (byte)(value & 0xFF); GBRegs.data[adr + 1] = (byte)((value >> 8) & 0xFF);
	}
	else if (adr == GBRegs.Sect_timer.TM2CNT_L.address)
	{
		UInt16 value = Timer.timers[2].retval; GBRegs.data[adr] = (byte)(value & 0xFF);
		GBRegs.data[adr + 1] = (byte)((value >> 8) & 0xFF);
	}
	else if (adr == GBRegs.Sect_timer.TM3CNT_L.address)
	{
		UInt16 value = Timer.timers[3].retval;
		GBRegs.data[adr] = (byte)(value & 0xFF); GBRegs.data[adr + 1] = (byte)((value >> 8) & 0xFF);
	}

	else if (adr == GBRegs.Sect_sound.SOUNDCNT_X.address)
	{
		GBRegs.data[adr] = (byte)(GBRegs.data[adr] & 0x80);
		if (Sound.soundGenerator->soundchannels[0].on && Sound.soundGenerator->enable_channels_left[0] || Sound.soundGenerator->enable_channels_right[0]) { GBRegs.data[adr] |= 0x01; }
		if (Sound.soundGenerator->soundchannels[1].on && Sound.soundGenerator->enable_channels_left[1] || Sound.soundGenerator->enable_channels_right[1]) { GBRegs.data[adr] |= 0x02; }
		if (Sound.soundGenerator->soundchannels[2].on && Sound.soundGenerator->enable_channels_left[2] || Sound.soundGenerator->enable_channels_right[2]) { GBRegs.data[adr] |= 0x04; }
		if (Sound.soundGenerator->soundchannels[3].on && Sound.soundGenerator->enable_channels_left[3] || Sound.soundGenerator->enable_channels_right[3]) { GBRegs.data[adr] |= 0x08; }
	}
}

void MEMORY::write_gbreg(UInt32 adr, UInt32 value, bool dwaccess)
{
	if (adr == GBRegs.Sect_display.DISPCNT.address) { GPU.dispcnt_write(); }
	if (adr == GBRegs.Sect_display.DISPSTAT.address) { GPU_Timing.dispstat_write(); }
	else if (adr == GBRegs.Sect_display.BG2RefX.address) { GPU.refpoint_update_2x_new(); }
	else if (adr == GBRegs.Sect_display.BG2RefX.address + 2) { GPU.refpoint_update_2x_new(); }
	else if (adr == GBRegs.Sect_display.BG2RefY.address) { GPU.refpoint_update_2y_new(); }
	else if (adr == GBRegs.Sect_display.BG2RefY.address + 2) { GPU.refpoint_update_2y_new(); }
	else if (adr == GBRegs.Sect_display.BG3RefX.address) { GPU.refpoint_update_3x_new(); }
	else if (adr == GBRegs.Sect_display.BG3RefX.address + 2) { GPU.refpoint_update_3x_new(); }
	else if (adr == GBRegs.Sect_display.BG3RefY.address) { GPU.refpoint_update_3y_new(); }
	else if (adr == GBRegs.Sect_display.BG3RefY.address + 2) { GPU.refpoint_update_3y_new(); }

	else if (adr >= GBRegs.Sect_sound.SOUND1CNT_L.address && adr < GBRegs.Sect_sound.FIFO_A.address)
	{
		Sound.set_soundreg(adr);
	}

	else if (adr == GBRegs.Sect_sound.FIFO_A.address) { SoundDMA.fill_fifo(0, value, dwaccess); }
	else if (adr == GBRegs.Sect_sound.FIFO_B.address) { SoundDMA.fill_fifo(1, value, dwaccess); }

	else if (adr == GBRegs.Sect_serial.SIOCNT.address) { Serial.write_SIOCNT(*(UInt16*)&GBRegs.data[GBRegs.Sect_serial.SIOCNT.address]); }

	else if (adr == GBRegs.Sect_timer.TM0CNT_L.address) { Timer.set_reload(0); }
	else if (adr == GBRegs.Sect_timer.TM0CNT_L.address + 2) { Timer.set_settings(0); }
	else if (adr == GBRegs.Sect_timer.TM1CNT_L.address) { Timer.set_reload(1); }
	else if (adr == GBRegs.Sect_timer.TM1CNT_L.address + 2) { Timer.set_settings(1); }
	else if (adr == GBRegs.Sect_timer.TM2CNT_L.address) { Timer.set_reload(2); }
	else if (adr == GBRegs.Sect_timer.TM2CNT_L.address + 2) { Timer.set_settings(2); }
	else if (adr == GBRegs.Sect_timer.TM3CNT_L.address) { Timer.set_reload(3); }
	else if (adr == GBRegs.Sect_timer.TM3CNT_L.address + 2) { Timer.set_settings(3); }

	else if (adr == GBRegs.Sect_dma.DMA0CNT_H.address + 2) { DMA.set_settings(0); }
	else if (adr == GBRegs.Sect_dma.DMA1CNT_H.address + 2) { DMA.set_settings(1); }
	else if (adr == GBRegs.Sect_dma.DMA2CNT_H.address + 2) { DMA.set_settings(2); }
	else if (adr == GBRegs.Sect_dma.DMA3CNT_H.address + 2) { DMA.set_settings(3); }

	else if (adr == GBRegs.Sect_keypad.KEYINPUT.address) { Joypad.set_reg(); }

	else if (adr == GBRegs.Sect_system.IME.address) { IRP.update_IME(*(UInt16*)&GBRegs.data[GBRegs.Sect_system.IME.address]); }
	else if (adr == GBRegs.Sect_system.IE.address) { IRP.update_IE(); }
	else if (adr == GBRegs.Sect_system.IF.address + 2) { IRP.clear_irp_bits(); }

	else if (adr == GBRegs.Sect_system.WAITCNT.address) { BusTiming.update(*(UInt16*)&GBRegs.data[GBRegs.Sect_system.WAITCNT.address]); }
	else if ((adr == GBRegs.Sect_system.POSTFLG.address) && (value == 1)) {}
	else if (adr == GBRegs.Sect_system.HALTCNT.address && !gameboy.loading_state)
	{
		if ((GBRegs.Sect_system.HALTCNT.read() & 0x80) == 0x80)
			CPU.stop = true;
		else
			CPU.halt = true;
	}
}