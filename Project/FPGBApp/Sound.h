#include "types.h"
#include "Soundgenerator.h"

class SOUND
{
public:
	const int SAMPLERATE = 16780000 / 44100;
	SoundGenerator* soundGenerator;

	void reset();
	void set_soundreg(UInt32 adr);
	void work();

private:
	bool sound_on;

};
extern SOUND Sound;