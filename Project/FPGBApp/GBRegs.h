#pragma once
#include <string>
using namespace std;

#include "types.h"

class GBReg
{
public:
	UInt32 address;
	byte msb;
	byte lsb;
	UInt16 count;
	UInt32 defaultvalue;

	GBReg();
	GBReg(UInt32 address, byte msb, byte lsb, UInt16 count, UInt32 defaultvalue, string accesstype);
	void write(UInt32 value);
	UInt32 read();
	bool on();
	void mask(UInt32 mask);
	void write_reg(UInt32 value);
};

class RegSect_display
{
public:
	/// <summary>
	/// LCD Control 2 R/W
	/// </summary>
	GBReg DISPCNT;
	/// <summary>
	/// BG Mode (0-5=Video Mode 0-5, 6-7=Prohibited)
	/// </summary>
	GBReg DISPCNT_BG_Mode;
	/// <summary>
	/// Reserved / CGB Mode (0=GBA, 1=CGB; can be set only by BIOS opcodes)
	/// </summary>
	GBReg DISPCNT_Reserved_CGB_Mode;
	/// <summary>
	/// Display Frame Select (0-1=Frame 0-1) (for BG Modes 4,5 only)
	/// </summary>
	GBReg DISPCNT_Display_Frame_Select;
	/// <summary>
	/// H-Blank Interval Free (1=Allow access to OAM during H-Blank)
	/// </summary>
	GBReg DISPCNT_H_Blank_IntervalFree;
	/// <summary>
	/// OBJ Character VRAM Mapping (0=Two dimensional, 1=One dimensional)
	/// </summary>
	GBReg DISPCNT_OBJ_Char_VRAM_Map;
	/// <summary>
	/// Forced Blank (1=Allow FAST access to VRAM,Palette,OAM)
	/// </summary>
	GBReg DISPCNT_Forced_Blank;
	/// <summary>
	/// Screen Display BG0 (0=Off, 1=On)
	/// </summary>
	GBReg DISPCNT_Screen_Display_BG0;
	/// <summary>
	/// Screen Display BG1 (0=Off, 1=On)
	/// </summary>
	GBReg DISPCNT_Screen_Display_BG1;
	/// <summary>
	/// Screen Display BG2 (0=Off, 1=On)
	/// </summary>
	GBReg DISPCNT_Screen_Display_BG2;
	/// <summary>
	/// Screen Display BG3 (0=Off, 1=On)
	/// </summary>
	GBReg DISPCNT_Screen_Display_BG3;
	/// <summary>
	/// Screen Display OBJ (0=Off, 1=On)
	/// </summary>
	GBReg DISPCNT_Screen_Display_OBJ;
	/// <summary>
	/// Window 0 Display Flag (0=Off, 1=On)
	/// </summary>
	GBReg DISPCNT_Window_0_Display_Flag;
	/// <summary>
	/// Window 1 Display Flag (0=Off, 1=On)
	/// </summary>
	GBReg DISPCNT_Window_1_Display_Flag;
	/// <summary>
	/// OBJ Window Display Flag (0=Off, 1=On)
	/// </summary>
	GBReg DISPCNT_OBJ_Wnd_Display_Flag;
	/// <summary>
	/// Undocumented - Green Swap 2 R/W
	/// </summary>
	GBReg GREENSWAP;
	/// <summary>
	/// General LCD Status (STAT,LYC) 2 R/W
	/// </summary>
	GBReg DISPSTAT;
	/// <summary>
	/// V-Blank flag (Read only) (1=VBlank) (set in line 160..226; not 227)
	/// </summary>
	GBReg DISPSTAT_V_Blank_flag;
	/// <summary>
	/// H-Blank flag (Read only) (1=HBlank) (toggled in all lines, 0..227)
	/// </summary>
	GBReg DISPSTAT_H_Blank_flag;
	/// <summary>
	/// V-Counter flag (Read only) (1=Match) (set in selected line) (R)
	/// </summary>
	GBReg DISPSTAT_V_Counter_flag;
	/// <summary>
	/// V-Blank IRQ Enable (1=Enable) (R/W)
	/// </summary>
	GBReg DISPSTAT_V_Blank_IRQ_Enable;
	/// <summary>
	/// H-Blank IRQ Enable (1=Enable) (R/W)
	/// </summary>
	GBReg DISPSTAT_H_Blank_IRQ_Enable;
	/// <summary>
	/// V-Counter IRQ Enable (1=Enable) (R/W)
	/// </summary>
	GBReg DISPSTAT_V_Counter_IRQ_Enable;
	/// <summary>
	/// V-Count Setting (LYC) (0..227) (R/W)
	/// </summary>
	GBReg DISPSTAT_V_Count_Setting;
	/// <summary>
	/// Vertical Counter (LY) 2 R
	/// </summary>
	GBReg VCOUNT;
	/// <summary>
	/// BG0 Control 2 R/W
	/// </summary>
	GBReg BG0CNT;
	/// <summary>
	/// BG Priority (0-3, 0=Highest)
	/// </summary>
	GBReg BG0CNT_BG_Priority;
	/// <summary>
	/// Character Base Block (0-3, in units of 16 KBytes) (=BG Tile Data)
	/// </summary>
	GBReg BG0CNT_Character_Base_Block;
	/// <summary>
	/// 4-5 Not used (must be zero)
	/// </summary>
	GBReg BG0CNT_UNUSED_4_5;
	/// <summary>
	/// Mosaic (0=Disable, 1=Enable)
	/// </summary>
	GBReg BG0CNT_Mosaic;
	/// <summary>
	/// Colors/Palettes (0=16/16, 1=256/1)
	/// </summary>
	GBReg BG0CNT_Colors_Palettes;
	/// <summary>
	/// Screen Base Block (0-31, in units of 2 KBytes) (=BG Map Data)
	/// </summary>
	GBReg BG0CNT_Screen_Base_Block;
	/// <summary>
	/// Screen Size (0-3)
	/// </summary>
	GBReg BG0CNT_Screen_Size;
	/// <summary>
	/// BG1 Control 2 R/W
	/// </summary>
	GBReg BG1CNT;
	/// <summary>
	/// BG Priority (0-3, 0=Highest)
	/// </summary>
	GBReg BG1CNT_BG_Priority;
	/// <summary>
	/// Character Base Block (0-3, in units of 16 KBytes) (=BG Tile Data)
	/// </summary>
	GBReg BG1CNT_Character_Base_Block;
	/// <summary>
	/// 4-5 Not used (must be zero)
	/// </summary>
	GBReg BG1CNT_UNUSED_4_5;
	/// <summary>
	/// Mosaic (0=Disable, 1=Enable)
	/// </summary>
	GBReg BG1CNT_Mosaic;
	/// <summary>
	/// Colors/Palettes (0=16/16, 1=256/1)
	/// </summary>
	GBReg BG1CNT_Colors_Palettes;
	/// <summary>
	/// Screen Base Block (0-31, in units of 2 KBytes) (=BG Map Data)
	/// </summary>
	GBReg BG1CNT_Screen_Base_Block;
	/// <summary>
	/// Screen Size (0-3)
	/// </summary>
	GBReg BG1CNT_Screen_Size;
	/// <summary>
	/// BG2 Control 2 R/W
	/// </summary>
	GBReg BG2CNT;
	/// <summary>
	/// BG Priority (0-3, 0=Highest)
	/// </summary>
	GBReg BG2CNT_BG_Priority;
	/// <summary>
	/// Character Base Block (0-3, in units of 16 KBytes) (=BG Tile Data)
	/// </summary>
	GBReg BG2CNT_Character_Base_Block;
	/// <summary>
	/// Mosaic (0=Disable, 1=Enable)
	/// </summary>
	GBReg BG2CNT_Mosaic;
	/// <summary>
	/// Colors/Palettes (0=16/16, 1=256/1)
	/// </summary>
	GBReg BG2CNT_Colors_Palettes;
	/// <summary>
	/// Screen Base Block (0-31, in units of 2 KBytes) (=BG Map Data)
	/// </summary>
	GBReg BG2CNT_Screen_Base_Block;
	/// <summary>
	/// Display Area Overflow (0=Transparent, 1=Wraparound; BG2CNT/BG3CNT only)
	/// </summary>
	GBReg BG2CNT_Display_Area_Overflow;
	/// <summary>
	/// Screen Size (0-3)
	/// </summary>
	GBReg BG2CNT_Screen_Size;
	/// <summary>
	/// BG3 Control 2 R/W
	/// </summary>
	GBReg BG3CNT;
	/// <summary>
	/// BG Priority (0-3, 0=Highest)
	/// </summary>
	GBReg BG3CNT_BG_Priority;
	/// <summary>
	/// Character Base Block (0-3, in units of 16 KBytes) (=BG Tile Data)
	/// </summary>
	GBReg BG3CNT_Character_Base_Block;
	/// <summary>
	/// Mosaic (0=Disable, 1=Enable)
	/// </summary>
	GBReg BG3CNT_Mosaic;
	/// <summary>
	/// Colors/Palettes (0=16/16, 1=256/1)
	/// </summary>
	GBReg BG3CNT_Colors_Palettes;
	/// <summary>
	/// Screen Base Block (0-31, in units of 2 KBytes) (=BG Map Data)
	/// </summary>
	GBReg BG3CNT_Screen_Base_Block;
	/// <summary>
	/// Display Area Overflow (0=Transparent, 1=Wraparound; BG2CNT/BG3CNT only)
	/// </summary>
	GBReg BG3CNT_Display_Area_Overflow;
	/// <summary>
	/// Screen Size (0-3)
	/// </summary>
	GBReg BG3CNT_Screen_Size;
	/// <summary>
	/// BG0 X-Offset 2 W
	/// </summary>
	GBReg BG0HOFS;
	/// <summary>
	/// BG0 Y-Offset 2 W
	/// </summary>
	GBReg BG0VOFS;
	/// <summary>
	/// BG1 X-Offset 2 W
	/// </summary>
	GBReg BG1HOFS;
	/// <summary>
	/// BG1 Y-Offset 2 W
	/// </summary>
	GBReg BG1VOFS;
	/// <summary>
	/// BG2 X-Offset 2 W
	/// </summary>
	GBReg BG2HOFS;
	/// <summary>
	/// BG2 Y-Offset 2 W
	/// </summary>
	GBReg BG2VOFS;
	/// <summary>
	/// BG3 X-Offset 2 W
	/// </summary>
	GBReg BG3HOFS;
	/// <summary>
	/// BG3 Y-Offset 2 W
	/// </summary>
	GBReg BG3VOFS;
	/// <summary>
	/// BG2 Rotation/Scaling Parameter A (dx) 2 W
	/// </summary>
	GBReg BG2RotScaleParDX;
	/// <summary>
	/// BG2 Rotation/Scaling Parameter B (dmx) 2 W
	/// </summary>
	GBReg BG2RotScaleParDMX;
	/// <summary>
	/// BG2 Rotation/Scaling Parameter C (dy) 2 W
	/// </summary>
	GBReg BG2RotScaleParDY;
	/// <summary>
	/// BG2 Rotation/Scaling Parameter D (dmy) 2 W
	/// </summary>
	GBReg BG2RotScaleParDMY;
	/// <summary>
	/// BG2 Reference Point X-Coordinate 4 W
	/// </summary>
	GBReg BG2RefX;
	/// <summary>
	/// BG2 Reference Point Y-Coordinate 4 W
	/// </summary>
	GBReg BG2RefY;
	/// <summary>
	/// BG3 Rotation/Scaling Parameter A (dx) 2 W
	/// </summary>
	GBReg BG3RotScaleParDX;
	/// <summary>
	/// BG3 Rotation/Scaling Parameter B (dmx) 2 W
	/// </summary>
	GBReg BG3RotScaleParDMX;
	/// <summary>
	/// BG3 Rotation/Scaling Parameter C (dy) 2 W
	/// </summary>
	GBReg BG3RotScaleParDY;
	/// <summary>
	/// BG3 Rotation/Scaling Parameter D (dmy) 2 W
	/// </summary>
	GBReg BG3RotScaleParDMY;
	/// <summary>
	/// BG3 Reference Point X-Coordinate 4 W
	/// </summary>
	GBReg BG3RefX;
	/// <summary>
	/// BG3 Reference Point Y-Coordinate 4 W
	/// </summary>
	GBReg BG3RefY;
	/// <summary>
	/// Window 0 Horizontal Dimensions 2 W
	/// </summary>
	GBReg WIN0H;
	/// <summary>
	/// Window 0 Horizontal Dimensions 2 W
	/// </summary>
	GBReg WIN0H_X2;
	/// <summary>
	/// Window 0 Horizontal Dimensions 2 W
	/// </summary>
	GBReg WIN0H_X1;
	/// <summary>
	/// Window 1 Horizontal Dimensions 2 W
	/// </summary>
	GBReg WIN1H;
	/// <summary>
	/// Window 1 Horizontal Dimensions 2 W
	/// </summary>
	GBReg WIN1H_X2;
	/// <summary>
	/// Window 1 Horizontal Dimensions 2 W
	/// </summary>
	GBReg WIN1H_X1;
	/// <summary>
	/// Window 0 Vertical Dimensions 2 W
	/// </summary>
	GBReg WIN0V;
	/// <summary>
	/// Window 0 Vertical Dimensions 2 W
	/// </summary>
	GBReg WIN0V_Y2;
	/// <summary>
	/// Window 0 Vertical Dimensions 2 W
	/// </summary>
	GBReg WIN0V_Y1;
	/// <summary>
	/// Window 1 Vertical Dimensions 2 W
	/// </summary>
	GBReg WIN1V;
	/// <summary>
	/// Window 1 Vertical Dimensions 2 W
	/// </summary>
	GBReg WIN1V_Y2;
	/// <summary>
	/// Window 1 Vertical Dimensions 2 W
	/// </summary>
	GBReg WIN1V_Y1;
	/// <summary>
	/// Inside of Window 0 and 1 2 R/W
	/// </summary>
	GBReg WININ;
	/// <summary>
	/// 0-3 Window_0_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_0_BG0_Enable;
	/// <summary>
	/// 0-3 Window_0_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_0_BG1_Enable;
	/// <summary>
	/// 0-3 Window_0_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_0_BG2_Enable;
	/// <summary>
	/// 0-3 Window_0_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_0_BG3_Enable;
	/// <summary>
	/// 4 Window_0_OBJ_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_0_OBJ_Enable;
	/// <summary>
	/// 5 Window_0_Special_Effect (0=Disable, 1=Enable)
	/// </summary>
	GBReg WININ_Window_0_Special_Effect;
	/// <summary>
	/// 8-11 Window_1_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_1_BG0_Enable;
	/// <summary>
	/// 8-11 Window_1_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_1_BG1_Enable;
	/// <summary>
	/// 8-11 Window_1_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_1_BG2_Enable;
	/// <summary>
	/// 8-11 Window_1_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_1_BG3_Enable;
	/// <summary>
	/// 12 Window_1_OBJ_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WININ_Window_1_OBJ_Enable;
	/// <summary>
	/// 13 Window_1_Special_Effect (0=Disable, 1=Enable)
	/// </summary>
	GBReg WININ_Window_1_Special_Effect;
	/// <summary>
	/// Inside of OBJ Window & Outside of Windows 2 R/W
	/// </summary>
	GBReg WINOUT;
	/// <summary>
	/// 0-3 Outside_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Outside_BG0_Enable;
	/// <summary>
	/// 0-3 Outside_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Outside_BG1_Enable;
	/// <summary>
	/// 0-3 Outside_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Outside_BG2_Enable;
	/// <summary>
	/// 0-3 Outside_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Outside_BG3_Enable;
	/// <summary>
	/// 4 Outside_OBJ_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Outside_OBJ_Enable;
	/// <summary>
	/// 5 Outside_Special_Effect (0=Disable, 1=Enable)
	/// </summary>
	GBReg WINOUT_Outside_Special_Effect;
	/// <summary>
	/// 8-11 object window_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Objwnd_BG0_Enable;
	/// <summary>
	/// 8-11 object window_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Objwnd_BG1_Enable;
	/// <summary>
	/// 8-11 object window_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Objwnd_BG2_Enable;
	/// <summary>
	/// 8-11 object window_BG0_BG3_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Objwnd_BG3_Enable;
	/// <summary>
	/// 12 object window_OBJ_Enable (0=No Display, 1=Display)
	/// </summary>
	GBReg WINOUT_Objwnd_OBJ_Enable;
	/// <summary>
	/// 13 object window_Special_Effect (0=Disable, 1=Enable)
	/// </summary>
	GBReg WINOUT_Objwnd_Special_Effect;
	/// <summary>
	/// Mosaic Size 2 W
	/// </summary>
	GBReg MOSAIC;
	/// <summary>
	///  0-3 BG_Mosaic_H_Size (minus 1)
	/// </summary>
	GBReg MOSAIC_BG_Mosaic_H_Size;
	/// <summary>
	///  4-7 BG_Mosaic_V_Size (minus 1)
	/// </summary>
	GBReg MOSAIC_BG_Mosaic_V_Size;
	/// <summary>
	///  8-11 OBJ_Mosaic_H_Size (minus 1)
	/// </summary>
	GBReg MOSAIC_OBJ_Mosaic_H_Size;
	/// <summary>
	///  12-15 OBJ_Mosaic_V_Size (minus 1)
	/// </summary>
	GBReg MOSAIC_OBJ_Mosaic_V_Size;
	/// <summary>
	/// Color Special Effects Selection 2 R/W
	/// </summary>
	GBReg BLDCNT;
	/// <summary>
	/// 0 (Background 0)
	/// </summary>
	GBReg BLDCNT_BG0_1st_Target_Pixel;
	/// <summary>
	/// 1 (Background 1)
	/// </summary>
	GBReg BLDCNT_BG1_1st_Target_Pixel;
	/// <summary>
	/// 2 (Background 2)
	/// </summary>
	GBReg BLDCNT_BG2_1st_Target_Pixel;
	/// <summary>
	/// 3 (Background 3)
	/// </summary>
	GBReg BLDCNT_BG3_1st_Target_Pixel;
	/// <summary>
	/// 4 (Top-most OBJ pixel)
	/// </summary>
	GBReg BLDCNT_OBJ_1st_Target_Pixel;
	/// <summary>
	/// 5 (Backdrop)
	/// </summary>
	GBReg BLDCNT_BD_1st_Target_Pixel;
	/// <summary>
	/// 6-7 (0-3, see below) 0 = None (Special effects disabled), 1 = Alpha Blending (1st+2nd Target mixed), 2 = Brightness Increase (1st Target becomes whiter), 3 = Brightness Decrease (1st Target becomes blacker)
	/// </summary>
	GBReg BLDCNT_Color_Special_Effect;
	/// <summary>
	/// 8 (Background 0)
	/// </summary>
	GBReg BLDCNT_BG0_2nd_Target_Pixel;
	/// <summary>
	/// 9 (Background 1)
	/// </summary>
	GBReg BLDCNT_BG1_2nd_Target_Pixel;
	/// <summary>
	/// 10 (Background 2)
	/// </summary>
	GBReg BLDCNT_BG2_2nd_Target_Pixel;
	/// <summary>
	/// 11 (Background 3)
	/// </summary>
	GBReg BLDCNT_BG3_2nd_Target_Pixel;
	/// <summary>
	/// 12 (Top-most OBJ pixel)
	/// </summary>
	GBReg BLDCNT_OBJ_2nd_Target_Pixel;
	/// <summary>
	/// 13 (Backdrop)
	/// </summary>
	GBReg BLDCNT_BD_2nd_Target_Pixel;
	/// <summary>
	/// Alpha Blending Coefficients 2 W
	/// </summary>
	GBReg BLDALPHA;
	/// <summary>
	/// 0-4 (1st Target) (0..16 = 0/16..16/16, 17..31=16/16)
	/// </summary>
	GBReg BLDALPHA_EVA_Coefficient;
	/// <summary>
	/// 8-12 (2nd Target) (0..16 = 0/16..16/16, 17..31=16/16)
	/// </summary>
	GBReg BLDALPHA_EVB_Coefficient;
	/// <summary>
	/// Brightness (Fade-In/Out) Coefficient 0-4 EVY Coefficient (Brightness) (0..16 = 0/16..16/16, 17..31=16/16
	/// </summary>
	GBReg BLDY;

	RegSect_display();
};

class RegSect_sound
{
public:
	/// <summary>
	/// Channel 1 Sweep register (NR10)
	/// </summary>
	GBReg SOUND1CNT_L;
	/// <summary>
	/// 0-2 R/W (n=0-7)
	/// </summary>
	GBReg SOUND1CNT_L_Number_of_sweep_shift;
	/// <summary>
	/// 3 R/W (0=Increase, 1=Decrease)
	/// </summary>
	GBReg SOUND1CNT_L_Sweep_Frequency_Direction;
	/// <summary>
	/// 4-6 R/W units of 7.8ms (0-7, min=7.8ms, max=54.7ms)
	/// </summary>
	GBReg SOUND1CNT_L_Sweep_Time;
	/// <summary>
	/// Channel 1 Duty/Length/Envelope (NR11, NR12)
	/// </summary>
	GBReg SOUND1CNT_H;
	/// <summary>
	/// 0-5 W units of (64-n)/256s (0-63)
	/// </summary>
	GBReg SOUND1CNT_H_Sound_length;
	/// <summary>
	/// 6-7 R/W (0-3, see below)
	/// </summary>
	GBReg SOUND1CNT_H_Wave_Pattern_Duty;
	/// <summary>
	/// 8-10 R/W units of n/64s (1-7, 0=No Envelope)
	/// </summary>
	GBReg SOUND1CNT_H_Envelope_Step_Time;
	/// <summary>
	/// 11 R/W (0=Decrease, 1=Increase)
	/// </summary>
	GBReg SOUND1CNT_H_Envelope_Direction;
	/// <summary>
	/// 12-15 R/W (1-15, 0=No Sound)
	/// </summary>
	GBReg SOUND1CNT_H_Initial_Volume_of_envelope;
	/// <summary>
	/// Channel 1 Frequency/Control (NR13, NR14)
	/// </summary>
	GBReg SOUND1CNT_X;
	/// <summary>
	/// 0-10 W 131072/(2048-n)Hz (0-2047)
	/// </summary>
	GBReg SOUND1CNT_X_Frequency;
	/// <summary>
	/// 14 R/W (1=Stop output when length in NR11 expires)
	/// </summary>
	GBReg SOUND1CNT_X_Length_Flag;
	/// <summary>
	/// 15 W (1=Restart Sound)
	/// </summary>
	GBReg SOUND1CNT_X_Initial;
	/// <summary>
	/// must return zero
	/// </summary>
	GBReg SOUND1CNT_XHighZero;
	/// <summary>
	/// Channel 2 Duty/Length/Envelope (NR21, NR22)
	/// </summary>
	GBReg SOUND2CNT_L;
	/// <summary>
	/// 0-5 W units of (64-n)/256s (0-63)
	/// </summary>
	GBReg SOUND2CNT_L_Sound_length;
	/// <summary>
	/// 6-7 R/W (0-3, see below)
	/// </summary>
	GBReg SOUND2CNT_L_Wave_Pattern_Duty;
	/// <summary>
	/// 8-10 R/W units of n/64s (1-7, 0=No Envelope)
	/// </summary>
	GBReg SOUND2CNT_L_Envelope_Step_Time;
	/// <summary>
	/// 11 R/W (0=Decrease, 1=Increase)
	/// </summary>
	GBReg SOUND2CNT_L_Envelope_Direction;
	/// <summary>
	/// 12-15 R/W (1-15, 0=No Sound)
	/// </summary>
	GBReg SOUND2CNT_L_Initial_Volume_of_envelope;
	/// <summary>
	/// Channel 2 Frequency/Control (NR23, NR24)
	/// </summary>
	GBReg SOUND2CNT_H;
	/// <summary>
	/// 0-10 W 131072/(2048-n)Hz (0-2047)
	/// </summary>
	GBReg SOUND2CNT_H_Frequency;
	/// <summary>
	/// 14 R/W (1=Stop output when length in NR11 expires)
	/// </summary>
	GBReg SOUND2CNT_H_Length_Flag;
	/// <summary>
	/// 15 W (1=Restart Sound)
	/// </summary>
	GBReg SOUND2CNT_H_Initial;
	/// <summary>
	/// must return zero
	/// </summary>
	GBReg SOUND2CNT_HHighZero;
	/// <summary>
	/// Channel 3 Stop/Wave RAM select (NR30)
	/// </summary>
	GBReg SOUND3CNT_L;
	/// <summary>
	/// 5 R/W (0=One bank/32 digits, 1=Two banks/64 digits)
	/// </summary>
	GBReg SOUND3CNT_L_Wave_RAM_Dimension;
	/// <summary>
	/// 6 R/W (0-1, see below)
	/// </summary>
	GBReg SOUND3CNT_L_Wave_RAM_Bank_Number;
	/// <summary>
	/// 7 R/W (0=Stop, 1=Playback)
	/// </summary>
	GBReg SOUND3CNT_L_Sound_Channel_3_Off;
	/// <summary>
	/// Channel 3 Length/Volume (NR31, NR32)
	/// </summary>
	GBReg SOUND3CNT_H;
	/// <summary>
	/// 0-7 W units of (256-n)/256s (0-255)
	/// </summary>
	GBReg SOUND3CNT_H_Sound_length;
	/// <summary>
	/// 13-14 R/W (0=Mute/Zero, 1=100%, 2=50%, 3=25%)
	/// </summary>
	GBReg SOUND3CNT_H_Sound_Volume;
	/// <summary>
	/// 15 R/W (0=Use above, 1=Force 75% regardless of above)
	/// </summary>
	GBReg SOUND3CNT_H_Force_Volume;
	/// <summary>
	/// Channel 3 Frequency/Control (NR33, NR34)
	/// </summary>
	GBReg SOUND3CNT_X;
	/// <summary>
	/// 0-10 W 2097152/(2048-n) Hz (0-2047)
	/// </summary>
	GBReg SOUND3CNT_X_Sample_Rate;
	/// <summary>
	/// 14 R/W (1=Stop output when length in NR31 expires)
	/// </summary>
	GBReg SOUND3CNT_X_Length_Flag;
	/// <summary>
	/// 15 W (1=Restart Sound)
	/// </summary>
	GBReg SOUND3CNT_X_Initial;
	/// <summary>
	/// must return zero
	/// </summary>
	GBReg SOUND3CNT_XHighZero;
	/// <summary>
	/// Channel 4 Length/Envelope (NR41, NR42)
	/// </summary>
	GBReg SOUND4CNT_L;
	/// <summary>
	/// 0-5 W units of (64-n)/256s (0-63)
	/// </summary>
	GBReg SOUND4CNT_L_Sound_length;
	/// <summary>
	/// 8-10 R/W units of n/64s (1-7, 0=No Envelope)
	/// </summary>
	GBReg SOUND4CNT_L_Envelope_Step_Time;
	/// <summary>
	/// 11 R/W (0=Decrease, 1=Increase)
	/// </summary>
	GBReg SOUND4CNT_L_Envelope_Direction;
	/// <summary>
	/// 12-15 R/W (1-15, 0=No Sound)
	/// </summary>
	GBReg SOUND4CNT_L_Initial_Volume_of_envelope;
	/// <summary>
	/// must return zero
	/// </summary>
	GBReg SOUND4CNT_LHighZero;
	/// <summary>
	/// Channel 4 Frequency/Control (NR43, NR44)
	/// </summary>
	GBReg SOUND4CNT_H;
	/// <summary>
	/// 0-2 R/W (r) 524288 Hz / r / 2^(s+1) ;For r=0 assume r=0.5 instead
	/// </summary>
	GBReg SOUND4CNT_H_Dividing_Ratio_of_Freq;
	/// <summary>
	/// 3 R/W (0=15 bits, 1=7 bits)
	/// </summary>
	GBReg SOUND4CNT_H_Counter_Step_Width;
	/// <summary>
	/// 4-7 R/W (s) 524288 Hz / r / 2^(s+1) ;For r=0 assume r=0.5 instead
	/// </summary>
	GBReg SOUND4CNT_H_Shift_Clock_Frequency;
	/// <summary>
	/// 14 R/W (1=Stop output when length in NR41 expires)
	/// </summary>
	GBReg SOUND4CNT_H_Length_Flag;
	/// <summary>
	/// 15 W (1=Restart Sound)
	/// </summary>
	GBReg SOUND4CNT_H_Initial;
	/// <summary>
	/// must return zero
	/// </summary>
	GBReg SOUND4CNT_HHighZero;
	/// <summary>
	/// Control Stereo/Volume/Enable (NR50, NR51)
	/// </summary>
	GBReg SOUNDCNT_L;
	/// <summary>
	/// 0-2 (0-7)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_1_4_Master_Volume_RIGHT;
	/// <summary>
	/// 4-6 (0-7)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_1_4_Master_Volume_LEFT;
	/// <summary>
	/// 8-11 (each Bit 8-11, 0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_1_Enable_Flags_RIGHT;
	/// <summary>
	/// 8-11 (each Bit 8-11, 0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_2_Enable_Flags_RIGHT;
	/// <summary>
	/// 8-11 (each Bit 8-11, 0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_3_Enable_Flags_RIGHT;
	/// <summary>
	/// 8-11 (each Bit 8-11, 0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_4_Enable_Flags_RIGHT;
	/// <summary>
	/// 12-15 (each Bit 12-15, 0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_1_Enable_Flags_LEFT;
	/// <summary>
	/// 12-15 (each Bit 12-15, 0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_2_Enable_Flags_LEFT;
	/// <summary>
	/// 12-15 (each Bit 12-15, 0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_3_Enable_Flags_LEFT;
	/// <summary>
	/// 12-15 (each Bit 12-15, 0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_L_Sound_4_Enable_Flags_LEFT;
	/// <summary>
	/// Control Mixing/DMA Control
	/// </summary>
	GBReg SOUNDCNT_H;
	/// <summary>
	/// 0-1 Sound # 1-4 Volume (0=25%, 1=50%, 2=100%, 3=Prohibited)
	/// </summary>
	GBReg SOUNDCNT_H_Sound_1_4_Volume;
	/// <summary>
	/// 2 DMA Sound A Volume (0=50%, 1=100%)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_A_Volume;
	/// <summary>
	/// 3 DMA Sound B Volume (0=50%, 1=100%)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_B_Volume;
	/// <summary>
	/// 8 DMA Sound A Enable RIGHT (0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_A_Enable_RIGHT;
	/// <summary>
	/// 9 DMA Sound A Enable LEFT (0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_A_Enable_LEFT;
	/// <summary>
	/// 10 DMA Sound A Timer Select (0=Timer 0, 1=Timer 1)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_A_Timer_Select;
	/// <summary>
	/// 11 DMA Sound A Reset FIFO (1=Reset)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_A_Reset_FIFO;
	/// <summary>
	/// 12 DMA Sound B Enable RIGHT (0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_B_Enable_RIGHT;
	/// <summary>
	/// 13 DMA Sound B Enable LEFT (0=Disable, 1=Enable)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_B_Enable_LEFT;
	/// <summary>
	/// 14 DMA Sound B Timer Select (0=Timer 0, 1=Timer 1)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_B_Timer_Select;
	/// <summary>
	/// 15 DMA Sound B Reset FIFO (1=Reset)
	/// </summary>
	GBReg SOUNDCNT_H_DMA_Sound_B_Reset_FIFO;
	/// <summary>
	/// Control Sound on/off (NR52)
	/// </summary>
	GBReg SOUNDCNT_X;
	/// <summary>
	/// 0 (Read Only)
	/// </summary>
	GBReg SOUNDCNT_X_Sound_1_ON_flag;
	/// <summary>
	/// 1 (Read Only)
	/// </summary>
	GBReg SOUNDCNT_X_Sound_2_ON_flag;
	/// <summary>
	/// 2 (Read Only)
	/// </summary>
	GBReg SOUNDCNT_X_Sound_3_ON_flag;
	/// <summary>
	/// 3 (Read Only)
	/// </summary>
	GBReg SOUNDCNT_X_Sound_4_ON_flag;
	/// <summary>
	/// 7 (0=Disable, 1=Enable) (Read/Write)
	/// </summary>
	GBReg SOUNDCNT_X_PSG_FIFO_Master_Enable;
	/// <summary>
	/// must return zero
	/// </summary>
	GBReg SOUNDCNT_XHighZero;
	/// <summary>
	/// Sound PWM Control (R/W)
	/// </summary>
	GBReg SOUNDBIAS;
	/// <summary>
	/// 0-9 (Default=200h, converting signed samples into unsigned)
	/// </summary>
	GBReg SOUNDBIAS_Bias_Level;
	/// <summary>
	/// 14-15 (Default=0, see below)
	/// </summary>
	GBReg SOUNDBIAS_Amp_Res_Sampling_Cycle;
	/// <summary>
	/// must return zero
	/// </summary>
	GBReg SOUNDBIAS_HighZero;
	/// <summary>
	/// Channel 3 Wave Pattern RAM (2 banks!!)
	/// </summary>
	GBReg WAVE_RAM;
	/// <summary>
	/// Channel 3 Wave Pattern RAM (2 banks!!)
	/// </summary>
	GBReg WAVE_RAM2;
	/// <summary>
	/// Channel 3 Wave Pattern RAM (2 banks!!)
	/// </summary>
	GBReg WAVE_RAM3;
	/// <summary>
	/// Channel 3 Wave Pattern RAM (2 banks!!)
	/// </summary>
	GBReg WAVE_RAM4;
	/// <summary>
	/// Channel A FIFO, Data 0-3
	/// </summary>
	GBReg FIFO_A;
	/// <summary>
	/// Channel B FIFO, Data 0-3
	/// </summary>
	GBReg FIFO_B;

	RegSect_sound();
};

class RegSect_dma
{
public:
	/// <summary>
	/// Source Address 4 W
	/// </summary>
	GBReg DMA0SAD;
	/// <summary>
	/// Destination Address 4 W
	/// </summary>
	GBReg DMA0DAD;
	/// <summary>
	/// Word Count 2 W
	/// </summary>
	GBReg DMA0CNT_L;
	/// <summary>
	/// Control 2 R/W
	/// </summary>
	GBReg DMA0CNT_H;
	/// <summary>
	/// 5-6 Dest Addr Control (0=Increment,1=Decrement,2=Fixed,3=Increment/Reload)
	/// </summary>
	GBReg DMA0CNT_H_Dest_Addr_Control;
	/// <summary>
	/// 7-8 Source Adr Control (0=Increment,1=Decrement,2=Fixed,3=Prohibited)
	/// </summary>
	GBReg DMA0CNT_H_Source_Adr_Control;
	/// <summary>
	/// 9 DMA Repeat (0=Off, 1=On) (Must be zero if Bit 11 set)
	/// </summary>
	GBReg DMA0CNT_H_DMA_Repeat;
	/// <summary>
	/// 10 DMA Transfer Type (0=16bit, 1=32bit)
	/// </summary>
	GBReg DMA0CNT_H_DMA_Transfer_Type;
	/// <summary>
	/// 12-13 DMA Start Timing (0=Immediately, 1=VBlank, 2=HBlank, 3=Special) The 'Special' setting (Start Timing=3) depends on the DMA channel: DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture
	/// </summary>
	GBReg DMA0CNT_H_DMA_Start_Timing;
	/// <summary>
	/// 14 IRQ upon end of Word Count (0=Disable, 1=Enable)
	/// </summary>
	GBReg DMA0CNT_H_IRQ_on;
	/// <summary>
	/// 15 DMA Enable (0=Off, 1=On)
	/// </summary>
	GBReg DMA0CNT_H_DMA_Enable;
	/// <summary>
	/// Source Address 4 W
	/// </summary>
	GBReg DMA1SAD;
	/// <summary>
	/// Destination Address 4 W
	/// </summary>
	GBReg DMA1DAD;
	/// <summary>
	/// Word Count 2 W
	/// </summary>
	GBReg DMA1CNT_L;
	/// <summary>
	/// Control 2 R/W
	/// </summary>
	GBReg DMA1CNT_H;
	/// <summary>
	/// 5-6 Dest Addr Control (0=Increment,1=Decrement,2=Fixed,3=Increment/Reload)
	/// </summary>
	GBReg DMA1CNT_H_Dest_Addr_Control;
	/// <summary>
	/// 7-8 Source Adr Control (0=Increment,1=Decrement,2=Fixed,3=Prohibited)
	/// </summary>
	GBReg DMA1CNT_H_Source_Adr_Control;
	/// <summary>
	/// 9 DMA Repeat (0=Off, 1=On) (Must be zero if Bit 11 set)
	/// </summary>
	GBReg DMA1CNT_H_DMA_Repeat;
	/// <summary>
	/// 10 DMA Transfer Type (0=16bit, 1=32bit)
	/// </summary>
	GBReg DMA1CNT_H_DMA_Transfer_Type;
	/// <summary>
	/// 12-13 DMA Start Timing (0=Immediately, 1=VBlank, 2=HBlank, 3=Special) The 'Special' setting (Start Timing=3) depends on the DMA channel: DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture
	/// </summary>
	GBReg DMA1CNT_H_DMA_Start_Timing;
	/// <summary>
	/// 14 IRQ upon end of Word Count (0=Disable, 1=Enable)
	/// </summary>
	GBReg DMA1CNT_H_IRQ_on;
	/// <summary>
	/// 15 DMA Enable (0=Off, 1=On)
	/// </summary>
	GBReg DMA1CNT_H_DMA_Enable;
	/// <summary>
	/// Source Address 4 W
	/// </summary>
	GBReg DMA2SAD;
	/// <summary>
	/// Destination Address 4 W
	/// </summary>
	GBReg DMA2DAD;
	/// <summary>
	/// Word Count 2 W
	/// </summary>
	GBReg DMA2CNT_L;
	/// <summary>
	/// Control 2 R/W
	/// </summary>
	GBReg DMA2CNT_H;
	/// <summary>
	/// 5-6 Dest Addr Control (0=Increment,1=Decrement,2=Fixed,3=Increment/Reload)
	/// </summary>
	GBReg DMA2CNT_H_Dest_Addr_Control;
	/// <summary>
	/// 7-8 Source Adr Control (0=Increment,1=Decrement,2=Fixed,3=Prohibited)
	/// </summary>
	GBReg DMA2CNT_H_Source_Adr_Control;
	/// <summary>
	/// 9 DMA Repeat (0=Off, 1=On) (Must be zero if Bit 11 set)
	/// </summary>
	GBReg DMA2CNT_H_DMA_Repeat;
	/// <summary>
	/// 10 DMA Transfer Type (0=16bit, 1=32bit)
	/// </summary>
	GBReg DMA2CNT_H_DMA_Transfer_Type;
	/// <summary>
	/// 12-13 DMA Start Timing (0=Immediately, 1=VBlank, 2=HBlank, 3=Special) The 'Special' setting (Start Timing=3) depends on the DMA channel: DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture
	/// </summary>
	GBReg DMA2CNT_H_DMA_Start_Timing;
	/// <summary>
	/// 14 IRQ upon end of Word Count (0=Disable, 1=Enable)
	/// </summary>
	GBReg DMA2CNT_H_IRQ_on;
	/// <summary>
	/// 15 DMA Enable (0=Off, 1=On)
	/// </summary>
	GBReg DMA2CNT_H_DMA_Enable;
	/// <summary>
	/// Source Address 4 W
	/// </summary>
	GBReg DMA3SAD;
	/// <summary>
	/// Destination Address 4 W
	/// </summary>
	GBReg DMA3DAD;
	/// <summary>
	/// Word Count 2 W
	/// </summary>
	GBReg DMA3CNT_L;
	/// <summary>
	/// Control 2 R/W
	/// </summary>
	GBReg DMA3CNT_H;
	/// <summary>
	/// 5-6 Dest Addr Control (0=Increment,1=Decrement,2=Fixed,3=Increment/Reload)
	/// </summary>
	GBReg DMA3CNT_H_Dest_Addr_Control;
	/// <summary>
	/// 7-8 Source Adr Control (0=Increment,1=Decrement,2=Fixed,3=Prohibited)
	/// </summary>
	GBReg DMA3CNT_H_Source_Adr_Control;
	/// <summary>
	/// 9 DMA Repeat (0=Off, 1=On) (Must be zero if Bit 11 set)
	/// </summary>
	GBReg DMA3CNT_H_DMA_Repeat;
	/// <summary>
	/// 10 DMA Transfer Type (0=16bit, 1=32bit)
	/// </summary>
	GBReg DMA3CNT_H_DMA_Transfer_Type;
	/// <summary>
	/// 11 Game Pak DRQ - DMA3 only - (0=Normal, 1=DRQ <from> Game Pak, DMA3)
	/// </summary>
	GBReg DMA3CNT_H_Game_Pak_DRQ;
	/// <summary>
	/// 12-13 DMA Start Timing (0=Immediately, 1=VBlank, 2=HBlank, 3=Special) The 'Special' setting (Start Timing=3) depends on the DMA channel: DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture
	/// </summary>
	GBReg DMA3CNT_H_DMA_Start_Timing;
	/// <summary>
	/// 14 IRQ upon end of Word Count (0=Disable, 1=Enable)
	/// </summary>
	GBReg DMA3CNT_H_IRQ_on;
	/// <summary>
	/// 15 DMA Enable (0=Off, 1=On)
	/// </summary>
	GBReg DMA3CNT_H_DMA_Enable;

	RegSect_dma();
};

class RegSect_timer
{
public:
	/// <summary>
	/// Timer 0 Counter/Reload 2 R/W
	/// </summary>
	GBReg TM0CNT_L;
	/// <summary>
	/// Timer 0 Control 2 R/W
	/// </summary>
	GBReg TM0CNT_H;
	/// <summary>
	/// Prescaler Selection (0=F/1, 1=F/64, 2=F/256, 3=F/1024)
	/// </summary>
	GBReg TM0CNT_H_Prescaler;
	/// <summary>
	/// Count-up Timing (0=Normal, 1=See below)
	/// </summary>
	GBReg TM0CNT_H_Count_up;
	/// <summary>
	/// Timer IRQ Enable (0=Disable, 1=IRQ on Timer overflow)
	/// </summary>
	GBReg TM0CNT_H_Timer_IRQ_Enable;
	/// <summary>
	/// Timer Start/Stop (0=Stop, 1=Operate)
	/// </summary>
	GBReg TM0CNT_H_Timer_Start_Stop;
	/// <summary>
	/// Timer 1 Counter/Reload 2 R/W
	/// </summary>
	GBReg TM1CNT_L;
	/// <summary>
	/// Timer 1 Control 2 R/W
	/// </summary>
	GBReg TM1CNT_H;
	/// <summary>
	/// Prescaler Selection (0=F/1, 1=F/64, 2=F/256, 3=F/1024)
	/// </summary>
	GBReg TM1CNT_H_Prescaler;
	/// <summary>
	/// Count-up Timing (0=Normal, 1=See below)
	/// </summary>
	GBReg TM1CNT_H_Count_up;
	/// <summary>
	/// Timer IRQ Enable (0=Disable, 1=IRQ on Timer overflow)
	/// </summary>
	GBReg TM1CNT_H_Timer_IRQ_Enable;
	/// <summary>
	/// Timer Start/Stop (0=Stop, 1=Operate)
	/// </summary>
	GBReg TM1CNT_H_Timer_Start_Stop;
	/// <summary>
	/// Timer 2 Counter/Reload 2 R/W
	/// </summary>
	GBReg TM2CNT_L;
	/// <summary>
	/// Timer 2 Control 2 R/W
	/// </summary>
	GBReg TM2CNT_H;
	/// <summary>
	/// Prescaler Selection (0=F/1, 1=F/64, 2=F/256, 3=F/1024)
	/// </summary>
	GBReg TM2CNT_H_Prescaler;
	/// <summary>
	/// Count-up Timing (0=Normal, 1=See below)
	/// </summary>
	GBReg TM2CNT_H_Count_up;
	/// <summary>
	/// Timer IRQ Enable (0=Disable, 1=IRQ on Timer overflow)
	/// </summary>
	GBReg TM2CNT_H_Timer_IRQ_Enable;
	/// <summary>
	/// Timer Start/Stop (0=Stop, 1=Operate)
	/// </summary>
	GBReg TM2CNT_H_Timer_Start_Stop;
	/// <summary>
	/// Timer 3 Counter/Reload 2 R/W
	/// </summary>
	GBReg TM3CNT_L;
	/// <summary>
	/// Timer 3 Control 2 R/W
	/// </summary>
	GBReg TM3CNT_H;
	/// <summary>
	/// Prescaler Selection (0=F/1, 1=F/64, 2=F/256, 3=F/1024)
	/// </summary>
	GBReg TM3CNT_H_Prescaler;
	/// <summary>
	/// Count-up Timing (0=Normal, 1=See below)
	/// </summary>
	GBReg TM3CNT_H_Count_up;
	/// <summary>
	/// Timer IRQ Enable (0=Disable, 1=IRQ on Timer overflow)
	/// </summary>
	GBReg TM3CNT_H_Timer_IRQ_Enable;
	/// <summary>
	/// Timer Start/Stop (0=Stop, 1=Operate)
	/// </summary>
	GBReg TM3CNT_H_Timer_Start_Stop;

	RegSect_timer();
};

class RegSect_serial
{
public:
	/// <summary>
	/// SIO Data (Normal-32bit Mode; shared with below) 4 R/W
	/// </summary>
	GBReg SIODATA32;
	/// <summary>
	/// SIO Data 0 (Parent) (Multi-Player Mode) 2 R/W
	/// </summary>
	GBReg SIOMULTI0;
	/// <summary>
	/// SIO Data 1 (1st Child) (Multi-Player Mode) 2 R/W
	/// </summary>
	GBReg SIOMULTI1;
	/// <summary>
	/// SIO Data 2 (2nd Child) (Multi-Player Mode) 2 R/W
	/// </summary>
	GBReg SIOMULTI2;
	/// <summary>
	/// SIO Data 3 (3rd Child) (Multi-Player Mode) 2 R/W
	/// </summary>
	GBReg SIOMULTI3;
	/// <summary>
	/// SIO Control Register 2 R/W
	/// </summary>
	GBReg SIOCNT;
	/// <summary>
	/// SIO Data (Local of MultiPlayer; shared below) 2 R/W
	/// </summary>
	GBReg SIOMLT_SEND;
	/// <summary>
	/// SIO Data (Normal-8bit and UART Mode) 2 R/W
	/// </summary>
	GBReg SIODATA8;
	/// <summary>
	/// SIO Mode Select/General Purpose Data 2 R/W
	/// </summary>
	GBReg RCNT;
	/// <summary>
	/// Ancient - Infrared Register (Prototypes only) - -
	/// </summary>
	GBReg IR;
	/// <summary>
	/// SIO JOY Bus Control 2 R/W
	/// </summary>
	GBReg JOYCNT;
	/// <summary>
	/// SIO JOY Bus Receive Data 4 R/W
	/// </summary>
	GBReg JOY_RECV;
	/// <summary>
	/// SIO JOY Bus Transmit Data 4 R/W
	/// </summary>
	GBReg JOY_TRANS;
	/// <summary>
	/// SIO JOY Bus Receive Status 2 R/?
	/// </summary>
	GBReg JOYSTAT;

	RegSect_serial();
};

class RegSect_keypad
{
public:
	/// <summary>
	/// Key Status 2 R
	/// </summary>
	GBReg KEYINPUT;
	/// <summary>
	/// Key Interrupt Control 2 R/W
	/// </summary>
	GBReg KEYCNT;

	RegSect_keypad();
};

class RegSect_system
{
public:
	/// <summary>
	/// Interrupt Enable Register
	/// </summary>
	GBReg IE;
	/// <summary>
	/// Interrupt Request Flags / IRQ Acknowledge
	/// </summary>
	GBReg IF;
	/// <summary>
	/// Game Pak Waitstate Control
	/// </summary>
	GBReg WAITCNT;
	/// <summary>
	/// Interrupt Master Enable Register
	/// </summary>
	GBReg IME;
	/// <summary>
	/// Undocumented - Post Boot Flag
	/// </summary>
	GBReg POSTFLG;
	/// <summary>
	/// Undocumented - Power Down Control
	/// </summary>
	GBReg HALTCNT;
	/// <summary>
	/// Undocumented - Internal Memory Control (R/W) -- Mirrors of 4000800h (repeated each 64K)
	/// </summary>
	GBReg MemCtrl;

	RegSect_system();
};

class GBREGS
{
public:
	RegSect_display Sect_display;
	RegSect_sound Sect_sound;
	RegSect_dma Sect_dma;
	RegSect_timer Sect_timer;
	RegSect_serial Sect_serial;
	RegSect_keypad Sect_keypad;
	RegSect_system Sect_system;

	byte data[2052];
	byte rwmask[2052];

	void reset();
};
extern GBREGS GBRegs;
