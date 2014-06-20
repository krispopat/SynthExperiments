/*
 * Envelope.h
 *
 *  Created on: 18 Jun 2014
 *      Author: Kris Popat
 */

#ifndef ENVELOPE_H_
#define ENVELOPE_H_

#include <Ramp.h>
#include <cstdint>
#include <vector>

const uint8_t RELEASE_PHASE = 0xFF;
const uint8_t SUSTAIN_PHASE = 0xEF;



class Envelope {
public:
				Envelope();
				Envelope( float sustainLevel );
	virtual		~Envelope( );

	Ramp*		createRamp ( int32_t rampSize, int32_t rampLength, RampType rampType, float startAmp, float endAmp );

	Ramp*		createReleaseRamp ( int32_t rampSize, int32_t rampLength,RampType rampType, float startAmp, float endAmp=0 );

	std::vector<Ramp*>		ramps;

	float		sustain;

	Ramp*		releaseRamp;

};

#endif /* ENVELOPE_H_ */
