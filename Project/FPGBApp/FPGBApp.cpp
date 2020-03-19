#include <iostream>
#include <algorithm>
#include "SDL.h"
using namespace std;

#include <windows.h>

#include "gameboy.h"
#include "GPU.h"
#include "Joypad.h"
#include "CPU.h"
#include "FileIO.h"

const int WIDTH = 240;
const int HEIGHT = 160;
int SCALE = 4;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* framebuffer;
SDL_mutex* mutex;

SDL_Thread* gbthread;

int emuframes;

bool speedunlock = false;

int emu(void* ptr)
{
	gameboy.run();
	return 0;
}

void savestate()
{
	gameboy.do_savestate = true;

	while (gameboy.do_savestate)
	{
		SDL_Delay(1);
	}
}

void loadstate()
{
	gameboy.do_loadstate = true;

	while (gameboy.do_loadstate)
	{
		SDL_Delay(1);
	}
}

void loadstate_fromdisk(string filename)
{
	FileIO.readfile(gameboy.savestate, filename, true);
	loadstate();
}

void drawer()
{
	SDL_Window* window = SDL_CreateWindow
	("FPGBApp", // window's title
		200, 200, // coordinates on the screen, in pixels, of the window's upper left corner
		WIDTH * SCALE, HEIGHT * SCALE, // window's length and height in pixels  
		SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
	SDL_RenderSetIntegerScale(renderer, SDL_bool::SDL_TRUE);
	mutex = SDL_CreateMutex();
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	// drawer
	Uint64 currentTime = SDL_GetPerformanceCounter();
	Uint64 lastTime_frame = SDL_GetPerformanceCounter();
	Uint64 lastTime_second = SDL_GetPerformanceCounter();
	double delta = 0;

	unsigned short frames = 0;

	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	const long frametime = (1000000 / 60);

	UInt64 oldcycles = 0;
	UInt64 oldcommands = 0;

	while (true)
	{
		currentTime = SDL_GetPerformanceCounter();

		double frametimeleft = frametime;

		delta = (double)((currentTime - lastTime_frame) * 1000000 / (double)SDL_GetPerformanceFrequency());
		if (delta >= frametimeleft)
		{
			frametimeleft = max(5000, frametimeleft + frametime - delta);
			lastTime_frame = SDL_GetPerformanceCounter();

			GPU.draw_game();
			SDL_UpdateTexture(framebuffer, NULL, GPU.buffer, WIDTH * sizeof(uint32_t));
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
			SDL_RenderPresent(renderer);
			SDL_PumpEvents();

			frames++;

			Joypad.KeyA = keystate[SDL_SCANCODE_A];
			Joypad.KeyB = keystate[SDL_SCANCODE_S];
			Joypad.KeyAToggle = keystate[SDL_SCANCODE_D];
			Joypad.KeyBToggle = keystate[SDL_SCANCODE_F];
			Joypad.KeyL = keystate[SDL_SCANCODE_Q];
			Joypad.KeyR = keystate[SDL_SCANCODE_W];
			Joypad.KeyStart = keystate[SDL_SCANCODE_X];
			Joypad.KeySelect = keystate[SDL_SCANCODE_Y] | keystate[SDL_SCANCODE_Z];
			Joypad.KeyLeft = keystate[SDL_SCANCODE_LEFT];
			Joypad.KeyRight = keystate[SDL_SCANCODE_RIGHT];
			Joypad.KeyUp = keystate[SDL_SCANCODE_UP];
			Joypad.KeyDown = keystate[SDL_SCANCODE_DOWN];

			if (keystate[SDL_SCANCODE_F5])
			{
				savestate();
			}
			if (keystate[SDL_SCANCODE_F9])
			{
				loadstate();
			}

			if (keystate[SDL_SCANCODE_SPACE] || keystate[SDL_SCANCODE_0])
			{
				GPU.lockSpeed = false;
				GPU.frameskip = 10;
				speedunlock = keystate[SDL_SCANCODE_0];
			}
			else if (!speedunlock && !keystate[SDL_SCANCODE_SPACE])
			{
				GPU.lockSpeed = true;
				GPU.frameskip = 0;
			}
			else if (speedunlock && keystate[SDL_SCANCODE_SPACE])
			{
				speedunlock = false;
			}

			if (keystate[SDL_SCANCODE_1])
			{
				SCALE = 1;
				SDL_SetWindowFullscreen(window, 0);
				SDL_SetWindowSize(window, WIDTH * SCALE, HEIGHT * SCALE);
			}
			if (keystate[SDL_SCANCODE_4])
			{
				SCALE = 4;
				SDL_SetWindowFullscreen(window, 0);
				SDL_SetWindowSize(window, WIDTH * SCALE, HEIGHT * SCALE);
			}
			if (keystate[SDL_SCANCODE_RETURN] && keystate[SDL_SCANCODE_LALT])
			{
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}

			if (keystate[SDL_SCANCODE_ESCAPE])
			{
				break;
			}
		}

		delta = (double)((currentTime - lastTime_second) * 1000000 / (double)SDL_GetPerformanceFrequency());
		if (delta >= 1000000)
		{
			UInt64 cpucycles = (UInt64)gameboy.cycles;
			double newcycles = (double)(cpucycles - oldcycles);
			std::cout << "CPU%: " << (int)(100 * newcycles / 16780000);
			std::cout << " | FPS: " << frames;
			std::cout << " | Intern FPS: " << GPU.intern_frames;
			std::cout << "(" << GPU.videomode_frames << ")";
			std::cout << " | AVG Cycles: " << (newcycles / (CPU.commands - oldcommands)) << "\n";
			SDL_SetWindowTitle(window, std::to_string(100 * newcycles / 16780000).c_str());

			lastTime_second = SDL_GetPerformanceCounter();
			//std::cout << frames << " " << GPU.intern_frames << text << "\n";
			frames = 0;
			if (SDL_LockMutex(mutex) == 0)
			{
				emuframes = 0;
				SDL_UnlockMutex(mutex);
			}
			if (SDL_LockMutex(GPU.drawlock) == 0)
			{
				GPU.intern_frames = 0;
				GPU.videomode_frames = 0;
				oldcycles = cpucycles;
				oldcommands = CPU.commands;
				SDL_UnlockMutex(GPU.drawlock);
			}
		}
	}

	gameboy.on = false;
	int threadReturnValue;
	SDL_WaitThread(gbthread, &threadReturnValue);

	SDL_DestroyWindow(window);
	SDL_Quit();
}

void openrom()
{
	if (gameboy.filename != "")
	{
		if (gbthread != 0)
		{
			int threadReturnValue;
			SDL_WaitThread(gbthread, &threadReturnValue);
		}
		gameboy.coldreset = true;
		emuframes = 0;
		gbthread = SDL_CreateThread(emu, "emuthread", (void*)NULL);
	}
}

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); // SDL_INIT_EVERYTHING

	GPU.drawlock = SDL_CreateMutex();

	// debug
	//gameboy.filename = "C:\test\suite_fillrate_10.gba";

	openrom();

	//loadstate_fromdisk("savestate.sst");

	drawer();

	return 0;
}