#include "SoundGenerator.h"
#include "SoundDMA.h"

SoundGenerator::SoundGenerator()
{
	volume_left_1_4 = 0;
	volume_right_1_4 = 0;

	float volume_1_4 = 0;
	float volume_dma0 = 0;
	float volume_dma1 = 0;

	for (int i = 0; i < 4; i++)
	{
		soundchannels[i].assign(i + 1);
	}

	for (int i = 0; i < 4; i++)
	{
		enable_channels_left[i] = false;
		enable_channels_right[i] = false;
	}

	nextSamples_lock = SDL_CreateMutex();

    SDL_AudioSpec wanted;
	wanted.callback = fill_audio;
	wanted.userdata = this;
	wanted.freq = 44100;
	wanted.format = AUDIO_S16;
	wanted.channels = 1;
	wanted.samples = 4096;

	SDL_OpenAudio(&wanted, NULL);
	SDL_PauseAudio(0);
}

void SoundGenerator::work()
{
	SoundDMA.soundDMAs[0].work();
	SoundDMA.soundDMAs[1].work();
}

void SoundGenerator::fill()
{
	if (SDL_LockMutex(nextSamples_lock) == 0)
	{
		if (nextSamples.size() < 5000)
		{
			float value = 0;
			for (int i = 0; i < 4; i++)
			{
				if (enable_channels_left[i] || enable_channels_right[i])
				{
					value += (soundchannels[i].get_next() * volume_1_4);
				}
			}
			value /= 2;

			value = value * (volume_left_1_4 + volume_right_1_4) / 14;

			value += volume_dma0 * ((float)(SByte)SoundDMA.soundDMAs[0].outfifo.front() / 64);
			if (SoundDMA.soundDMAs[0].outfifo.size() > 1)
			{
				SoundDMA.soundDMAs[0].outfifo.pop();
			}

			value += volume_dma1 * ((float)(SByte)SoundDMA.soundDMAs[1].outfifo.front() / 64);
			if (SoundDMA.soundDMAs[1].outfifo.size() > 1)
			{
				SoundDMA.soundDMAs[1].outfifo.pop();
			}

			nextSamples.push(value);
		}
		SDL_UnlockMutex(nextSamples_lock);
	}
}

void fill_audio(void* udata, Uint8* stream, int len)
{
	SoundGenerator* soundGenerator = (SoundGenerator*)udata;

	if (SDL_LockMutex(soundGenerator->nextSamples_lock) == 0)
	{
		for (int n = 0; n < len; n += 2)
		{
			if (!soundGenerator->nextSamples.empty())
			{
				float value = soundGenerator->nextSamples.front();
				Int16 valueInt = value * 8192;
				soundGenerator->nextSamples.pop();
				stream[n] = (byte)(valueInt & 0xFF);
				stream[n + 1] = (byte)(valueInt >> 8);
			}
			else
			{
				stream[n] = 0;
				stream[n + 1] = 0;
			}
		}
		SDL_UnlockMutex(soundGenerator->nextSamples_lock);
	}
}