#pragma once

#include "types.h"

enum class GPIOState
{
	IDLE,
	COMMAND,
	DATA,
	READDATA
};

class Gpio
{
public:
	byte retval; // 4 bit
	byte select; // 4 bit
	byte enable; // 1 bit
	byte command; // 8 bit
	int dataLen; // 3 bit
	int bits; // 6 bit (0..55/56)
	GPIOState state; // 2 bit
	byte data[7];     // 7 * 8 bit 
	byte clockslow; // 8 bit

	bool gpioWrite(uint address, ushort value);
	ushort gpioRead(uint address);

};
extern Gpio gpio;