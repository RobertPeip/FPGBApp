#include "types.h"

class SoundChannel
{
public:
	byte Number_of_sweep_shift;
	bool Sweep_Frequency_Direction;
	byte Sweep_Time;

	byte Sound_length;
	byte Wave_Pattern_Duty;
	byte Envelope_Step_Time;
	bool Envelope_Direction;
	byte Initial_Volume_of_envelope;

	Int32 Frequency;
	bool Length_Flag;
	bool Initial;

	bool waveenable;
	bool wavebanks;
	bool banknumber;

	bool lfsr7bit;

	float volume;
	bool on;

	SoundChannel();
	void assign(int id);
	void setNewSweep();
	void setNewLength();
	void setNewEnvelope();
	void setNewFreq();
	void setWaveTable();
	void setNewPoly(byte divider_raw, byte shift_raw);
	void setNewInitial();

	float get_next();

private:
	int id;

	byte wavetable[32];
	byte wavetable2[32];
	int wavetable_Length;

	int wavetable_ptr;

	// channel1-4
	UInt32 freq_cnt;
	UInt32 length_left;

	//channel1
	UInt32 sweepcnt;

	// channel1,2,4
	UInt32 envelope_cnt;
	Int32 envelope_add;

	// channel 3

	// channel 4
	UInt32 lfsr;
	UInt32 ch4_freqdivider;

	UInt32 soundcycles_freq;
	UInt32 soundcycles_sweep;
	UInt32 soundcycles_envelope;
	UInt32 soundcycles_length;

	void recalc_freq();
	void update_timebased(UInt32 new_cycles);
};