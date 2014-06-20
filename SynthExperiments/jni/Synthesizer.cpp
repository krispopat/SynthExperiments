/*
 * Synthesizer.cpp
 *
 *  Created on: 12 Jun 2014
 *      Author: Kris Popat
 */

#include <ADSREnvelope.h>
#include <MIDINote.h>
#include <stddef.h>
#include <Synthesizer.h>
#include <WaveTable.h>


Synthesizer::Synthesizer( int bufferSize ) {

	for ( int i = 0; i < MAX_POLYPHONY; i++ ) {
		notes[i] = new MIDINote( bufferSize );
	}
	envelope = new ADSREnvelope();
	sineOscillator = new WaveTable ( sine, 1024, 1 ); // just one cycle
	noteIndex = 0;
}



Synthesizer::~Synthesizer() {
	for ( int i = 0; i < MAX_POLYPHONY; i++ ) {
		if ( notes[i] != NULL ) {
			delete notes[i];
		}
	}
	if ( envelope != NULL ) {
		delete envelope;
	}
	if ( sineOscillator != NULL ) {
		delete sineOscillator;
	}
}


void Synthesizer::GetAudioSamples( int audioSampleCount, int16_t *buffer )
{
	int i,j;
	for ( i = 0; i < MAX_POLYPHONY; i++ ) {
		// for each note that is active
		if ( notes[i]->isActive() ) {
			MIDINote* note = notes[i];
			float envelopeFactor = 0;
			// get the current envelope phase from the note
			if ( note->envelopePhase == SUSTAIN_PHASE ) {
				envelopeFactor = envelope->sustain;
			}
			// if envelope phase is SUSTAIN_PHASE then get sustain factor value
			// else
			// get current envelope phase position from the note
			// time per sample is 1/44100 seconds
			// lookup envelope

			// get frequency from note
			// calculate delta for oscillator lookup
			// get last oscillator delta position (oscillatorPhase) used
			// add delta to delta position for first lookup
			// rotate if required
			for ( j = 0; j < audioSampleCount; j++ ) {


			}
		}
	}
}


void Synthesizer::MIDIMessage( uint8_t* msg, int messageLength )
{
	uint8_t message = msg[0] & 0xF0;
	if ( message == 0x80 || message == 0x90 ) {
		notes[noteIndex++]->updateFromMIDIMessage(msg, messageLength );
		if ( noteIndex == MAX_POLYPHONY ) {
			noteIndex = 0; // rotate
		}
	}
}
