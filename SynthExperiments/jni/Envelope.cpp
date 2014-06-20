/*
 * Envelope.cpp
 *
 *  Created on: 18 Jun 2014
 *      Author: Kris Popat
 */

#include <Envelope.h>
#include <stddef.h>
#include <stl/_vector.h>


Envelope::Envelope()
{
	sustain = 0.8;
	releaseRamp = NULL;
}

Envelope::Envelope( float sustainLevel )
{
	sustain = sustainLevel;
	releaseRamp = NULL;
}



Envelope::~Envelope() {
	int c = ramps.size();
	for ( int i = 0; i < c; i++ ) {
		Ramp* ramp = ramps.at(i);
		delete ramp;
	}
	if ( releaseRamp ) {
		delete releaseRamp;
	}
}




Ramp* Envelope::createRamp(
		int32_t rampSize,
		int32_t rampLength,
		RampType rampType,
		float startAmp,
		float endAmp )
{
	Ramp* ramp = new Ramp(rampType, rampSize, rampLength, startAmp, endAmp );
	ramps.push_back( ramp );
	return ramp;
}



Ramp* Envelope::createReleaseRamp(int32_t rampSize,
		int32_t rampLength,
		RampType rampType,
		float startAmp,
		float endAmp)
{
	releaseRamp = new Ramp ( rampType, rampSize, rampLength, startAmp, endAmp );
	return releaseRamp;
}
