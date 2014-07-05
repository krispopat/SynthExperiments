/*
 * MIDINote.cpp
 *
 *  Created on: 18 Jun 2014
 *      Author: Kris Popat
 */

#include <Envelope.h>
//#include <malloc.h>
#include <MIDINote.h>
#include <Ramp.h>
#include <stddef.h>
#include <stl/_cmath.h>
#include <stl/_vector.h>
#include <cmath>

MIDINote::MIDINote( Envelope* inEnvelope, double inEnvelopePhaseDelta )
{
	resetNoteData();
	envelope = inEnvelope;
	envelopePhaseDelta = inEnvelopePhaseDelta;
}

MIDINote::MIDINote( uint8_t* msg, int msgLength, Envelope* inEnvelope, double inEnvelopePhaseDelta )
{
	resetNoteData();
	envelope = inEnvelope;
	envelopePhaseDelta = inEnvelopePhaseDelta;
	updateFromMIDIMessage(msg, msgLength);
}

MIDINote::~MIDINote()
{
}


void MIDINote::resetNoteData( )
{
	pitch = 0x0;
	channel = 0x0;
	velocity = 0x0;
	detune = 0;
	keydown = false;
	active = false;
	frequency = 0;
	generatorPhase = 0;
	envelopePhase = 0;
	envelopePhasePosition = 0;
	envelopeFactor = 0;
}


bool MIDINote::updateFromMIDIMessage(uint8_t* msg, int msgLength)
{
	uint8_t channelMessage = msg[0] & 0xF0;
	if ( channelMessage == 0x80 ) {
		this->setKeydown(false);
		envelopePhase = RELEASE_PHASE;
		envelopePhasePosition = 0;
		return true;
	}
	else if ( channelMessage == 0x90 ) {
		pitch = msg[1];
		channel = channelMessage & 0x0F;
		velocity = msg[2];
		envelopePhase = 0;
		frequency = computeFrequency();
		generatorPhase = 0;
		envelopePhasePosition = 0;
		envelopeFactor = 0;
		active = true;
		if ( velocity == 0 ) {
			this->setKeydown(false);
			envelopePhase = RELEASE_PHASE;
		}
		else {
			setKeydown(true);
		}
		return true;
	}
	return false;
}



double MIDINote::computeFrequency()
{
	double exponent = ( (double)pitch - 69.0 ) / 12.0;
	return pow( 2.0, exponent ) * 440.0;
}

static double log2(double d) {
	const double kLog2Reciprocal = 1.442695040888963;
	return log(d) * kLog2Reciprocal;
}


void MIDINote::setNoteFromFrequency( double frequency )
{
	double n = frequency / 440.0;
	pitch = (uint8_t)round( 69 + 12 * log2( n ) );
}


double MIDINote::calculateEnvelopeFactorPerSample ( )
{
	double factor;
	if ( envelopePhase == 0 && keydown == false ) {
		// note not active
		return 0;
	}
	if ( envelopePhase == SUSTAIN_PHASE ) {
		factor = envelope->sustain;
	}
	else {
		// We are not in sustain phase on the envelope so
		// get the current ramp
		Ramp* ramp = NULL;
		 if ( envelopePhase == RELEASE_PHASE ) {
			 ramp = envelope->releaseRamp;
			 factor = ramp->calculatedLookup(envelopeFactor,0, floor ( envelopePhasePosition) );
		 }
		 else {
			 // We are assuming that note->envelopePhase is valid
			 // if not this would be a programming error
			ramp = envelope->ramps.at(envelopePhase);
			factor = ramp->lookup( floor( envelopePhasePosition ) );
			if ( envelopeFactor != -1 ) {
				envelopeFactor = factor;
			}
		 }
		 // calculate the offset in milliseconds
		 // uses the stored phase position in the note
		 if ( factor == -1 ) {
			 if ( envelopePhase == RELEASE_PHASE ) {
				 // this note has finished, break to outer loop
				 resetNoteData( );
				 factor = 0;
			 }
			 else {
				 // we've run out of lookup on that ramp, so move to next one
				 envelopePhase++;
				 envelopePhasePosition = envelopePhasePosition - ramp->rampTimeMS;
				 // if we've run out of ramps at the onset of the note then go to sustain phase
				 if ( envelopePhase >= envelope->ramps.size() ) {
					 envelopePhase = SUSTAIN_PHASE;
					 factor = envelope->sustain;
					 envelopeFactor = factor;
				 }
				 else {
					 ramp = envelope->ramps.at( envelopePhase );
					 factor = ramp->lookup ( floor( envelopePhasePosition ) );
					 if ( factor != -1 ) {
						 envelopeFactor = factor;
					 }
				 }
			 }
		 }
		 envelopePhasePosition += envelopePhaseDelta;
	}
	return factor;
}



bool MIDINote::isActive() const
{
	return active;
}



void MIDINote::setActive(bool active)
{
	this->active = active;
}



bool MIDINote::isKeydown() const
{
	return keydown;
}



void MIDINote::setKeydown(bool keydown)
{
	this->keydown = keydown;
}



uint8_t MIDINote::getChannel() const
{
	return channel;
}



float MIDINote::getDetune() const
{
	return detune;
}



void MIDINote::setDetune(float detune)
{
	this->detune = detune;
}



uint8_t MIDINote::getPitch() const
{
	return pitch;
}



uint8_t MIDINote::getVelocity() const {
	return velocity;
}
