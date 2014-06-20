/*
 * Ramp.cpp
 *
 *  Created on: 18 Jun 2014
 *      Author: Kris Popat
 */

#include <malloc.h>
#include <Ramp.h>
#include <cmath>

const double pi = 3.14159265358979323846;


Ramp::Ramp() {
	rampData = NULL;
	generateRamp ( DEFAULT_RAMP_TYPE, DEFAULT_RAMP_SIZE, DEFAULT_RAMP_TIME, DEFAULT_START_AMP, DEFAULT_END_AMP );
}


Ramp::Ramp( RampType rampType, int32_t dataSize, int32_t rampTime, float startAmp, float endAmp )
{
	rampData = NULL;
	generateRamp ( rampType, dataSize, rampTime, startAmp, endAmp );
}



Ramp::~Ramp() {
	if ( rampData ) {
		free( rampData );
	}
}


float Ramp::lookup ( int32_t millisecondsOffset )
{
	if ( millisecondsOffset > rampTimeMS ) {
		return -1;
	}
	else {
		int lookupDelta = (int)( ( float )rampDataSize / ( float )rampTimeMS ) * millisecondsOffset;
		return rampData[lookupDelta];
	}
}



void Ramp::generateRamp( RampType rampType, int32_t dataSize, int32_t rampTime, float startAmp, float endAmp )
{
	if ( rampData ) free( rampData );
	rampData = (float*)malloc( dataSize * sizeof ( float ) );
	rampDataSize = dataSize;
	rampTimeMS = rampTime;
	bool rising = ( startAmp < endAmp );
	float cf, delta, deltaInc, rotation, amp;
	int i;
	switch( rampType ) {
	case bellCurve:
		if ( rising ) {
			cf = 2 * pi * ( 1 / ( float )( dataSize * 2 ) );
			delta = 1;
			deltaInc = delta * dataSize;
			amp = endAmp - startAmp;
			for ( i = 0; i < dataSize; i++ ) {
				rotation = cf * deltaInc;
				rampData[i] = ( amp / 2 + startAmp ) + ( ( amp / 2 ) * cos ( rotation ) );
				deltaInc -= delta;
			}
		}
		else {
			cf = 2 * pi * ( 1 / ( float )( dataSize * 2 ) );
			delta = 1;
			deltaInc = 0;
			amp = startAmp - endAmp;
			for ( i = 0; i < dataSize; i++ ) {
				rotation = cf * deltaInc;
				rampData[i] = ( amp / 2 + endAmp ) + ( ( amp / 2 ) * cos ( rotation ) );
				deltaInc += delta;
			}
		}
		break;

	case slope:
		if ( rising ) {
			amp = endAmp - startAmp;
			delta = amp/(float)dataSize;
			deltaInc = 0;

			for ( int32_t i = 0; i < dataSize; i++ ) {
				rampData[i] = deltaInc + startAmp;
				deltaInc += delta;
			}
		}
		else {
			amp = startAmp - endAmp;
			delta = 1.0/(float)dataSize;
			deltaInc = 1.0;
			for ( int32_t i = 0; i < dataSize; i++ ) {
				rampData[i] = deltaInc + endAmp;
				deltaInc -= delta;
			}
		}
		break;


	case empty:
	default:
		// fill with zeros
		for ( int32_t i = 0; i < dataSize; i++ ) {
			rampData[i] = 0.0;
		}
		break;
	}
}



float* Ramp::getRampData() const
{
	return rampData;
}

int32_t Ramp::getRampDataSize() const
{
	return rampDataSize;
}


int32_t Ramp::getRampTimeMs() const
{
	return rampTimeMS;
}


void Ramp::setRampTimeMs(int32_t rampTimeMs)
{
	rampTimeMS = rampTimeMs;
}
