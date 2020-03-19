#include "GBRegs.h"

GBREGS GBRegs;

GBReg::GBReg()
{
	address = 0;
	count = 0;
	defaultvalue = 0;
	lsb = 0;
	msb = 0;
}

GBReg::GBReg(UInt32 address, byte msb, byte lsb, UInt16 count, UInt32 defaultvalue, string accesstype)
{
	this->address = address;
	this->msb = msb;
	this->lsb = lsb;
	this->count = count;
	this->defaultvalue = defaultvalue;
}

void GBReg::write(UInt32 value)
{
	UInt32 newvalue = *(UInt32*)&GBRegs.data[address];
	UInt32 filter = ~((((UInt32)pow(2, msb + 1)) - 1) - (((UInt32)pow(2, lsb)) - 1));
	newvalue = newvalue & filter;
	newvalue |= value << lsb;

	write_reg(newvalue);
}

UInt32 GBReg::read()
{
	UInt32 value = *(UInt32*)&GBRegs.data[address];
	UInt32 filter = ((((UInt32)pow(2, msb + 1)) - 1) - (((UInt32)pow(2, lsb)) - 1));
	value = (value & filter) >> lsb;

	return value;
}

bool GBReg::on()
{
	UInt32 value = *(UInt32*)&GBRegs.data[address];
	return (((value >> lsb) & 1) == 1);
}

void GBReg::mask(UInt32 mask)
{
	UInt32 value = *(UInt32*)&GBRegs.data[address];
	write_reg(value & mask);
}

void GBReg::write_reg(UInt32 value)
{
	if (msb < 8)
	{
		GBRegs.data[address] = (byte)value;
	}
	else if (msb < 16)
	{
		GBRegs.data[address] = (byte)(value & 0xFF);
		GBRegs.data[address + 1] = (byte)((value >> 8) & 0xFF);
	}
	else
	{
		GBRegs.data[address] = (byte)(value & 0xFF);
		GBRegs.data[address + 1] = (byte)((value >> 8) & 0xFF);
		GBRegs.data[address + 2] = (byte)((value >> 16) & 0xFF);
		GBRegs.data[address + 3] = (byte)((value >> 24) & 0xFF);
	}
}

RegSect_display::RegSect_display()
{
	DISPCNT = GBReg(0x000, 15, 0, 1, 0x0080, "readwrite");
	DISPCNT_BG_Mode = GBReg(0x000, 2, 0, 1, 0, "readwrite");
	DISPCNT_Reserved_CGB_Mode = GBReg(0x000, 3, 3, 1, 0, "readwrite");
	DISPCNT_Display_Frame_Select = GBReg(0x000, 4, 4, 1, 0, "readwrite");
	DISPCNT_H_Blank_IntervalFree = GBReg(0x000, 5, 5, 1, 0, "readwrite");
	DISPCNT_OBJ_Char_VRAM_Map = GBReg(0x000, 6, 6, 1, 0, "readwrite");
	DISPCNT_Forced_Blank = GBReg(0x000, 7, 7, 1, 0, "readwrite");
	DISPCNT_Screen_Display_BG0 = GBReg(0x000, 8, 8, 1, 0, "readwrite");
	DISPCNT_Screen_Display_BG1 = GBReg(0x000, 9, 9, 1, 0, "readwrite");
	DISPCNT_Screen_Display_BG2 = GBReg(0x000, 10, 10, 1, 0, "readwrite");
	DISPCNT_Screen_Display_BG3 = GBReg(0x000, 11, 11, 1, 0, "readwrite");
	DISPCNT_Screen_Display_OBJ = GBReg(0x000, 12, 12, 1, 0, "readwrite");
	DISPCNT_Window_0_Display_Flag = GBReg(0x000, 13, 13, 1, 0, "readwrite");
	DISPCNT_Window_1_Display_Flag = GBReg(0x000, 14, 14, 1, 0, "readwrite");
	DISPCNT_OBJ_Wnd_Display_Flag = GBReg(0x000, 15, 15, 1, 0, "readwrite");
	GREENSWAP = GBReg(0x000, 31, 16, 1, 0, "readwrite");
	DISPSTAT = GBReg(0x004, 15, 0, 1, 0, "readwrite");
	DISPSTAT_V_Blank_flag = GBReg(0x004, 0, 0, 1, 0, "readonly");
	DISPSTAT_H_Blank_flag = GBReg(0x004, 1, 1, 1, 0, "readonly");
	DISPSTAT_V_Counter_flag = GBReg(0x004, 2, 2, 1, 0, "readonly");
	DISPSTAT_V_Blank_IRQ_Enable = GBReg(0x004, 3, 3, 1, 0, "readwrite");
	DISPSTAT_H_Blank_IRQ_Enable = GBReg(0x004, 4, 4, 1, 0, "readwrite");
	DISPSTAT_V_Counter_IRQ_Enable = GBReg(0x004, 5, 5, 1, 0, "readwrite");
	DISPSTAT_V_Count_Setting = GBReg(0x004, 15, 8, 1, 0, "readwrite");
	VCOUNT = GBReg(0x004, 31, 16, 1, 0, "readwrite");
	BG0CNT = GBReg(0x008, 15, 0, 1, 0, "writeonly");
	BG0CNT_BG_Priority = GBReg(0x008, 1, 0, 1, 0, "readwrite");
	BG0CNT_Character_Base_Block = GBReg(0x008, 3, 2, 1, 0, "readwrite");
	BG0CNT_UNUSED_4_5 = GBReg(0x008, 5, 4, 1, 0, "readwrite");
	BG0CNT_Mosaic = GBReg(0x008, 6, 6, 1, 0, "readwrite");
	BG0CNT_Colors_Palettes = GBReg(0x008, 7, 7, 1, 0, "readwrite");
	BG0CNT_Screen_Base_Block = GBReg(0x008, 12, 8, 1, 0, "readwrite");
	BG0CNT_Screen_Size = GBReg(0x008, 15, 14, 1, 0, "readwrite");
	BG1CNT = GBReg(0x00A, 15, 0, 1, 0, "writeonly");
	BG1CNT_BG_Priority = GBReg(0x00A, 1, 0, 1, 0, "readwrite");
	BG1CNT_Character_Base_Block = GBReg(0x00A, 3, 2, 1, 0, "readwrite");
	BG1CNT_UNUSED_4_5 = GBReg(0x00A, 5, 4, 1, 0, "readwrite");
	BG1CNT_Mosaic = GBReg(0x00A, 6, 6, 1, 0, "readwrite");
	BG1CNT_Colors_Palettes = GBReg(0x00A, 7, 7, 1, 0, "readwrite");
	BG1CNT_Screen_Base_Block = GBReg(0x00A, 12, 8, 1, 0, "readwrite");
	BG1CNT_Screen_Size = GBReg(0x00A, 15, 14, 1, 0, "readwrite");
	BG2CNT = GBReg(0x00C, 15, 0, 1, 0, "readwrite");
	BG2CNT_BG_Priority = GBReg(0x00C, 1, 0, 1, 0, "readwrite");
	BG2CNT_Character_Base_Block = GBReg(0x00C, 3, 2, 1, 0, "readwrite");
	BG2CNT_Mosaic = GBReg(0x00C, 6, 6, 1, 0, "readwrite");
	BG2CNT_Colors_Palettes = GBReg(0x00C, 7, 7, 1, 0, "readwrite");
	BG2CNT_Screen_Base_Block = GBReg(0x00C, 12, 8, 1, 0, "readwrite");
	BG2CNT_Display_Area_Overflow = GBReg(0x00C, 13, 13, 1, 0, "readwrite");
	BG2CNT_Screen_Size = GBReg(0x00C, 15, 14, 1, 0, "readwrite");
	BG3CNT = GBReg(0x00E, 15, 0, 1, 0, "readwrite");
	BG3CNT_BG_Priority = GBReg(0x00E, 1, 0, 1, 0, "readwrite");
	BG3CNT_Character_Base_Block = GBReg(0x00E, 3, 2, 1, 0, "readwrite");
	BG3CNT_Mosaic = GBReg(0x00E, 6, 6, 1, 0, "readwrite");
	BG3CNT_Colors_Palettes = GBReg(0x00E, 7, 7, 1, 0, "readwrite");
	BG3CNT_Screen_Base_Block = GBReg(0x00E, 12, 8, 1, 0, "readwrite");
	BG3CNT_Display_Area_Overflow = GBReg(0x00E, 13, 13, 1, 0, "readwrite");
	BG3CNT_Screen_Size = GBReg(0x00E, 15, 14, 1, 0, "readwrite");
	BG0HOFS = GBReg(0x010, 15, 0, 1, 0, "writeonly");
	BG0VOFS = GBReg(0x012, 15, 0, 1, 0, "writeonly");
	BG1HOFS = GBReg(0x014, 15, 0, 1, 0, "writeonly");
	BG1VOFS = GBReg(0x016, 15, 0, 1, 0, "writeonly");
	BG2HOFS = GBReg(0x018, 15, 0, 1, 0, "writeonly");
	BG2VOFS = GBReg(0x01A, 15, 0, 1, 0, "writeonly");
	BG3HOFS = GBReg(0x01C, 15, 0, 1, 0, "writeonly");
	BG3VOFS = GBReg(0x01E, 15, 0, 1, 0, "writeonly");
	BG2RotScaleParDX = GBReg(0x020, 15, 0, 1, 256, "writeonly");
	BG2RotScaleParDMX = GBReg(0x020, 31, 16, 1, 0, "writeonly");
	BG2RotScaleParDY = GBReg(0x024, 15, 0, 1, 0, "writeonly");
	BG2RotScaleParDMY = GBReg(0x024, 31, 16, 1, 256, "writeonly");
	BG2RefX = GBReg(0x028, 27, 0, 1, 0, "writeonly");
	BG2RefY = GBReg(0x02C, 27, 0, 1, 0, "writeonly");
	BG3RotScaleParDX = GBReg(0x030, 15, 0, 1, 256, "writeonly");
	BG3RotScaleParDMX = GBReg(0x030, 31, 16, 1, 0, "writeonly");
	BG3RotScaleParDY = GBReg(0x034, 15, 0, 1, 0, "writeonly");
	BG3RotScaleParDMY = GBReg(0x034, 31, 16, 1, 256, "writeonly");
	BG3RefX = GBReg(0x038, 27, 0, 1, 0, "writeonly");
	BG3RefY = GBReg(0x03C, 27, 0, 1, 0, "writeonly");
	WIN0H = GBReg(0x040, 15, 0, 1, 0, "writeonly");
	WIN0H_X2 = GBReg(0x040, 7, 0, 1, 0, "writeonly");
	WIN0H_X1 = GBReg(0x040, 15, 8, 1, 0, "writeonly");
	WIN1H = GBReg(0x040, 31, 16, 1, 0, "writeonly");
	WIN1H_X2 = GBReg(0x040, 23, 16, 1, 0, "writeonly");
	WIN1H_X1 = GBReg(0x040, 31, 24, 1, 0, "writeonly");
	WIN0V = GBReg(0x044, 15, 0, 1, 0, "writeonly");
	WIN0V_Y2 = GBReg(0x044, 7, 0, 1, 0, "writeonly");
	WIN0V_Y1 = GBReg(0x044, 15, 8, 1, 0, "writeonly");
	WIN1V = GBReg(0x044, 31, 16, 1, 0, "writeonly");
	WIN1V_Y2 = GBReg(0x044, 23, 16, 1, 0, "writeonly");
	WIN1V_Y1 = GBReg(0x044, 31, 24, 1, 0, "writeonly");
	WININ = GBReg(0x048, 15, 0, 1, 0, "writeonly");
	WININ_Window_0_BG0_Enable = GBReg(0x048, 0, 0, 1, 0, "readwrite");
	WININ_Window_0_BG1_Enable = GBReg(0x048, 1, 1, 1, 0, "readwrite");
	WININ_Window_0_BG2_Enable = GBReg(0x048, 2, 2, 1, 0, "readwrite");
	WININ_Window_0_BG3_Enable = GBReg(0x048, 3, 3, 1, 0, "readwrite");
	WININ_Window_0_OBJ_Enable = GBReg(0x048, 4, 4, 1, 0, "readwrite");
	WININ_Window_0_Special_Effect = GBReg(0x048, 5, 5, 1, 0, "readwrite");
	WININ_Window_1_BG0_Enable = GBReg(0x048, 8, 8, 1, 0, "readwrite");
	WININ_Window_1_BG1_Enable = GBReg(0x048, 9, 9, 1, 0, "readwrite");
	WININ_Window_1_BG2_Enable = GBReg(0x048, 10, 10, 1, 0, "readwrite");
	WININ_Window_1_BG3_Enable = GBReg(0x048, 11, 11, 1, 0, "readwrite");
	WININ_Window_1_OBJ_Enable = GBReg(0x048, 12, 12, 1, 0, "readwrite");
	WININ_Window_1_Special_Effect = GBReg(0x048, 13, 13, 1, 0, "readwrite");
	WINOUT = GBReg(0x048, 31, 16, 1, 0, "writeonly");
	WINOUT_Outside_BG0_Enable = GBReg(0x048, 16, 16, 1, 0, "readwrite");
	WINOUT_Outside_BG1_Enable = GBReg(0x048, 17, 17, 1, 0, "readwrite");
	WINOUT_Outside_BG2_Enable = GBReg(0x048, 18, 18, 1, 0, "readwrite");
	WINOUT_Outside_BG3_Enable = GBReg(0x048, 19, 19, 1, 0, "readwrite");
	WINOUT_Outside_OBJ_Enable = GBReg(0x048, 20, 20, 1, 0, "readwrite");
	WINOUT_Outside_Special_Effect = GBReg(0x048, 21, 21, 1, 0, "readwrite");
	WINOUT_Objwnd_BG0_Enable = GBReg(0x048, 24, 24, 1, 0, "readwrite");
	WINOUT_Objwnd_BG1_Enable = GBReg(0x048, 25, 25, 1, 0, "readwrite");
	WINOUT_Objwnd_BG2_Enable = GBReg(0x048, 26, 26, 1, 0, "readwrite");
	WINOUT_Objwnd_BG3_Enable = GBReg(0x048, 27, 27, 1, 0, "readwrite");
	WINOUT_Objwnd_OBJ_Enable = GBReg(0x048, 28, 28, 1, 0, "readwrite");
	WINOUT_Objwnd_Special_Effect = GBReg(0x048, 29, 29, 1, 0, "readwrite");
	MOSAIC = GBReg(0x04C, 15, 0, 1, 0, "writeonly");
	MOSAIC_BG_Mosaic_H_Size = GBReg(0x04C, 3, 0, 1, 0, "writeonly");
	MOSAIC_BG_Mosaic_V_Size = GBReg(0x04C, 7, 4, 1, 0, "writeonly");
	MOSAIC_OBJ_Mosaic_H_Size = GBReg(0x04C, 11, 8, 1, 0, "writeonly");
	MOSAIC_OBJ_Mosaic_V_Size = GBReg(0x04C, 15, 12, 1, 0, "writeonly");
	BLDCNT = GBReg(0x050, 13, 0, 1, 0, "readwrite");
	BLDCNT_BG0_1st_Target_Pixel = GBReg(0x050, 0, 0, 1, 0, "readwrite");
	BLDCNT_BG1_1st_Target_Pixel = GBReg(0x050, 1, 1, 1, 0, "readwrite");
	BLDCNT_BG2_1st_Target_Pixel = GBReg(0x050, 2, 2, 1, 0, "readwrite");
	BLDCNT_BG3_1st_Target_Pixel = GBReg(0x050, 3, 3, 1, 0, "readwrite");
	BLDCNT_OBJ_1st_Target_Pixel = GBReg(0x050, 4, 4, 1, 0, "readwrite");
	BLDCNT_BD_1st_Target_Pixel = GBReg(0x050, 5, 5, 1, 0, "readwrite");
	BLDCNT_Color_Special_Effect = GBReg(0x050, 7, 6, 1, 0, "readwrite");
	BLDCNT_BG0_2nd_Target_Pixel = GBReg(0x050, 8, 8, 1, 0, "readwrite");
	BLDCNT_BG1_2nd_Target_Pixel = GBReg(0x050, 9, 9, 1, 0, "readwrite");
	BLDCNT_BG2_2nd_Target_Pixel = GBReg(0x050, 10, 10, 1, 0, "readwrite");
	BLDCNT_BG3_2nd_Target_Pixel = GBReg(0x050, 11, 11, 1, 0, "readwrite");
	BLDCNT_OBJ_2nd_Target_Pixel = GBReg(0x050, 12, 12, 1, 0, "readwrite");
	BLDCNT_BD_2nd_Target_Pixel = GBReg(0x050, 13, 13, 1, 0, "readwrite");
	BLDALPHA = GBReg(0x050, 28, 16, 1, 0, "writeonly");
	BLDALPHA_EVA_Coefficient = GBReg(0x050, 20, 16, 1, 0, "readwrite");
	BLDALPHA_EVB_Coefficient = GBReg(0x050, 28, 24, 1, 0, "readwrite");
	BLDY = GBReg(0x054, 4, 0, 1, 0, "writeonly");
}

RegSect_sound::RegSect_sound()
{
	SOUND1CNT_L = GBReg(0x060, 6, 0, 1, 0, "readwrite");
	SOUND1CNT_L_Number_of_sweep_shift = GBReg(0x060, 2, 0, 1, 0, "readwrite");
	SOUND1CNT_L_Sweep_Frequency_Direction = GBReg(0x060, 3, 3, 1, 0, "readwrite");
	SOUND1CNT_L_Sweep_Time = GBReg(0x060, 6, 4, 1, 0, "readwrite");
	SOUND1CNT_H = GBReg(0x060, 31, 16, 1, 0, "writeonly");
	SOUND1CNT_H_Sound_length = GBReg(0x060, 21, 16, 1, 0, "writeonly");
	SOUND1CNT_H_Wave_Pattern_Duty = GBReg(0x060, 23, 22, 1, 0, "readwrite");
	SOUND1CNT_H_Envelope_Step_Time = GBReg(0x060, 26, 24, 1, 0, "readwrite");
	SOUND1CNT_H_Envelope_Direction = GBReg(0x060, 27, 27, 1, 0, "readwrite");
	SOUND1CNT_H_Initial_Volume_of_envelope = GBReg(0x060, 31, 28, 1, 0, "readwrite");
	SOUND1CNT_X = GBReg(0x064, 15, 0, 1, 0, "writeonly");
	SOUND1CNT_X_Frequency = GBReg(0x064, 10, 0, 1, 0, "writeonly");
	SOUND1CNT_X_Length_Flag = GBReg(0x064, 14, 14, 1, 0, "readwrite");
	SOUND1CNT_X_Initial = GBReg(0x064, 15, 15, 1, 0, "writeonly");
	SOUND1CNT_XHighZero = GBReg(0x064, 31, 16, 1, 0, "readonly");
	SOUND2CNT_L = GBReg(0x068, 15, 0, 1, 0, "writeonly");
	SOUND2CNT_L_Sound_length = GBReg(0x068, 5, 0, 1, 0, "writeonly");
	SOUND2CNT_L_Wave_Pattern_Duty = GBReg(0x068, 7, 6, 1, 0, "readwrite");
	SOUND2CNT_L_Envelope_Step_Time = GBReg(0x068, 10, 8, 1, 0, "readwrite");
	SOUND2CNT_L_Envelope_Direction = GBReg(0x068, 11, 11, 1, 0, "readwrite");
	SOUND2CNT_L_Initial_Volume_of_envelope = GBReg(0x068, 15, 12, 1, 0, "readwrite");
	SOUND2CNT_H = GBReg(0x06C, 15, 0, 1, 0, "writeonly");
	SOUND2CNT_H_Frequency = GBReg(0x06C, 10, 0, 1, 0, "writeonly");
	SOUND2CNT_H_Length_Flag = GBReg(0x06C, 14, 14, 1, 0, "readwrite");
	SOUND2CNT_H_Initial = GBReg(0x06C, 15, 15, 1, 0, "writeonly");
	SOUND2CNT_HHighZero = GBReg(0x06C, 31, 16, 1, 0, "readonly");
	SOUND3CNT_L = GBReg(0x070, 15, 0, 1, 0, "writeonly");
	SOUND3CNT_L_Wave_RAM_Dimension = GBReg(0x070, 5, 5, 1, 0, "readwrite");
	SOUND3CNT_L_Wave_RAM_Bank_Number = GBReg(0x070, 6, 6, 1, 0, "readwrite");
	SOUND3CNT_L_Sound_Channel_3_Off = GBReg(0x070, 7, 7, 1, 0, "readwrite");
	SOUND3CNT_H = GBReg(0x070, 31, 16, 1, 0, "writeonly");
	SOUND3CNT_H_Sound_length = GBReg(0x070, 23, 16, 1, 0, "writeonly");
	SOUND3CNT_H_Sound_Volume = GBReg(0x070, 30, 29, 1, 0, "readwrite");
	SOUND3CNT_H_Force_Volume = GBReg(0x070, 31, 31, 1, 0, "readwrite");
	SOUND3CNT_X = GBReg(0x074, 15, 0, 1, 0, "writeonly");
	SOUND3CNT_X_Sample_Rate = GBReg(0x074, 10, 0, 1, 0, "writeonly");
	SOUND3CNT_X_Length_Flag = GBReg(0x074, 14, 14, 1, 0, "readwrite");
	SOUND3CNT_X_Initial = GBReg(0x074, 15, 15, 1, 0, "writeonly");
	SOUND3CNT_XHighZero = GBReg(0x074, 31, 16, 1, 0, "readonly");
	SOUND4CNT_L = GBReg(0x078, 15, 0, 1, 0, "writeonly");
	SOUND4CNT_L_Sound_length = GBReg(0x078, 5, 0, 1, 0, "writeonly");
	SOUND4CNT_L_Envelope_Step_Time = GBReg(0x078, 10, 8, 1, 0, "readwrite");
	SOUND4CNT_L_Envelope_Direction = GBReg(0x078, 11, 11, 1, 0, "readwrite");
	SOUND4CNT_L_Initial_Volume_of_envelope = GBReg(0x078, 15, 12, 1, 0, "readwrite");
	SOUND4CNT_LHighZero = GBReg(0x078, 31, 16, 1, 0, "readonly");
	SOUND4CNT_H = GBReg(0x07C, 15, 0, 1, 0, "writeonly");
	SOUND4CNT_H_Dividing_Ratio_of_Freq = GBReg(0x07C, 2, 0, 1, 0, "readwrite");
	SOUND4CNT_H_Counter_Step_Width = GBReg(0x07C, 3, 3, 1, 0, "readwrite");
	SOUND4CNT_H_Shift_Clock_Frequency = GBReg(0x07C, 7, 4, 1, 0, "readwrite");
	SOUND4CNT_H_Length_Flag = GBReg(0x07C, 14, 14, 1, 0, "readwrite");
	SOUND4CNT_H_Initial = GBReg(0x07C, 15, 15, 1, 0, "writeonly");
	SOUND4CNT_HHighZero = GBReg(0x07C, 31, 16, 1, 0, "readonly");
	SOUNDCNT_L = GBReg(0x080, 15, 0, 1, 0, "writeonly");
	SOUNDCNT_L_Sound_1_4_Master_Volume_RIGHT = GBReg(0x080, 2, 0, 1, 0, "readwrite");
	SOUNDCNT_L_Sound_1_4_Master_Volume_LEFT = GBReg(0x080, 6, 4, 1, 0, "readwrite");
	SOUNDCNT_L_Sound_1_Enable_Flags_RIGHT = GBReg(0x080, 8, 8, 1, 0, "readwrite");
	SOUNDCNT_L_Sound_2_Enable_Flags_RIGHT = GBReg(0x080, 9, 9, 1, 0, "readwrite");
	SOUNDCNT_L_Sound_3_Enable_Flags_RIGHT = GBReg(0x080, 10, 10, 1, 0, "readwrite");
	SOUNDCNT_L_Sound_4_Enable_Flags_RIGHT = GBReg(0x080, 11, 11, 1, 0, "readwrite");
	SOUNDCNT_L_Sound_1_Enable_Flags_LEFT = GBReg(0x080, 12, 12, 1, 0, "readwrite");
	SOUNDCNT_L_Sound_2_Enable_Flags_LEFT = GBReg(0x080, 13, 13, 1, 0, "readwrite");
	SOUNDCNT_L_Sound_3_Enable_Flags_LEFT = GBReg(0x080, 14, 14, 1, 0, "readwrite");
	SOUNDCNT_L_Sound_4_Enable_Flags_LEFT = GBReg(0x080, 15, 15, 1, 0, "readwrite");
	SOUNDCNT_H = GBReg(0x080, 31, 16, 1, 0, "readwrite");
	SOUNDCNT_H_Sound_1_4_Volume = GBReg(0x080, 17, 16, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_A_Volume = GBReg(0x080, 18, 18, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_B_Volume = GBReg(0x080, 19, 19, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_A_Enable_RIGHT = GBReg(0x080, 24, 24, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_A_Enable_LEFT = GBReg(0x080, 25, 25, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_A_Timer_Select = GBReg(0x080, 26, 26, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_A_Reset_FIFO = GBReg(0x080, 27, 27, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_B_Enable_RIGHT = GBReg(0x080, 28, 28, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_B_Enable_LEFT = GBReg(0x080, 29, 29, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_B_Timer_Select = GBReg(0x080, 30, 30, 1, 0, "readwrite");
	SOUNDCNT_H_DMA_Sound_B_Reset_FIFO = GBReg(0x080, 31, 31, 1, 0, "readwrite");
	SOUNDCNT_X = GBReg(0x084, 7, 0, 1, 0, "readwrite");
	SOUNDCNT_X_Sound_1_ON_flag = GBReg(0x084, 0, 0, 1, 0, "readwrite");
	SOUNDCNT_X_Sound_2_ON_flag = GBReg(0x084, 1, 1, 1, 0, "readwrite");
	SOUNDCNT_X_Sound_3_ON_flag = GBReg(0x084, 2, 2, 1, 0, "readwrite");
	SOUNDCNT_X_Sound_4_ON_flag = GBReg(0x084, 3, 3, 1, 0, "readwrite");
	SOUNDCNT_X_PSG_FIFO_Master_Enable = GBReg(0x084, 7, 7, 1, 0, "readwrite");
	SOUNDCNT_XHighZero = GBReg(0x084, 31, 16, 1, 0, "readonly");
	SOUNDBIAS = GBReg(0x088, 15, 0, 1, 0x0200, "readwrite");
	SOUNDBIAS_Bias_Level = GBReg(0x088, 9, 0, 1, 0, "readwrite");
	SOUNDBIAS_Amp_Res_Sampling_Cycle = GBReg(0x088, 15, 14, 1, 0, "readwrite");
	SOUNDBIAS_HighZero = GBReg(0x088, 31, 16, 1, 0, "readonly");
	WAVE_RAM = GBReg(0x090, 31, 0, 4, 0, "readwrite");
	WAVE_RAM2 = GBReg(0x094, 31, 0, 1, 0, "readwrite");
	WAVE_RAM3 = GBReg(0x098, 31, 0, 1, 0, "readwrite");
	WAVE_RAM4 = GBReg(0x09C, 31, 0, 1, 0, "readwrite");
	FIFO_A = GBReg(0x0A0, 31, 0, 1, 0, "writeonly");
	FIFO_B = GBReg(0x0A4, 31, 0, 1, 0, "writeonly");
}

RegSect_dma::RegSect_dma()
{
	DMA0SAD = GBReg(0xB0, 31, 0, 1, 0, "writeonly");
	DMA0DAD = GBReg(0xB4, 31, 0, 1, 0, "writeonly");
	DMA0CNT_L = GBReg(0xB8, 15, 0, 1, 0, "writeonly");
	DMA0CNT_H = GBReg(0xB8, 31, 16, 1, 0, "writeonly");
	DMA0CNT_H_Dest_Addr_Control = GBReg(0xB8, 22, 21, 1, 0, "readwrite");
	DMA0CNT_H_Source_Adr_Control = GBReg(0xB8, 24, 23, 1, 0, "readwrite");
	DMA0CNT_H_DMA_Repeat = GBReg(0xB8, 25, 25, 1, 0, "readwrite");
	DMA0CNT_H_DMA_Transfer_Type = GBReg(0xB8, 26, 26, 1, 0, "readwrite");
	DMA0CNT_H_DMA_Start_Timing = GBReg(0xB8, 29, 28, 1, 0, "readwrite");
	DMA0CNT_H_IRQ_on = GBReg(0xB8, 30, 30, 1, 0, "readwrite");
	DMA0CNT_H_DMA_Enable = GBReg(0xB8, 31, 31, 1, 0, "readwrite");
	DMA1SAD = GBReg(0xBC, 31, 0, 1, 0, "writeonly");
	DMA1DAD = GBReg(0xC0, 31, 0, 1, 0, "writeonly");
	DMA1CNT_L = GBReg(0xC4, 15, 0, 1, 0, "writeonly");
	DMA1CNT_H = GBReg(0xC4, 31, 16, 1, 0, "writeonly");
	DMA1CNT_H_Dest_Addr_Control = GBReg(0xC4, 22, 21, 1, 0, "readwrite");
	DMA1CNT_H_Source_Adr_Control = GBReg(0xC4, 24, 23, 1, 0, "readwrite");
	DMA1CNT_H_DMA_Repeat = GBReg(0xC4, 25, 25, 1, 0, "readwrite");
	DMA1CNT_H_DMA_Transfer_Type = GBReg(0xC4, 26, 26, 1, 0, "readwrite");
	DMA1CNT_H_DMA_Start_Timing = GBReg(0xC4, 29, 28, 1, 0, "readwrite");
	DMA1CNT_H_IRQ_on = GBReg(0xC4, 30, 30, 1, 0, "readwrite");
	DMA1CNT_H_DMA_Enable = GBReg(0xC4, 31, 31, 1, 0, "readwrite");
	DMA2SAD = GBReg(0xC8, 31, 0, 1, 0, "writeonly");
	DMA2DAD = GBReg(0xCC, 31, 0, 1, 0, "writeonly");
	DMA2CNT_L = GBReg(0xD0, 15, 0, 1, 0, "writeonly");
	DMA2CNT_H = GBReg(0xD0, 31, 16, 1, 0, "writeonly");
	DMA2CNT_H_Dest_Addr_Control = GBReg(0xD0, 22, 21, 1, 0, "readwrite");
	DMA2CNT_H_Source_Adr_Control = GBReg(0xD0, 24, 23, 1, 0, "readwrite");
	DMA2CNT_H_DMA_Repeat = GBReg(0xD0, 25, 25, 1, 0, "readwrite");
	DMA2CNT_H_DMA_Transfer_Type = GBReg(0xD0, 26, 26, 1, 0, "readwrite");
	DMA2CNT_H_DMA_Start_Timing = GBReg(0xD0, 29, 28, 1, 0, "readwrite");
	DMA2CNT_H_IRQ_on = GBReg(0xD0, 30, 30, 1, 0, "readwrite");
	DMA2CNT_H_DMA_Enable = GBReg(0xD0, 31, 31, 1, 0, "readwrite");
	DMA3SAD = GBReg(0xD4, 31, 0, 1, 0, "writeonly");
	DMA3DAD = GBReg(0xD8, 31, 0, 1, 0, "writeonly");
	DMA3CNT_L = GBReg(0xDC, 15, 0, 1, 0, "writeonly");
	DMA3CNT_H = GBReg(0xDC, 31, 16, 1, 0, "writeonly");
	DMA3CNT_H_Dest_Addr_Control = GBReg(0xDC, 22, 21, 1, 0, "readwrite");
	DMA3CNT_H_Source_Adr_Control = GBReg(0xDC, 24, 23, 1, 0, "readwrite");
	DMA3CNT_H_DMA_Repeat = GBReg(0xDC, 25, 25, 1, 0, "readwrite");
	DMA3CNT_H_DMA_Transfer_Type = GBReg(0xDC, 26, 26, 1, 0, "readwrite");
	DMA3CNT_H_Game_Pak_DRQ = GBReg(0xDC, 27, 27, 1, 0, "readwrite");
	DMA3CNT_H_DMA_Start_Timing = GBReg(0xDC, 29, 28, 1, 0, "readwrite");
	DMA3CNT_H_IRQ_on = GBReg(0xDC, 30, 30, 1, 0, "readwrite");
	DMA3CNT_H_DMA_Enable = GBReg(0xDC, 31, 31, 1, 0, "readwrite");
}

RegSect_timer::RegSect_timer()
{
	TM0CNT_L = GBReg(0x100, 15, 0, 1, 0, "readwrite");
	TM0CNT_H = GBReg(0x100, 31, 16, 1, 0, "readwrite");
	TM0CNT_H_Prescaler = GBReg(0x100, 17, 16, 1, 0, "readwrite");
	TM0CNT_H_Count_up = GBReg(0x100, 18, 18, 1, 0, "readwrite");
	TM0CNT_H_Timer_IRQ_Enable = GBReg(0x100, 22, 22, 1, 0, "readwrite");
	TM0CNT_H_Timer_Start_Stop = GBReg(0x100, 23, 23, 1, 0, "readwrite");
	TM1CNT_L = GBReg(0x104, 15, 0, 1, 0, "readwrite");
	TM1CNT_H = GBReg(0x104, 31, 16, 1, 0, "readwrite");
	TM1CNT_H_Prescaler = GBReg(0x104, 17, 16, 1, 0, "readwrite");
	TM1CNT_H_Count_up = GBReg(0x104, 18, 18, 1, 0, "readwrite");
	TM1CNT_H_Timer_IRQ_Enable = GBReg(0x104, 22, 22, 1, 0, "readwrite");
	TM1CNT_H_Timer_Start_Stop = GBReg(0x104, 23, 23, 1, 0, "readwrite");
	TM2CNT_L = GBReg(0x108, 15, 0, 1, 0, "readwrite");
	TM2CNT_H = GBReg(0x108, 31, 16, 1, 0, "readwrite");
	TM2CNT_H_Prescaler = GBReg(0x108, 17, 16, 1, 0, "readwrite");
	TM2CNT_H_Count_up = GBReg(0x108, 18, 18, 1, 0, "readwrite");
	TM2CNT_H_Timer_IRQ_Enable = GBReg(0x108, 22, 22, 1, 0, "readwrite");
	TM2CNT_H_Timer_Start_Stop = GBReg(0x108, 23, 23, 1, 0, "readwrite");
	TM3CNT_L = GBReg(0x10C, 15, 0, 1, 0, "readwrite");
	TM3CNT_H = GBReg(0x10C, 31, 16, 1, 0, "readwrite");
	TM3CNT_H_Prescaler = GBReg(0x10C, 17, 16, 1, 0, "readwrite");
	TM3CNT_H_Count_up = GBReg(0x10C, 18, 18, 1, 0, "readwrite");
	TM3CNT_H_Timer_IRQ_Enable = GBReg(0x10C, 22, 22, 1, 0, "readwrite");
	TM3CNT_H_Timer_Start_Stop = GBReg(0x10C, 23, 23, 1, 0, "readwrite");
}

RegSect_serial::RegSect_serial()
{
	SIODATA32 = GBReg(0x120, 31, 0, 1, 0, "readwrite");
	SIOMULTI0 = GBReg(0x120, 15, 0, 1, 0, "readwrite");
	SIOMULTI1 = GBReg(0x122, 15, 0, 1, 0, "readwrite");
	SIOMULTI2 = GBReg(0x124, 15, 0, 1, 0, "readwrite");
	SIOMULTI3 = GBReg(0x126, 15, 0, 1, 0, "readwrite");
	SIOCNT = GBReg(0x128, 15, 0, 1, 0, "readwrite");
	SIOMLT_SEND = GBReg(0x12A, 15, 0, 1, 0, "readwrite");
	SIODATA8 = GBReg(0x12A, 15, 0, 1, 0, "readwrite");
	RCNT = GBReg(0x134, 15, 0, 1, 0, "readwrite");
	IR = GBReg(0x136, 15, 0, 1, 0, "readwrite");
	JOYCNT = GBReg(0x140, 15, 0, 1, 0, "readwrite");
	JOY_RECV = GBReg(0x150, 31, 0, 1, 0, "readwrite");
	JOY_TRANS = GBReg(0x154, 31, 0, 1, 0, "readwrite");
	JOYSTAT = GBReg(0x158, 15, 0, 1, 0, "readwrite");
}

RegSect_keypad::RegSect_keypad()
{
	KEYINPUT = GBReg(0x130, 15, 0, 1, 0x3FF, "readonly");
	KEYCNT = GBReg(0x132, 15, 0, 1, 0, "readwrite");
}

RegSect_system::RegSect_system()
{
	IE = GBReg(0x200, 15, 0, 1, 0, "readwrite");
	IF = GBReg(0x200, 31, 16, 1, 0, "readwrite");
	WAITCNT = GBReg(0x204, 15, 0, 1, 0, "readwrite");
	IME = GBReg(0x208, 31, 0, 1, 0, "readwrite");
	POSTFLG = GBReg(0x300, 7, 0, 1, 0, "readwrite");
	HALTCNT = GBReg(0x300, 15, 8, 1, 0, "writeonly");
	MemCtrl = GBReg(0x800, 31, 0, 1, 0, "readwrite");
}

void GBREGS::reset()
{
	//Sect_display = new RegSect_display();
	//Sect_sound = new RegSect_sound();
	//Sect_dma = new RegSect_dma();
	//Sect_timer = new RegSect_timer();
	//Sect_serial = new RegSect_serial();
	//Sect_keypad = new RegSect_keypad();
	//Sect_system = new RegSect_system();

	// DISPCNT at 0x000 = 0x0080;
	data[0] = 128 & 0xFF;
	// BG2RotScaleParDX at 0x020 = 256;
	data[32] = 256 & 0xFF;
	data[33] = (256 >> 8) & 0xFF;
	// BG2RotScaleParDMY at 0x024 = 256;
	data[36] = 16777216 & 0xFF;
	data[37] = (16777216 >> 8) & 0xFF;
	data[38] = (16777216 >> 16) & 0xFF;
	data[39] = (16777216 >> 24) & 0xFF;
	// BG3RotScaleParDX at 0x030 = 256;
	data[48] = 256 & 0xFF;
	data[49] = (256 >> 8) & 0xFF;
	// BG3RotScaleParDMY at 0x034 = 256;
	data[52] = 16777216 & 0xFF;
	data[53] = (16777216 >> 8) & 0xFF;
	data[54] = (16777216 >> 16) & 0xFF;
	data[55] = (16777216 >> 24) & 0xFF;
	// SOUNDBIAS at 0x088 = 0x0200;
	data[136] = 512 & 0xFF;
	data[137] = (512 >> 8) & 0xFF;

	// DISPCNT at 0x000 = 0xFFFFFFFF;
	rwmask[0] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[1] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[2] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[3] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// DISPSTAT at 0x004 = 0xFFFFFFFF;
	rwmask[4] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[5] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[6] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[7] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// BG0CNT at 0x008 = 0xDFFF;
	rwmask[8] = (byte)(0xDFFF & 0xFF);
	rwmask[9] = (byte)((0xDFFF >> 8) & 0xFF);
	rwmask[10] = (byte)((0xDFFF >> 16) & 0xFF);
	rwmask[11] = (byte)((0xDFFF >> 24) & 0xFF);
	// BG1CNT at 0x00A = 0xDFFF;
	rwmask[10] = (byte)(0xDFFF & 0xFF);
	rwmask[11] = (byte)((0xDFFF >> 8) & 0xFF);
	rwmask[12] = (byte)((0xDFFF >> 16) & 0xFF);
	rwmask[13] = (byte)((0xDFFF >> 24) & 0xFF);
	// BG2CNT at 0x00C = 0xFFFF;
	rwmask[12] = (byte)(0xFFFF & 0xFF);
	rwmask[13] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[14] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[15] = (byte)((0xFFFF >> 24) & 0xFF);
	// BG3CNT at 0x00E = 0xFFFF;
	rwmask[14] = (byte)(0xFFFF & 0xFF);
	rwmask[15] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[16] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[17] = (byte)((0xFFFF >> 24) & 0xFF);
	// BG0HOFS at 0x010 = 0x0;
	rwmask[16] = (byte)(0x0 & 0xFF);
	rwmask[17] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[18] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[19] = (byte)((0x0 >> 24) & 0xFF);
	// BG0VOFS at 0x012 = 0x0;
	rwmask[18] = (byte)(0x0 & 0xFF);
	rwmask[19] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[20] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[21] = (byte)((0x0 >> 24) & 0xFF);
	// BG1HOFS at 0x014 = 0x0;
	rwmask[20] = (byte)(0x0 & 0xFF);
	rwmask[21] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[22] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[23] = (byte)((0x0 >> 24) & 0xFF);
	// BG1VOFS at 0x016 = 0x0;
	rwmask[22] = (byte)(0x0 & 0xFF);
	rwmask[23] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[24] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[25] = (byte)((0x0 >> 24) & 0xFF);
	// BG2HOFS at 0x018 = 0x0;
	rwmask[24] = (byte)(0x0 & 0xFF);
	rwmask[25] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[26] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[27] = (byte)((0x0 >> 24) & 0xFF);
	// BG2VOFS at 0x01A = 0x0;
	rwmask[26] = (byte)(0x0 & 0xFF);
	rwmask[27] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[28] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[29] = (byte)((0x0 >> 24) & 0xFF);
	// BG3HOFS at 0x01C = 0x0;
	rwmask[28] = (byte)(0x0 & 0xFF);
	rwmask[29] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[30] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[31] = (byte)((0x0 >> 24) & 0xFF);
	// BG3VOFS at 0x01E = 0x0;
	rwmask[30] = (byte)(0x0 & 0xFF);
	rwmask[31] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[32] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[33] = (byte)((0x0 >> 24) & 0xFF);
	// BG2RotScaleParDX at 0x020 = 0x0;
	rwmask[32] = (byte)(0x0 & 0xFF);
	rwmask[33] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[34] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[35] = (byte)((0x0 >> 24) & 0xFF);
	// BG2RotScaleParDY at 0x024 = 0x0;
	rwmask[36] = (byte)(0x0 & 0xFF);
	rwmask[37] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[38] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[39] = (byte)((0x0 >> 24) & 0xFF);
	// BG2RefX at 0x028 = 0x0;
	rwmask[40] = (byte)(0x0 & 0xFF);
	rwmask[41] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[42] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[43] = (byte)((0x0 >> 24) & 0xFF);
	// BG2RefY at 0x02C = 0x0;
	rwmask[44] = (byte)(0x0 & 0xFF);
	rwmask[45] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[46] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[47] = (byte)((0x0 >> 24) & 0xFF);
	// BG3RotScaleParDX at 0x030 = 0x0;
	rwmask[48] = (byte)(0x0 & 0xFF);
	rwmask[49] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[50] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[51] = (byte)((0x0 >> 24) & 0xFF);
	// BG3RotScaleParDY at 0x034 = 0x0;
	rwmask[52] = (byte)(0x0 & 0xFF);
	rwmask[53] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[54] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[55] = (byte)((0x0 >> 24) & 0xFF);
	// BG3RefX at 0x038 = 0x0;
	rwmask[56] = (byte)(0x0 & 0xFF);
	rwmask[57] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[58] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[59] = (byte)((0x0 >> 24) & 0xFF);
	// BG3RefY at 0x03C = 0x0;
	rwmask[60] = (byte)(0x0 & 0xFF);
	rwmask[61] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[62] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[63] = (byte)((0x0 >> 24) & 0xFF);
	// WIN0H at 0x040 = 0x0;
	rwmask[64] = (byte)(0x0 & 0xFF);
	rwmask[65] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[66] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[67] = (byte)((0x0 >> 24) & 0xFF);
	// WIN0V at 0x044 = 0x0;
	rwmask[68] = (byte)(0x0 & 0xFF);
	rwmask[69] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[70] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[71] = (byte)((0x0 >> 24) & 0xFF);
	// WININ at 0x048 = 0x3F3F3F3F;
	rwmask[72] = (byte)(0x3F3F3F3F & 0xFF);
	rwmask[73] = (byte)((0x3F3F3F3F >> 8) & 0xFF);
	rwmask[74] = (byte)((0x3F3F3F3F >> 16) & 0xFF);
	rwmask[75] = (byte)((0x3F3F3F3F >> 24) & 0xFF);
	// MOSAIC at 0x04C = 0x0;
	rwmask[76] = (byte)(0x0 & 0xFF);
	rwmask[77] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[78] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[79] = (byte)((0x0 >> 24) & 0xFF);
	// BLDCNT at 0x050 = 0x1F1F3FFF;
	rwmask[80] = (byte)(0x1F1F3FFF & 0xFF);
	rwmask[81] = (byte)((0x1F1F3FFF >> 8) & 0xFF);
	rwmask[82] = (byte)((0x1F1F3FFF >> 16) & 0xFF);
	rwmask[83] = (byte)((0x1F1F3FFF >> 24) & 0xFF);
	// BLDY at 0x054 = 0x0;
	rwmask[84] = (byte)(0x0 & 0xFF);
	rwmask[85] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[86] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[87] = (byte)((0x0 >> 24) & 0xFF);
	// SOUND1CNT_L at 0x060 = 0xFFC0007F;
	rwmask[96] = (byte)(0xFFC0007F & 0xFF);
	rwmask[97] = (byte)((0xFFC0007F >> 8) & 0xFF);
	rwmask[98] = (byte)((0xFFC0007F >> 16) & 0xFF);
	rwmask[99] = (byte)((0xFFC0007F >> 24) & 0xFF);
	// SOUND1CNT_X at 0x064 = 0xFFFF4000;
	rwmask[100] = (byte)(0xFFFF4000 & 0xFF);
	rwmask[101] = (byte)((0xFFFF4000 >> 8) & 0xFF);
	rwmask[102] = (byte)((0xFFFF4000 >> 16) & 0xFF);
	rwmask[103] = (byte)((0xFFFF4000 >> 24) & 0xFF);
	// SOUND2CNT_L at 0x068 = 0xFFC0;
	rwmask[104] = (byte)(0xFFC0 & 0xFF);
	rwmask[105] = (byte)((0xFFC0 >> 8) & 0xFF);
	rwmask[106] = (byte)((0xFFC0 >> 16) & 0xFF);
	rwmask[107] = (byte)((0xFFC0 >> 24) & 0xFF);
	// SOUND2CNT_H at 0x06C = 0xFFFF4000;
	rwmask[108] = (byte)(0xFFFF4000 & 0xFF);
	rwmask[109] = (byte)((0xFFFF4000 >> 8) & 0xFF);
	rwmask[110] = (byte)((0xFFFF4000 >> 16) & 0xFF);
	rwmask[111] = (byte)((0xFFFF4000 >> 24) & 0xFF);
	// SOUND3CNT_L at 0x070 = 0xE00000E0;
	rwmask[112] = (byte)(0xE00000E0 & 0xFF);
	rwmask[113] = (byte)((0xE00000E0 >> 8) & 0xFF);
	rwmask[114] = (byte)((0xE00000E0 >> 16) & 0xFF);
	rwmask[115] = (byte)((0xE00000E0 >> 24) & 0xFF);
	// SOUND3CNT_X at 0x074 = 0xFFFF4000;
	rwmask[116] = (byte)(0xFFFF4000 & 0xFF);
	rwmask[117] = (byte)((0xFFFF4000 >> 8) & 0xFF);
	rwmask[118] = (byte)((0xFFFF4000 >> 16) & 0xFF);
	rwmask[119] = (byte)((0xFFFF4000 >> 24) & 0xFF);
	// SOUND4CNT_L at 0x078 = 0xFFFFFF00;
	rwmask[120] = (byte)(0xFFFFFF00 & 0xFF);
	rwmask[121] = (byte)((0xFFFFFF00 >> 8) & 0xFF);
	rwmask[122] = (byte)((0xFFFFFF00 >> 16) & 0xFF);
	rwmask[123] = (byte)((0xFFFFFF00 >> 24) & 0xFF);
	// SOUND4CNT_H at 0x07C = 0xFFFF40FF;
	rwmask[124] = (byte)(0xFFFF40FF & 0xFF);
	rwmask[125] = (byte)((0xFFFF40FF >> 8) & 0xFF);
	rwmask[126] = (byte)((0xFFFF40FF >> 16) & 0xFF);
	rwmask[127] = (byte)((0xFFFF40FF >> 24) & 0xFF);
	// SOUNDCNT_L at 0x080 = 0xFFFFFF77;
	rwmask[128] = (byte)(0xFFFFFF77 & 0xFF);
	rwmask[129] = (byte)((0xFFFFFF77 >> 8) & 0xFF);
	rwmask[130] = (byte)((0xFFFFFF77 >> 16) & 0xFF);
	rwmask[131] = (byte)((0xFFFFFF77 >> 24) & 0xFF);
	// SOUNDCNT_X at 0x084 = 0xFFFF00FF;
	rwmask[132] = (byte)(0xFFFF00FF & 0xFF);
	rwmask[133] = (byte)((0xFFFF00FF >> 8) & 0xFF);
	rwmask[134] = (byte)((0xFFFF00FF >> 16) & 0xFF);
	rwmask[135] = (byte)((0xFFFF00FF >> 24) & 0xFF);
	// SOUNDBIAS at 0x088 = 0xFFFFFFFF;
	rwmask[136] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[137] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[138] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[139] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// WAVE_RAM at 0x090 = 0xFFFFFFFF;
	rwmask[144] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[145] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[146] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[147] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// WAVE_RAM2 at 0x094 = 0xFFFFFFFF;
	rwmask[148] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[149] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[150] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[151] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// WAVE_RAM3 at 0x098 = 0xFFFFFFFF;
	rwmask[152] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[153] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[154] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[155] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// WAVE_RAM4 at 0x09C = 0xFFFFFFFF;
	rwmask[156] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[157] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[158] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[159] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// FIFO_A at 0x0A0 = 0x0;
	rwmask[160] = (byte)(0x0 & 0xFF);
	rwmask[161] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[162] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[163] = (byte)((0x0 >> 24) & 0xFF);
	// FIFO_B at 0x0A4 = 0x0;
	rwmask[164] = (byte)(0x0 & 0xFF);
	rwmask[165] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[166] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[167] = (byte)((0x0 >> 24) & 0xFF);
	// DMA0SAD at 0xB0 = 0x0;
	rwmask[176] = (byte)(0x0 & 0xFF);
	rwmask[177] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[178] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[179] = (byte)((0x0 >> 24) & 0xFF);
	// DMA0DAD at 0xB4 = 0x0;
	rwmask[180] = (byte)(0x0 & 0xFF);
	rwmask[181] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[182] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[183] = (byte)((0x0 >> 24) & 0xFF);
	// DMA0CNT_L at 0xB8 = 0xF7E00000;
	rwmask[184] = (byte)(0xF7E00000 & 0xFF);
	rwmask[185] = (byte)((0xF7E00000 >> 8) & 0xFF);
	rwmask[186] = (byte)((0xF7E00000 >> 16) & 0xFF);
	rwmask[187] = (byte)((0xF7E00000 >> 24) & 0xFF);
	// DMA1SAD at 0xBC = 0x0;
	rwmask[188] = (byte)(0x0 & 0xFF);
	rwmask[189] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[190] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[191] = (byte)((0x0 >> 24) & 0xFF);
	// DMA1DAD at 0xC0 = 0x0;
	rwmask[192] = (byte)(0x0 & 0xFF);
	rwmask[193] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[194] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[195] = (byte)((0x0 >> 24) & 0xFF);
	// DMA1CNT_L at 0xC4 = 0xF7E00000;
	rwmask[196] = (byte)(0xF7E00000 & 0xFF);
	rwmask[197] = (byte)((0xF7E00000 >> 8) & 0xFF);
	rwmask[198] = (byte)((0xF7E00000 >> 16) & 0xFF);
	rwmask[199] = (byte)((0xF7E00000 >> 24) & 0xFF);
	// DMA2SAD at 0xC8 = 0x0;
	rwmask[200] = (byte)(0x0 & 0xFF);
	rwmask[201] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[202] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[203] = (byte)((0x0 >> 24) & 0xFF);
	// DMA2DAD at 0xCC = 0x0;
	rwmask[204] = (byte)(0x0 & 0xFF);
	rwmask[205] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[206] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[207] = (byte)((0x0 >> 24) & 0xFF);
	// DMA2CNT_L at 0xD0 = 0xF7E00000;
	rwmask[208] = (byte)(0xF7E00000 & 0xFF);
	rwmask[209] = (byte)((0xF7E00000 >> 8) & 0xFF);
	rwmask[210] = (byte)((0xF7E00000 >> 16) & 0xFF);
	rwmask[211] = (byte)((0xF7E00000 >> 24) & 0xFF);
	// DMA3SAD at 0xD4 = 0x0;
	rwmask[212] = (byte)(0x0 & 0xFF);
	rwmask[213] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[214] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[215] = (byte)((0x0 >> 24) & 0xFF);
	// DMA3DAD at 0xD8 = 0x0;
	rwmask[216] = (byte)(0x0 & 0xFF);
	rwmask[217] = (byte)((0x0 >> 8) & 0xFF);
	rwmask[218] = (byte)((0x0 >> 16) & 0xFF);
	rwmask[219] = (byte)((0x0 >> 24) & 0xFF);
	// DMA3CNT_L at 0xDC = 0xFFE00000;
	rwmask[220] = (byte)(0xFFE00000 & 0xFF);
	rwmask[221] = (byte)((0xFFE00000 >> 8) & 0xFF);
	rwmask[222] = (byte)((0xFFE00000 >> 16) & 0xFF);
	rwmask[223] = (byte)((0xFFE00000 >> 24) & 0xFF);
	// TM0CNT_L at 0x100 = 0xFFFFFFFF;
	rwmask[256] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[257] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[258] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[259] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// TM1CNT_L at 0x104 = 0xFFFFFFFF;
	rwmask[260] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[261] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[262] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[263] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// TM2CNT_L at 0x108 = 0xFFFFFFFF;
	rwmask[264] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[265] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[266] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[267] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// TM3CNT_L at 0x10C = 0xFFFFFFFF;
	rwmask[268] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[269] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[270] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[271] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// SIODATA32 at 0x120 = 0xFFFFFFFF;
	rwmask[288] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[289] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[290] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[291] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// SIOMULTI1 at 0x122 = 0xFFFF;
	rwmask[290] = (byte)(0xFFFF & 0xFF);
	rwmask[291] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[292] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[293] = (byte)((0xFFFF >> 24) & 0xFF);
	// SIOMULTI2 at 0x124 = 0xFFFF;
	rwmask[292] = (byte)(0xFFFF & 0xFF);
	rwmask[293] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[294] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[295] = (byte)((0xFFFF >> 24) & 0xFF);
	// SIOMULTI3 at 0x126 = 0xFFFF;
	rwmask[294] = (byte)(0xFFFF & 0xFF);
	rwmask[295] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[296] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[297] = (byte)((0xFFFF >> 24) & 0xFF);
	// SIOCNT at 0x128 = 0xFFFF;
	rwmask[296] = (byte)(0xFFFF & 0xFF);
	rwmask[297] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[298] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[299] = (byte)((0xFFFF >> 24) & 0xFF);
	// SIOMLT_SEND at 0x12A = 0xFFFF;
	rwmask[298] = (byte)(0xFFFF & 0xFF);
	rwmask[299] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[300] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[301] = (byte)((0xFFFF >> 24) & 0xFF);
	// KEYINPUT at 0x130 = 0xFFFF;
	rwmask[304] = (byte)(0xFFFF & 0xFF);
	rwmask[305] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[306] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[307] = (byte)((0xFFFF >> 24) & 0xFF);
	// KEYCNT at 0x132 = 0xFFFF;
	rwmask[306] = (byte)(0xFFFF & 0xFF);
	rwmask[307] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[308] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[309] = (byte)((0xFFFF >> 24) & 0xFF);
	// RCNT at 0x134 = 0xFFFF;
	rwmask[308] = (byte)(0xFFFF & 0xFF);
	rwmask[309] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[310] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[311] = (byte)((0xFFFF >> 24) & 0xFF);
	// IR at 0x136 = 0xFFFF;
	rwmask[310] = (byte)(0xFFFF & 0xFF);
	rwmask[311] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[312] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[313] = (byte)((0xFFFF >> 24) & 0xFF);
	// JOYCNT at 0x140 = 0xFFFF;
	rwmask[320] = (byte)(0xFFFF & 0xFF);
	rwmask[321] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[322] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[323] = (byte)((0xFFFF >> 24) & 0xFF);
	// JOY_RECV at 0x150 = 0xFFFFFFFF;
	rwmask[336] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[337] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[338] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[339] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// JOY_TRANS at 0x154 = 0xFFFFFFFF;
	rwmask[340] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[341] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[342] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[343] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// JOYSTAT at 0x158 = 0xFFFF;
	rwmask[344] = (byte)(0xFFFF & 0xFF);
	rwmask[345] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[346] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[347] = (byte)((0xFFFF >> 24) & 0xFF);
	// IE at 0x200 = 0xFFFFFFFF;
	rwmask[512] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[513] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[514] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[515] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// WAITCNT at 0x204 = 0xFFFF;
	rwmask[516] = (byte)(0xFFFF & 0xFF);
	rwmask[517] = (byte)((0xFFFF >> 8) & 0xFF);
	rwmask[518] = (byte)((0xFFFF >> 16) & 0xFF);
	rwmask[519] = (byte)((0xFFFF >> 24) & 0xFF);
	// IME at 0x208 = 0xFFFFFFFF;
	rwmask[520] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[521] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[522] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[523] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
	// POSTFLG at 0x300 = 0xFF;
	rwmask[768] = (byte)(0xFF & 0xFF);
	rwmask[769] = (byte)((0xFF >> 8) & 0xFF);
	rwmask[770] = (byte)((0xFF >> 16) & 0xFF);
	rwmask[771] = (byte)((0xFF >> 24) & 0xFF);
	// MemCtrl at 0x800 = 0xFFFFFFFF;
	rwmask[2048] = (byte)(0xFFFFFFFF & 0xFF);
	rwmask[2049] = (byte)((0xFFFFFFFF >> 8) & 0xFF);
	rwmask[2050] = (byte)((0xFFFFFFFF >> 16) & 0xFF);
	rwmask[2051] = (byte)((0xFFFFFFFF >> 24) & 0xFF);
}