/*
 * Synthesizer.h
 *
 *  Created on: 12 Jun 2014
 *      Author: Kris Popat
 */

#ifndef SYNTHESIZER_H_
#define SYNTHESIZER_H_

#include <SoundDistributor.h>
#include <cstdint>

class CyclicBuffer;

const int MAX_POLYPHONY = 16;

class ADSREnvelope;
class MIDINote;
class WaveTable;

class Synthesizer: public SoundDistributor {
public:
	Synthesizer( int inBufferSize, int inSampleRate );
	virtual ~Synthesizer();

	void GetAudioSamples( int audioSampleCount, int16_t *buffer );

	void MIDIMessage ( uint8_t* msg, int messageLength );

private:
	void CheckInputStream ( );

	int noteIndex; // to rotate through notes
	MIDINote* notes[MAX_POLYPHONY]; // 16 note polyphony for now
	ADSREnvelope* envelope; // single envelope
	WaveTable* sineOscillator; // single oscilator for now
	CyclicBuffer* inputStream;
	double oscillatorFrequency;

};

#endif /* SYNTHESIZER_H_ */
