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


Synthesizer::Synthesizer( int inBufferSize, int inSampleRate ):SoundDistributor(inBufferSize, inSampleRate )
{
	for ( int i = 0; i < MAX_POLYPHONY; i++ ) {
		notes[i] = new MIDINote(  );
	}
	envelope = new ADSREnvelope();
	sineOscillator = new WaveTable ( sine, 2048, 1 ); // just one cycle
	sineOscillator->generateWave();
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
	bool firstIteration = true;
	bool noNotes = true;
	for ( i = 0; i < MAX_POLYPHONY; i++ ) {
		// for each note that is active
		if ( notes[i]->isActive( ) ) {
			noNotes = false;
			MIDINote* note = notes[i];

			// get frequency from note
			// calculate delta for oscillator lookup

			double oscillatorFrequency = sampleRate / ( sineOscillator->mTableSize );
			double oscillatorDelta = note->frequency / oscillatorFrequency;
			double envelopePhaseDelta = (1.0/sampleRate)*1000.0;

			double volume = note->getVelocity()/(double)127.0;

			for ( j = 0; j < audioSampleCount; j++ ) {
				if ( firstIteration ) {
					// clean buffer first time round;
					buffer[j] = 0;
				}
				double envelopeFactor;
				// if envelope phase is SUSTAIN_PHASE then get sustain factor value
				// else
				// get current envelope phase position from the note
				// time per sample is 1/44100 seconds
				// lookup envelope
				if ( note->envelopePhase == SUSTAIN_PHASE ) {
					envelopeFactor = envelope->sustain;
				}
				else {
					// We are not in sustain phase on the envelope so
					// get the current ramp
					Ramp* ramp = NULL;
					 if ( note->envelopePhase == RELEASE_PHASE ) {
						 ramp = envelope->releaseRamp;
					 }
					 else {
						 // We are assuming that note->envelopePhase is valid
						 // if not this would be a programming error
						ramp = envelope->ramps.at(note->envelopePhase);
					 }
					 // calculate the offset in milliseconds
					 // uses the stored phase position in the note
					 envelopeFactor = ramp->lookup( floor( note->envelopePhasePosition ) );
					 if ( envelopeFactor == -1 ) {
						 if ( note->envelopePhase == RELEASE_PHASE ) {
							 // this note has finished, break to outer loop
							 note->resetNoteData( );
							 break;
						 }
						 // we've run out of lookup on that ramp, so move to next one
						 note->envelopePhase++;
						 note->envelopePhasePosition = note->envelopePhasePosition - ramp->rampTimeMS;
						 // if we've run out of ramps at the onset of the note then go to sustain phase
						 if ( note->envelopePhase >= envelope->ramps.size() ) {
							 note->envelopePhase = SUSTAIN_PHASE;
							 envelopeFactor = envelope->sustain;
						 }
						 else {
							 ramp = envelope->ramps.at( note->envelopePhase );
							 envelopeFactor = ramp->lookup ( floor( note->envelopePhasePosition ) );
						 }
					 }
					 note->envelopePhasePosition += envelopePhaseDelta;
				}
				// get a sample from the oscillator
				double currentOscillatorPhase = note->oscilatorPhase;
				int16_t oscAmp = sineOscillator->lookup( round( currentOscillatorPhase ) );
				int16_t scaledAmp = (int16_t)( ((double)oscAmp * volume) * envelopeFactor );
				buffer[j] += scaledAmp;
				currentOscillatorPhase += oscillatorDelta;
				if ( currentOscillatorPhase > sineOscillator->mTableSize ) {
					currentOscillatorPhase = currentOscillatorPhase - sineOscillator->mTableSize;
				}
				note->oscilatorPhase = currentOscillatorPhase;

			}
			firstIteration = false; // don't clean buffer each time!
		}
	}
	if ( noNotes == true ) {
		//clear the array - not sure memset is thread safe
		for ( j = 0; j < audioSampleCount; j++ ) {
			buffer[j] = 0;
		}
	}
}


void Synthesizer::MIDIMessage( uint8_t* msg, int messageLength )
{
	uint8_t message = msg[0] & 0xF0;
	if ( message == 0x80 || ( message == 0x90 && msg[2] == 0 ) ) {
		// note off
		// search for the equivalent note on
		MIDINote* n = NULL;
		for ( int i = 0; i < MAX_POLYPHONY; i++ ) {
			if ( notes[i]->getPitch() == msg[1]) {
				n = notes[i];
				break;
			}
		}
		if ( n != NULL ) {
			n->updateFromMIDIMessage( msg, messageLength );
		}
	}
	else if ( message == 0x90 ) {
		// find the next slot for a new note
		notes[noteIndex++]->updateFromMIDIMessage(msg, messageLength );
		if ( noteIndex == MAX_POLYPHONY ) {
			noteIndex = 0; // rotate
		}
	}
}
