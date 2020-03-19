#include "Flash.h"
#include "Memory.h"

FLASH Flash;

void FLASH::reset()
{
	for (int i = 0; i < SIZE_FLASH1M; i++)
	{
		flashSaveMemory[i] = 0xFF;
	}

	saveType = SAVETYPE::GBA_SAVE_AUTO;
	flashState = FLASHSTATE::FLASH_READ_ARRAY;
	flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
	flashSize = SIZE_FLASH512;
	flashDeviceID = 0x1b;
	flashManufacturerID = 0x32;
	flashBank = 0;
}

void FLASH::flashSetSize(int size)
{
	if (size == SIZE_FLASH512)
	{
		flashDeviceID = 0x1b;
		flashManufacturerID = 0x32;
	}
	else
	{
		flashDeviceID = 0x13; //0x09;
		flashManufacturerID = 0x62; //0xc2;
	}
	flashSize = size;
}

byte FLASH::flashRead(UInt32 address)
{
	address &= 0xFFFF;

	switch (flashReadState)
	{
	case FLASHSTATE::FLASH_READ_ARRAY:
		return flashSaveMemory[(flashBank << 16) + address];
	case FLASHSTATE::FLASH_AUTOSELECT:
		switch (address & 0xFF)
		{
		case 0:
			return flashManufacturerID;
		case 1:
			return flashDeviceID;
		}
		break;
	case FLASHSTATE::FLASH_ERASE_COMPLETE:
		flashState = FLASHSTATE::FLASH_READ_ARRAY;
		flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
		return 0xFF;
	};
	return 0;
}

void FLASH::flashSaveDecide(UInt32 address, byte data)
{
	if (saveType == SAVETYPE::GBA_SAVE_EEPROM)
	{
		return;
	}

	if (Memory.SramEnabled && Memory.FlashEnabled)
	{
		if (address == 0x0e005555)
		{
			saveType = SAVETYPE::GBA_SAVE_FLASH;
			Memory.SramEnabled = false;
		}
		else
		{
			saveType = SAVETYPE::GBA_SAVE_SRAM;
			Memory.FlashEnabled = false;
		}
	}
	
	if (saveType == SAVETYPE::GBA_SAVE_FLASH)
	{
		flashWrite(address, data);
	}
	else if (saveType == SAVETYPE::GBA_SAVE_SRAM)
	{
		sramWrite(address, data);
	}
}

void FLASH::sramWrite(UInt32 address, byte data)
{
	flashSaveMemory[address & 0xFFFF] = data;
}

void FLASH::flashWrite(UInt32 address, byte data)
{
	address &= 0xFFFF;
	switch (flashState)
	{
	case FLASHSTATE::FLASH_READ_ARRAY:
		if (address == 0x5555 && data == 0xAA)
			flashState = FLASHSTATE::FLASH_CMD_1;
		break;
	case FLASHSTATE::FLASH_CMD_1:
		if (address == 0x2AAA && data == 0x55)
			flashState = FLASHSTATE::FLASH_CMD_2;
		else
			flashState = FLASHSTATE::FLASH_READ_ARRAY;
		break;
	case FLASHSTATE::FLASH_CMD_2:
		if (address == 0x5555)
		{
			if (data == 0x90)
			{
				flashState = FLASHSTATE::FLASH_AUTOSELECT;
				flashReadState = FLASHSTATE::FLASH_AUTOSELECT;
			}
			else if (data == 0x80)
			{
				flashState = FLASHSTATE::FLASH_CMD_3;
			}
			else if (data == 0xF0)
			{
				flashState = FLASHSTATE::FLASH_READ_ARRAY;
				flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
			}
			else if (data == 0xA0)
			{
				flashState = FLASHSTATE::FLASH_PROGRAM;
			}
			else if (data == 0xB0 && flashSize == SIZE_FLASH1M)
			{
				flashState = FLASHSTATE::FLASH_SETBANK;
			}
			else
			{
				flashState = FLASHSTATE::FLASH_READ_ARRAY;
				flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
			}
		}
		else
		{
			flashState = FLASHSTATE::FLASH_READ_ARRAY;
			flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
		}
		break;
	case FLASHSTATE::FLASH_CMD_3:
		if (address == 0x5555 && data == 0xAA)
		{
			flashState = FLASHSTATE::FLASH_CMD_4;
		}
		else
		{
			flashState = FLASHSTATE::FLASH_READ_ARRAY;
			flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
		}
		break;
	case FLASHSTATE::FLASH_CMD_4:
		if (address == 0x2AAA && data == 0x55)
		{
			flashState = FLASHSTATE::FLASH_CMD_5;
		}
		else
		{
			flashState = FLASHSTATE::FLASH_READ_ARRAY;
			flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
		}
		break;
	case FLASHSTATE::FLASH_CMD_5: // SECTOR ERASE
		if (data == 0x30)
		{
			for (int i = 0; i < 0x1000; i++)
			{
				flashSaveMemory[(flashBank << 16) + (address & 0xF000) + i] = 0xFF;
			}
			flashReadState = FLASHSTATE::FLASH_ERASE_COMPLETE;
		}
		else if (data == 0x10) // CHIP ERASE
		{
			for (int i = 0; i < flashSize; i++)
			{
				flashSaveMemory[i] = 0xFF;
			}
			flashReadState = FLASHSTATE::FLASH_ERASE_COMPLETE;
		}
		else
		{
			flashState = FLASHSTATE::FLASH_READ_ARRAY;
			flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
		}
		break;
	case FLASHSTATE::FLASH_AUTOSELECT:
		if (data == 0xF0)
		{
			flashState = FLASHSTATE::FLASH_READ_ARRAY;
			flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
		}
		else if (address == 0x5555 && data == 0xAA)
			flashState = FLASHSTATE::FLASH_CMD_1;
		else
		{
			flashState = FLASHSTATE::FLASH_READ_ARRAY;
			flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
		}
		break;
	case FLASHSTATE::FLASH_PROGRAM:
		flashSaveMemory[(flashBank << 16) + address] = data;
		flashState = FLASHSTATE::FLASH_READ_ARRAY;
		flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
		break;
	case FLASHSTATE::FLASH_SETBANK:
		if (address == 0)
		{
			flashBank = (byte)(data & 1);
		}
		flashState = FLASHSTATE::FLASH_READ_ARRAY;
		flashReadState = FLASHSTATE::FLASH_READ_ARRAY;
		break;
	}
}