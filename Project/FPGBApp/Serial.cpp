#include "Serial.h"
#include "CPU.h"
#include "GBRegs.h"
#include "IRP.h"

SERIAL Serial;

void SERIAL::reset()
{
	serialcnt = 0;
	IRP_enable = false;
	start = false;
	hispeed = false;
	int bits = 0;
	int bitcount = 0;
}

void SERIAL::work()
{
	if (start)
	{
		serialcnt += CPU.newticks;

		if ((!hispeed && serialcnt >= 64) || (hispeed && serialcnt >= 8))
		{
			if (hispeed)
			{
				serialcnt -= 8;
			}
			else
			{
				serialcnt -= 64;
			}
			bitcount++;
			if (bitcount == bits)
			{
				if (IRP_enable)
				{
					IRP.set_irp_bit(IRP.IRPMASK_Serial);
				}
				GBRegs.Sect_serial.SIOCNT.mask(0xDF7F);
				start = false;
			}
		}
	}
}

void SERIAL::write_SIOCNT(UInt16 value)
{
	IRP_enable = ((value >> 14) & 1) == 1;

	hispeed = ((value >> 1) & 1) == 1;

	start = ((value >> 7) & 1) == 1;
	if (((value >> 12) & 1) == 1)
	{
		bits = 32;
	}
	else
	{
		bits = 8;
	}
	if (start)
	{
		bitcount = 0;
		serialcnt = 0;
	}
}