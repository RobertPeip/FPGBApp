#pragma once
#include "SDL.h"
#include "types.h"

class Pixel
{
public:
	Byte color_red;
	Byte color_green;
	Byte color_blue;
	bool transparent;
	byte prio;
	bool undrawn;
	bool objwnd;
	bool alpha;

	void update(Byte color_red, Byte color_green, Byte color_blue);
	void copy(Pixel source);
	void copycolor(Pixel source);
	void mixcolor(Pixel source, Pixel secondpixel);
	void calcalpha(Pixel secondpixel, byte eva, byte evb);
	void whiter(byte change);
	void blacker(byte change);
};

enum WINDOWSTATE
{
	Init,
	Win0,
	Win1,
	Obj,
	Out
};

class Gpu
{
public:
	bool lockSpeed = true;
	int speedmult = 1;
	int frameskip = 0;
	int frameskip_counter = 0;
	bool interlace_blending = false;
	bool colorshader = false;
	bool doubleres = false;
	bool SSAA4x = false;
	bool even_frame;

	SDL_mutex* drawlock;

	UInt64 intern_frames;
	UInt64 videomode_frames;
	bool framebuffer_select;

	// registers
	byte videomode;
	bool forcedblank;

	// static array, so less allocate
	unsigned int buffer[240 * 160];

	Pixel pixels[480][320];
	Pixel pixels_interlace[480][320];

	Pixel pixels_bg0[240];
	Pixel pixels_bg1[240];
	Pixel* pixels_bg2;
	Pixel pixels_bg2_1[480];
	Pixel pixels_bg2_2[480];
	Pixel pixels_bg3[240];
	Pixel pixels_obj[240];
	Pixel pixelbackdrop;
	Pixel pixelspecial;
	Pixel pixelalpha2;
	Pixel pixelfinal;

	Uint64 lastTime_frame;

	// on delay
	bool on_delay_bg0[3];
	bool on_delay_bg1[3];
	bool on_delay_bg2[3];
	bool on_delay_bg3[3];

	// affine bg
	Int32 ref2_x;
	Int32 ref2_y;
	Int32 ref3_x;
	Int32 ref3_y;

	Int32 ref2_x_next;
	Int32 ref2_y_next;
	bool ref2_x_new;
	bool ref2_y_new;
	Int32 ref3_x_next;
	Int32 ref3_y_next;
	bool ref3_x_new;
	bool ref3_y_new;

	// mosaic
	byte mosaik_bg0_vcnt;
	byte mosaik_bg1_vcnt;
	byte mosaik_bg2_vcnt;
	byte mosaik_bg3_vcnt;

	// frametime
	const long FRAMETIME = (1000000 / 60);
	long frametimeleft;

	void reset();
	void dispcnt_write();
	void refpoint_update_all();
	void refpoint_update_2x();
	void refpoint_update_2y();
	void refpoint_update_3x();
	void refpoint_update_3y();
	void refpoint_update_2x_new();
	void refpoint_update_2y_new();
	void refpoint_update_3x_new();
	void refpoint_update_3y_new();
	void once_per_hblank();
	void next_line(byte line);
	void draw_line(byte y_in);
	void draw_bg_mode0(Pixel pixelslocal[], byte y, UInt32 mapbase, UInt32 tilebase, bool hicolor, byte screensize, UInt16 scrollX, UInt16 scrollY);
	void draw_bg_mode2(Pixel pixelslocal[], UInt32 mapbase, UInt32 tilebase, bool wrapping, byte screensize, Int32 refX, Int32 refY, Int16 dx, Int16 dy, bool doubleres, bool is_bg2);
	void draw_bg_mode2_SSAA4x(Pixel pixelslocal[], UInt32 mapbase, UInt32 tilebase, bool wrapping, byte screensize, Int32 refX, Int32 refY, Int16 dx, Int16 dy, bool is_bg2);
	void draw_bg_mode3(Int32 refX, Int32 refY, Int16 dx, Int16 dy);
	void draw_bg_mode4(Int32 refX, Int32 refY, Int16 dx, Int16 dy);
	void draw_bg_mode5(Int32 refX, Int32 refY, Int16 dx, Int16 dy);
	void draw_obj(int y, int baseaddr);
	void draw_game();

//private:
	//void GetPointBounds(PointF[] points, out float xmin, out float xmax, out float ymin, out float ymax)
};
extern Gpu GPU;