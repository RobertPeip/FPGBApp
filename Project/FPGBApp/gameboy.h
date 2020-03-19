#pragma once
#include <string>
using namespace std;

#include "types.h"

class Gameboy
{
public:
	bool on = false;
	bool coldreset = false;

	string filename;

	Int64 cycles = 0;

	string statefilename;
	bool do_savestate = false;
	bool do_loadstate = false;
	bool loading_state;
	UInt32 savestate[131072];

	void reset();
	void run();
	void create_savestate();
	void load_savestate();
};
extern Gameboy gameboy;