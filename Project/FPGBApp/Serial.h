#include "types.h"

class SERIAL
{
public:
	Int32 serialcnt;

	bool IRP_enable;
	bool start;
	bool hispeed;
	int bits;
	int bitcount;

	void reset();
	void work();
	void write_SIOCNT(UInt16 value);
};
extern SERIAL Serial;