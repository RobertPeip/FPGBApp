#pragma once

#include "types.h"

class Gpio
{
public:
	bool gpioWrite(uint address, ushort value);
	ushort gpioRead(uint address);

};
extern Gpio gpio;