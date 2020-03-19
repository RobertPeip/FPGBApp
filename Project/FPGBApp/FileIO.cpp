#include "FileIO.h"
#include "SDL.h"

FILEIO FileIO;

int FILEIO::readfile(void* target, string filename, bool absolutePath)
{
	string fullname;
	if (absolutePath)
	{
		fullname = filename;
	}
	else
	{
		char* path = SDL_GetBasePath();
		fullname = path + filename;
	}

	SDL_RWops* rw = SDL_RWFromFile(fullname.c_str(), "rb");
	if (rw == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "File not found", fullname.c_str(), NULL);
		return 0;
	}

	Sint64 res_size = SDL_RWsize(rw);

	Sint64 nb_read_total = 0, nb_read = 1;
	char* buf = (char*)target;
	while (nb_read_total < res_size && nb_read != 0) 
	{
		nb_read = SDL_RWread(rw, target, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);

	return res_size;
}