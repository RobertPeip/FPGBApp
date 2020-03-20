#include <iostream>
#include <algorithm>
#include "SDL.h"
using namespace std;

//#define CONSOLE

#include <windows.h>

#include "gameboy.h"
#include "GPU.h"
#include "Joypad.h"
#include "CPU.h"
#include "FileIO.h"
#include "Memory.h"
#include "OSD.h"

const int WIDTH = 240;
const int HEIGHT = 160;

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

void savegame()
{
	Memory.createGameRAMSnapshot = true;
	while (Memory.createGameRAMSnapshot)
	{
		SDL_Delay(1);
	}
	Memory.save_gameram(gameboy.filename);
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

void loadstate_fromdisk_auto(string filename)
{
	string savefilename = filename.substr(filename.find_last_of("/\\") + 1);
	savefilename = savefilename.substr(0, savefilename.find_last_of(".") + 1) + "sst";
	FileIO.readfile(gameboy.savestate, savefilename, false);
	loadstate();
}

void savestate_todisk(string filename)
{
	string savefilename = filename.substr(filename.find_last_of("/\\") + 1);
	savefilename = savefilename.substr(0, savefilename.find_last_of(".") + 1) + "sst";
	FileIO.writefile(gameboy.savestate, savefilename, 131072 * 4,false);
}

void openrom()
{
	if (gameboy.filename != "")
	{
		OSD.isOpen = false;
		if (gbthread != 0)
		{
			gameboy.on = false;
			int threadReturnValue;
			SDL_WaitThread(gbthread, &threadReturnValue);
		}
		gameboy.coldreset = true;
		emuframes = 0;
		gbthread = SDL_CreateThread(emu, "emuthread", (void*)NULL);
	}
}

void set_displaysize(int mult, bool fullscreen)
{
	if (fullscreen)
	{
		OSD.displaysize = 0;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		OSD.displaysize = mult;
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, WIDTH * mult, HEIGHT * mult);
	}
}

void drawer()
{
	window = SDL_CreateWindow ("FPGBApp", 200, 200,WIDTH * 4, HEIGHT * 4, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
	SDL_RenderSetIntegerScale(renderer, SDL_bool::SDL_TRUE);
	mutex = SDL_CreateMutex();
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	// gamepad
	SDL_GameController* controller = NULL;
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			controller = SDL_GameControllerOpen(i);
			if (controller) 
			{
				break;
			}
		}
	}


	// drawer
	Uint64 currentTime = SDL_GetPerformanceCounter();
	Uint64 lastTime_frame = SDL_GetPerformanceCounter();
	Uint64 lastTime_second = SDL_GetPerformanceCounter();
	Uint64 lastTime_idle = SDL_GetPerformanceCounter();
	double delta = 0;

	unsigned short frames = 0;

	int OSDidlecheck = 0;

#ifdef CONSOLE
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

	const long frametime = (1000000 / 60);

	UInt64 oldcycles = 0;
	UInt64 oldcommands = 0;

	bool running = true;
	while (running)
	{
		currentTime = SDL_GetPerformanceCounter();

		double frametimeleft = frametime;

		delta = (double)((currentTime - lastTime_frame) * 1000000 / (double)SDL_GetPerformanceFrequency());
		if (delta >= frametimeleft)
		{
			frametimeleft = max(5000, frametimeleft + frametime - delta);
			lastTime_frame = SDL_GetPerformanceCounter();

			if (OSD.isOpen)
			{
				OSD.render(renderer);
			}
			else
			{
				GPU.draw_game();
				SDL_UpdateTexture(framebuffer, NULL, GPU.buffer, WIDTH * sizeof(uint32_t));
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
			}

			SDL_RenderPresent(renderer);
			SDL_PumpEvents();

			frames++;

			if (OSD.isOpen)
			{
				gameboy.pause = true;
				if (keystate[SDL_SCANCODE_ESCAPE] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK))
				{
					if (OSD.idle)
					{
						OSD.isOpen = false;
						OSD.idle = false;
					}
				}
				else if (keystate[SDL_SCANCODE_UP] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
				{
					OSD.selectPrev();
				}
				else if (keystate[SDL_SCANCODE_DOWN] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
				{
					OSD.selectNext();
				}
				else if (keystate[SDL_SCANCODE_LEFT] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
				{
					OSD.largePrev();
				}
				else if (keystate[SDL_SCANCODE_RIGHT] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
				{
					OSD.largeNext();
				}
				else if (keystate[SDL_SCANCODE_A] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
				{
					if (OSD.idle)
					{
						OSD.idle = false;
						if (OSD.OsdType == OSDTYPE::MAIN)
						{
							switch ((OSDMAINMENU)OSD.selected)
							{
							case OSDMAINMENU::LOADGAME: OSD.gotoLoadGame(); break;
							case OSDMAINMENU::SAVESTATE: savestate(); OSD.isOpen = false; break;
							case OSDMAINMENU::SAVEMEMDISK: savegame(); OSD.isOpen = false; break;
							case OSDMAINMENU::SAVESTATEDISK: savestate_todisk(gameboy.filename); OSD.isOpen = false; break;
							case OSDMAINMENU::LOADSTATE: loadstate(); OSD.isOpen = false; break;
							case OSDMAINMENU::LOADSTATEDISK: loadstate_fromdisk_auto(gameboy.filename); OSD.isOpen = false; break;
							case OSDMAINMENU::DISPLAYSIZE:
								OSD.displaysize += 1;
								if (OSD.displaysize > 6)
									OSD.displaysize = 0;
								if (OSD.displaysize == 0)
								{
									set_displaysize(0, true);
									OSD.exchangeText((int)OSDMAINMENU::DISPLAYSIZE, "Displaysize: Fullscreen");
								}
								else
								{
									set_displaysize(OSD.displaysize, false);
									OSD.exchangeText((int)OSDMAINMENU::DISPLAYSIZE, "Displaysize: " + std::to_string(OSD.displaysize));
								}
								break;
							case OSDMAINMENU::FLICKERBLEND:
								GPU.interlace_blending = !GPU.interlace_blending;
								if (GPU.interlace_blending)
								{
									OSD.exchangeText((int)OSDMAINMENU::FLICKERBLEND, "Flickerblend: on");
								}
								else
								{
									OSD.exchangeText((int)OSDMAINMENU::FLICKERBLEND, "Flickerblend: off");
								}
								break;
							case OSDMAINMENU::CPUSTEPS:
								CPU.additional_steps += 1;
								if (CPU.additional_steps > 10)
									CPU.additional_steps = -10;
								OSD.exchangeText((int)OSDMAINMENU::CPUSTEPS, "CPU Steps: " + std::to_string(CPU.additional_steps));
								break;
							case OSDMAINMENU::EXIT: running = false; break;
							}
						}
						else if (OSD.OsdType == OSDTYPE::LOAD)
						{
							if (OSD.selectFile())
							{
								openrom();
								OSD.isOpen = false;
							}
						}
					}
				}
				else if (keystate[SDL_SCANCODE_S] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
				{
					if (OSD.idle)
					{
						OSD.idle = false;
						if (OSD.OsdType == OSDTYPE::MAIN)
						{
							OSD.isOpen = false;
						}
						else if (OSD.OsdType == OSDTYPE::LOAD)
						{
							OSD.gotoMain();
						}
					}
				}
				else
				{
					OSD.idle = true;
				}
			}
			else
			{
				gameboy.pause = false;

				Joypad.KeyA = keystate[SDL_SCANCODE_A] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
				Joypad.KeyB = keystate[SDL_SCANCODE_S] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X);
				Joypad.KeyAToggle = keystate[SDL_SCANCODE_D];
				Joypad.KeyBToggle = keystate[SDL_SCANCODE_F];
				Joypad.KeyL = keystate[SDL_SCANCODE_Q] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
				Joypad.KeyR = keystate[SDL_SCANCODE_W] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
				Joypad.KeyStart = keystate[SDL_SCANCODE_X] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START);
				Joypad.KeySelect = keystate[SDL_SCANCODE_Y] | keystate[SDL_SCANCODE_Z] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
				Joypad.KeyLeft = keystate[SDL_SCANCODE_LEFT] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
				Joypad.KeyRight = keystate[SDL_SCANCODE_RIGHT] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
				Joypad.KeyUp = keystate[SDL_SCANCODE_UP] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
				Joypad.KeyDown = keystate[SDL_SCANCODE_DOWN] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);

				if (keystate[SDL_SCANCODE_ESCAPE] | SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK))
				{
					if (OSD.idle)
					{
						OSD.gotoMain();
						OSD.isOpen = true;
						OSD.idle = false;
					}
				}
				else
				{
					OSD.idle = true;
				}

				if (keystate[SDL_SCANCODE_F5])
				{
					savestate();
				}
				if (keystate[SDL_SCANCODE_F6])
				{
					savegame();
				}
				if (keystate[SDL_SCANCODE_F7])
				{
					savestate_todisk(gameboy.filename);
				}
				if (keystate[SDL_SCANCODE_F9])
				{
					loadstate();
				}
				if (keystate[SDL_SCANCODE_F10])
				{
					loadstate_fromdisk_auto(gameboy.filename);
				}

				if (keystate[SDL_SCANCODE_SPACE] || keystate[SDL_SCANCODE_0] || SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y))
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
			}

			if (keystate[SDL_SCANCODE_1]) { set_displaysize(1, false); }
			if (keystate[SDL_SCANCODE_2]) { set_displaysize(2, false); }
			if (keystate[SDL_SCANCODE_3]) { set_displaysize(3, false); }
			if (keystate[SDL_SCANCODE_4]) { set_displaysize(4, false); }
			if (keystate[SDL_SCANCODE_5]) { set_displaysize(5, false); }
			if (keystate[SDL_SCANCODE_6]) { set_displaysize(6, false); }
			if (keystate[SDL_SCANCODE_RETURN] && keystate[SDL_SCANCODE_LALT]) { set_displaysize(0, true); }
		}

		delta = (double)((currentTime - lastTime_idle) * 1000000 / (double)SDL_GetPerformanceFrequency());
		if (delta >= 10000)
		{
			if (!OSD.idle)
			{
				OSDidlecheck--;
				if (OSDidlecheck <= 0)
				{
					OSD.idle = true;
					OSDidlecheck = 20;
				}
			}
			else
			{
				OSDidlecheck = 50;
			}
			lastTime_idle = SDL_GetPerformanceCounter();
		}

		delta = (double)((currentTime - lastTime_second) * 1000000 / (double)SDL_GetPerformanceFrequency());
		if (delta >= 1000000)
		{
			UInt64 cpucycles = (UInt64)gameboy.cycles;
			double newcycles = (double)(cpucycles - oldcycles);
#ifdef CONSOLE
			std::cout << "CPU%: " << (int)(100 * newcycles / 16780000);
			std::cout << " | FPS: " << frames;
			std::cout << " | Intern FPS: " << GPU.intern_frames;
			std::cout << "(" << GPU.videomode_frames << ")";
			std::cout << " | AVG Cycles: " << (newcycles / (CPU.commands - oldcommands)) << "\n";
#endif
			SDL_SetWindowTitle(window, std::to_string(100 * newcycles / 16780000).c_str());

			lastTime_second = SDL_GetPerformanceCounter();
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

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER); // SDL_INIT_EVERYTHING
	TTF_Init();
	set_displaysize(4, false);
	OSD.init();


	GPU.drawlock = SDL_CreateMutex();

	// debug
	//gameboy.filename = "C:\test\suite_fillrate_10.gba";

	openrom();

	//loadstate_fromdisk("savestate.sst");

	drawer();

	return 0;
}