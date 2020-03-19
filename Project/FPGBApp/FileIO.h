#pragma once
#include <string>
using namespace std;

#include "types.h"

class FILEIO
{
public:
	int readfile(void* target, string filename, bool absolutePath);

};
extern FILEIO FileIO;