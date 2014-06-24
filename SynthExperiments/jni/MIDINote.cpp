/*
 * MIDINote.cpp
 *
 *  Created on: 18 Jun 2014
 *      Author: Kris Popat
 */

#include <Envelope.h>
#include <malloc.h>
#include <MIDINote.h>
#include <cmath>

MIDINote::MIDINote( ) {
	pitch = 0x0;
	channel = 0x0;
	velocity = 0x0;
	detune = 0;
	keydown = false;
	active = false;
	frequency = 0;
	oscilatorPhase = 0;
	envelopePhase = 0;
	envelopePhasePosition = 0;

}

MIDINote::MIDINote( uint8_t* msg, int msgLength)
{
	updateFromMIDIMessage(msg, msgLength);
}

MIDINote::~MIDINote()
{
}


bool MIDINote::updateFromMIDIMessage(uint8_t* msg, int msgLength)
{
	uint8_t channelMessage = msg[0];
	if ( channelMessage & 0xf0 == 0x80 ) {
		velocity = 0;
		this->setKeydown(false);
		envelopePhase = RELEASE_PHASE;
		envelopePhasePosition = 0;
		return true;
	}
	else if ( channelMessage & 0xf0 == 0x90 ) {
		pitch = msg[1];
		channel = channelMessage & 0x0F;
		velocity = msg[2];
		envelopePhase = 0;
		frequency = computeFrequency();
		oscilatorPhase = 0;
		envelopePhasePosition = 0;
		active = true;
		if ( velocity == 0 ) {
			this->setKeydown(false);
			envelopePhase = RELEASE_PHASE;
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
