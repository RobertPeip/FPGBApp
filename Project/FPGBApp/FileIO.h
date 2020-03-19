#pragma once
#include <string>
using namespace std;

#include "types.h"

class FILEIO
{
public:
	bool fileExists(string filename, bool absolutePath);
	int readfile(void* target, string filename, bool absolutePath);
	void writefile(void* source, string filename, int size, bool absolutePath);
};
extern FILEIO FileIO;