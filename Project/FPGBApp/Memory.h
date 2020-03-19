#pragma once
#include <string>
using namespace std;

#include "types.h"

class MEMORY
{
public:
	bool has_tilt = false;
	bool gpio_enable = false;

	Byte WRAM_Large[262144];
	Byte WRAM_Small[32768];
	Byte VRAM[98304];
	Byte OAMRAM[1024];
	Byte PaletteRAM[1024];
	Byte GameRom[33554432];

	UInt32 GameRom_max;
	byte blockcmd_lowerbits;

	Byte GameRamSnapshot[131072 + 8192];
	bool createGameRAMSnapshot;

	Byte GBRom[16384];

	bool EEPROMEnabled;
	bool FlashEnabled;
	bool SramEnabled;
	bool EEPROMSensorEnabled;
	//Func<UInt32, byte, object> SaveGameFunc;

	byte biosProtected[4];

	UInt16 tiltx;
	UInt16 tilty;

	bool unreadable;
	UInt32 lastreadvalue;

	bool gpio_used;

	void reset(string filename);
	void GameRAMSnapshot();
	void load_gameram();
	void save_gameram();
	byte read_unreadable_byte(UInt32 offset);
	UInt16 read_unreadable_word();
	UInt32 read_unreadable_dword();
	byte read_byte(UInt32 address);
	UInt32 read_word(UInt32 address);
	UInt32 read_dword(UInt32 address);
	void write_byte(UInt32 address, byte data);
	void write_word(UInt32 address, UInt16 data);
	void write_dword(UInt32 address, UInt32 data);
	void prepare_read_gbreg(UInt32 adr);
	void write_gbreg(UInt32 adr, UInt32 value, bool dwaccess);
};
extern MEMORY Memory;