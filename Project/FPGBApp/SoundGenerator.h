#include <queue>
using namespace std;

#include "types.h"
#include "SoundChannel.h"
#include "SDL.h"

class SoundGenerator
{
public:
	SoundChannel soundchannels[4];

	std::queue<float> nextSamples;
	SDL_mutex* nextSamples_lock;

	UInt32 volume_left_1_4;
	UInt32 volume_right_1_4;

	float volume_1_4;
	float volume_dma0;
	float volume_dma1;

	bool enable_channels_left[4];
	bool enable_channels_right[4];

	SoundGenerator();
	void work();
	void fill();

	static Uint8* audio_chunk;
	static Uint32 audio_len;
	static Uint8* audio_pos;
};

extern void fill_audio(void* udata, Uint8* stream, int len);
