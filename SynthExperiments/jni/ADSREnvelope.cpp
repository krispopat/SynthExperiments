/*
 * ADSREnvelope.cpp
 *
 *  Created on: 19 Jun 2014
 *      Author: Kris Popat
 */

#include <ADSREnvelope.h>

ADSREnvelope::ADSREnvelope( )
{
	generateEnvelopeRamps ( DEFAULT_ATTACK, DEFAULT_DECAY, DEFAULT_SUSTAIN, DEFAULT_RELEASE );
}


ADSREnvelope::ADSREnvelope( uint32_t attack, uint32_t decay, float sustain, uint32_t release )
{
	generateEnvelopeRamps( attack, decay, sustain, release );
}



void ADSREnvelope::generateEnvelopeRamps(uint32_t attack, uint32_t decay,
		float sustain, uint32_t release)
{
	this->createRamp( attack, attack, slope, 0.0, 1.0 );
	this->createRamp( decay, decay, slope, 1.0, sustain );
	this->sustain = sustain;
	this->createReleaseRamp ( release, release, slope, sustain );
}


uint32_t ADSREnvelope::getAttackTime() const
{
	return attackTime;
}


void ADSREnvelope::setAttackTime(uint32_t attackTime)
{
	this->attackTime = attackTime;
	Ramp* attackRamp = ramps.at(0);
	attackRamp->generateRamp( slope, attackTime, attackTime, 0, 1 );
}


uint32_t ADSREnvelope::getDecayTime() const
{
	return decayTime;
}


void ADSREnvelope::setDecayTime(uint32_t decayTime)
{
	this->decayTime = decayTime;
	Ramp* decayRamp = ramps.at(1);
	decayRamp->generateRamp( slope, decayTime, decayTime, 1, this->sustain );
}


uint32_t ADSREnvelope::getReleaseTime() const
{
	return releaseTime;
}

void ADSREnvelope::setReleaseTime(uint32_t releaseTime)
{
	this->releaseTime = releaseTime;
	this->releaseRamp->generateRamp( slope, releaseTime, releaseTime, this->sustain, 0 );
}
