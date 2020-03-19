#include "EEProm.h"
#include "DMA.h"

EEPROM EEProm;

void EEPROM::reset()
{
	for (int i = 0; i < 8192; i++)
	{
		eepromData[i] = 0xFF;
	}

	eepromMode = EEPROMSTATE::EEPROM_IDLE;
	eepromBits = 0;
	eepromByte = 0;
	eepromAddress = 0;
}

void EEPROM::write(byte value)
{
	if (DMA.cpuDmaCount == 0)
	{
		return;
	}
	int bit = value & 1;
	switch (eepromMode)
	{
	case EEPROMSTATE::EEPROM_IDLE:
		eepromByte = 0;
		eepromBits = 1;
		eepromBuffer[eepromByte] = (byte)bit;
		eepromMode = EEPROMSTATE::EEPROM_READADDRESS;
		break;
	case EEPROMSTATE::EEPROM_READADDRESS:
		eepromBuffer[eepromByte] <<= 1;
		eepromBuffer[eepromByte] = (byte)(eepromBuffer[eepromByte] | bit);
		eepromBits++;
		if ((eepromBits & 7) == 0)
		{
			eepromByte++;
		}
		if (DMA.cpuDmaCount == 0x11 || DMA.cpuDmaCount == 0x51)
		{
			if (eepromBits == 0x11)
			{
				//eepromInUse = true;
				eepromAddress = ((eepromBuffer[0] & 0x3F) << 8) | ((eepromBuffer[1] & 0xFF));
				if ((eepromBuffer[0] & 0x40) == 0)
				{
					eepromBuffer[0] = (byte)bit;
					eepromBits = 1;
					eepromByte = 0;
					eepromMode = EEPROMSTATE::EEPROM_WRITEDATA;
				}
				else
				{
					eepromMode = EEPROMSTATE::EEPROM_READDATA;
					eepromByte = 0;
					eepromBits = 0;
				}
			}
		}
		else
		{
			if (eepromBits == 9)
			{
				//eepromInUse = true;
				eepromAddress = (eepromBuffer[0] & 0x3F);
				if ((eepromBuffer[0] & 0x40) == 0)
				{
					eepromBuffer[0] = (byte)bit;
					eepromBits = 1;
					eepromByte = 0;
					eepromMode = EEPROMSTATE::EEPROM_WRITEDATA;
				}
				else
				{
					eepromMode = EEPROMSTATE::EEPROM_READDATA;
					eepromByte = 0;
					eepromBits = 0;
				}
			}
		}
		break;
	case EEPROMSTATE::EEPROM_READDATA:
	case EEPROMSTATE::EEPROM_READDATA2:
		// should we reset here?
		eepromMode = EEPROMSTATE::EEPROM_IDLE;
		break;
	case EEPROMSTATE::EEPROM_WRITEDATA:
		eepromBuffer[eepromByte] <<= 1;
		eepromBuffer[eepromByte] = (byte)(eepromBuffer[eepromByte] | bit);
		eepromBits++;
		if ((eepromBits & 7) == 0)
		{
			eepromByte++;
		}
		if (eepromBits == 0x40)
		{
			//eepromInUse = true;
			// write data;
			for (int i = 0; i < 8; i++)
			{
				eepromData[(eepromAddress << 3) + i] = eepromBuffer[i];
			}
		}
		else if (eepromBits == 0x41)
		{
			eepromMode = EEPROMSTATE::EEPROM_IDLE;
			eepromByte = 0;
			eepromBits = 0;
		}
		break;
	}
}

int EEPROM::read()
{
	switch (eepromMode)
	{
	case EEPROMSTATE::EEPROM_IDLE:
	case EEPROMSTATE::EEPROM_READADDRESS:
	case EEPROMSTATE::EEPROM_WRITEDATA:
		return 1;
	case EEPROMSTATE::EEPROM_READDATA:
	{
		eepromBits++;
		if (eepromBits == 4)
		{
			eepromMode = EEPROMSTATE::EEPROM_READDATA2;
			eepromBits = 0;
			eepromByte = 0;
		}
		return 0;
	}
	case EEPROMSTATE::EEPROM_READDATA2:
	{
		int address = eepromAddress << 3;
		int mask = 1 << (7 - (eepromBits & 7));
		int data = 0;
		if ((eepromData[address + eepromByte] & mask) > 0)
		{
			data = 1;
		}
		eepromBits++;
		if ((eepromBits & 7) == 0)
		{
			eepromByte++;
		}
		if (eepromBits == 0x40)
		{
			eepromMode = EEPROMSTATE::EEPROM_IDLE;
		}
		return data;
	}
	default:
		return 0;
	}
}