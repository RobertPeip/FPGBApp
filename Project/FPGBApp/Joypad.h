#pragma once

#include "types.h"

class JOYPAD
{
public:
	bool KeyA;
	bool KeyB;
	bool KeyL;
	bool KeyR;
	bool KeyStart;
	bool KeySelect;
	bool KeyUp;
	bool KeyDown;
	bool KeyLeft;
	bool KeyRight;

	bool KeyAToggle;
	bool KeyBToggle;

	UInt16 oldvalue;

	void set_reg(bool forceIRQcheck);
	void check_irp();

private:
	UInt16 value;
};
extern JOYPAD Joypad;