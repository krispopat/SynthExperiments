/*
 * Synthesizer.cpp
 *
 *  Created on: 12 Jun 2014
 *      Author: Kris Popat
 */

#include <ADSREnvelope.h>
#include <CyclicBuffer.h>
#include <MIDINote.h>
#include <Oscillator.h>
#include <stddef.h>
//#include <SampleGenerator.h>
#include <Synthesizer.h>
#include <WaveTable.h>
//#include <cmath>


Synthesizer::Synthesizer( int inBufferSize, int inSampleRate, int16_t overallAmplitude ):SoundDistributor(inBufferSize, inSampleRate )
{
	envelope = new ADSREnvelope();
	double envelopePhaseDelta = (1.0/sampleRate)*1000.0;

	for ( int i = 0; i < MAX_POLYPHONY; i++ ) {
		notes[i] = new MIDINote( envelope, envelopePhaseDelta );
	}
	sineTable = new WaveTable ( sine, 2048, 1 ); // just one cycle
	sineTable->generateWave();
	generator = new Oscillator ( sineTable, sampleRate );
	inputStream = new CyclicBuffer();
	noteIndex = 0;
	volume = overallAmplitude;
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
	if ( generator != NULL ) {
		delete generator;
	}
	if ( sineTable != NULL ) {
		delete sineTable;
	}
	if ( inputStream != NULL ) {
		delete inputStream;
	}
}



void Synthesizer::GetAudioSamples( int audioSampleCount, int16_t *buffer )
{
	int i,j;
	bool firstIteration = true;
	bool noNotes = true;

	CheckInputStream();

	for ( i = 0; i < MAX_POLYPHONY; i++ ) {
		// for each note that is active
		if ( notes[i]->isActive( ) ) {
			noNotes = false;
			MIDINote* note = notes[i];

			// get frequency from note
			// calculate delta for oscillator lookup
			double delta = generator->CalculateDeltaForFrequency( note->frequency );

			double noteVelocity = note->getVelocity()/(double)127.0;

			for ( j = 0; j < audioSampleCount; j++ ) {
				if ( firstIteration ) {
					// clean buffer first time round;
					buffer[j] = 0;
				}
				double envelopeFactor = note->calculateEnvelopeFactorPerSample();

				// calculate the current amplitude
				int16_t amplitude = ( int16_t )( volume * envelopeFactor ) * noteVelocity;
				int16_t sample = generator->GetSample( amplitude, note->generatorPhase );

				int32_t accumulator = buffer[j] + sample;
				accumulator = ( accumulator > 0x8000 ) ? 0x8000 : ( accumulator < -0x7FFF ) ? 0-0x7FFF : accumulator;

				buffer[j] = accumulator;

				note->generatorPhase += delta;

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
	inputStream->Write( messageLength,msg);
}


void Synthesizer::CheckInputStream ( )
{
	if ( inputStream->NumberOfBytesToRead() > 2 ) { // this is bad, needs writing properly
		uint8_t msg[3];
		inputStream->Read(3 ,msg );
		uint8_t message = msg[0] & 0xF0;
		if ( message == 0x80 || ( message == 0x90 && msg[2] == 0 ) ) {
			// note off
			// search for the equivalent note on
			MIDINote* n = NULL;
			for ( int i = 0; i < MAX_POLYPHONY; i++ ) {
				if ( notes[i]->getPitch() == msg[1] && notes[i]->isKeydown()) {
					n = notes[i];
					break;
				}
			}
			if ( n != NULL ) {
				n->updateFromMIDIMessage( msg, 3 );
			}
		}
		else if ( message == 0x90 ) {
			// find the next slot for a new note
			// find free note
			for ( int i = 0; i < MAX_POLYPHONY; i++ ) {
				MIDINote* n = notes[i];
				if ( n->getPitch() == msg[1] ) {
					if ( n->isKeydown() ) {
						// oops we've got an extra noteon
						break;
					}
				}
				else if ( n->getPitch() == 0){
					n->updateFromMIDIMessage(msg,3);
					break;
				}
			}
			//notes[noteIndex++]->updateFromMIDIMessage(msg, 3 );
			//if ( noteIndex == MAX_POLYPHONY ) {
			//	noteIndex = 0; // rotate
			//}
		}
	}
}
