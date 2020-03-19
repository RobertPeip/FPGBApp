#pragma once

#include "types.h"

enum class EEPROMSTATE
{
	EEPROM_IDLE,
	EEPROM_READADDRESS,
	EEPROM_READDATA,
	EEPROM_READDATA2,
	EEPROM_WRITEDATA
};

class EEPROM
{
public:
	byte eepromData[8192];
	byte eepromBuffer[16];

	EEPROMSTATE eepromMode;
	int eepromBits;
	int eepromByte;
	int eepromAddress;

	void reset();
	void write(byte value);
	int read();


};
extern EEPROM EEProm;