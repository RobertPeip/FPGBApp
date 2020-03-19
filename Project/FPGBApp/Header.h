#pragma once

#include "types.h"

class HEADER
{
public:
	void autocheck();
	void check_flash_size();
	bool FindArray(byte* array, unsigned int arraylength, byte* pattern, int patternlength);
};
extern HEADER Header;