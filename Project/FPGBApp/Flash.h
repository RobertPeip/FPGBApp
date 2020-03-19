#pragma once

#include "types.h"

enum class FLASHSTATE
{
	FLASH_READ_ARRAY,
	FLASH_CMD_1,
	FLASH_CMD_2,
	FLASH_AUTOSELECT,
	FLASH_CMD_3,
	FLASH_CMD_4,
	FLASH_CMD_5,
	FLASH_ERASE_COMPLETE,
	FLASH_PROGRAM,
	FLASH_SETBANK
};

enum class SAVETYPE
{
	GBA_SAVE_AUTO,
	GBA_SAVE_EEPROM,
	GBA_SAVE_SRAM,
	GBA_SAVE_FLASH,
	GBA_SAVE_EEPROM_SENSOR,
	GBA_SAVE_NONE
};

class FLASH
{
public:
	const int SIZE_FLASH1M = 131072;
	const int SIZE_FLASH512 = 65536;

	byte flashSaveMemory[131072];

	SAVETYPE saveType;

	FLASHSTATE flashState;
	FLASHSTATE flashReadState;
	byte flashBank = 0;

	void reset();
	void flashSetSize(int size);
	byte flashRead(UInt32 address);
	void flashSaveDecide(UInt32 address, byte data);
	void sramWrite(UInt32 address, byte data);
	void flashWrite(UInt32 address, byte data);

private:
	int flashSize;
	byte flashDeviceID;
	byte flashManufacturerID;

};
extern FLASH Flash;