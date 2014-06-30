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

	int lookupDelta = calculateDelta ( millisecondsOffset );
	if ( lookupDelta >= rampDataSize ) {
		return -1;
	}
	else {
		return rampData[lookupDelta];
	}
}



float Ramp::calculatedLookup ( float startAmp, float endAmp, int32_t offset )
{
	int offsetIndex = calculateDelta ( offset );
	if ( offsetIndex >= rampDataSize ) {
		return -1;
	}
	else {
		bool rising = ( startAmp < endAmp );
		float cf, delta, inc, rot, amp;
		amp = ( rising ) ? endAmp - startAmp : startAmp - endAmp;
		switch ( rampType ) {
		case bellCurve:
			cf = 2 * pi * ( 1 / (float ) ( rampDataSize * 2 ) );
			delta = 1;
			rot = cf * offsetIndex;
			if ( rising ) {
				return  ( amp / 2 + startAmp ) + ( ( amp /2 ) * cos ( rot ) );
			}
			else {
				return ( amp / 2 + endAmp ) + ( ( amp / 2 ) * cos ( rot ) );
			}
			break;

		case slope:
			delta = amp/(float)rampDataSize;
			if ( rising ) {
				return startAmp + ( offsetIndex * delta );
			}
			else {
				return startAmp - ( offsetIndex * delta );
			}
		}
	}
}


void Ramp::regenerateRamp ( float startAmp, float endAmp )
{
	bool rising = ( startAmp < endAmp );
	float cf, delta, deltaInc, rotation, amp;
	int i;
	switch( rampType ) {
	case bellCurve:
		if ( rising ) {
			cf = 2 * pi * ( 1 / ( float )( rampDataSize * 2 ) );
			delta = 1;
			deltaInc = delta * rampDataSize;
			amp = endAmp - startAmp;
			for ( i = 0; i < rampDataSize; i++ ) {
				rotation = cf * deltaInc;
				rampData[i] = ( amp / 2 + startAmp ) + ( ( amp / 2 ) * cos ( rotation ) );
				deltaInc -= delta;
			}
		}
		else {
			cf = 2 * pi * ( 1 / ( float )( rampDataSize * 2 ) );
			delta = 1;
			deltaInc = 0;
			amp = startAmp - endAmp;
			for ( i = 0; i < rampDataSize; i++ ) {
				rotation = cf * deltaInc;
				rampData[i] = ( amp / 2 + endAmp ) + ( ( amp / 2 ) * cos ( rotation ) );
				deltaInc += delta;
			}
		}
		break;

	case slope:
		if ( rising ) {
			amp = endAmp - startAmp;
			delta = amp/(float)rampDataSize;
			deltaInc = 0;

			for ( int32_t i = 0; i < rampDataSize; i++ ) {
				rampData[i] = deltaInc + startAmp;
				deltaInc += delta;
			}
		}
		else {
			amp = startAmp - endAmp;
			delta = amp/(float)rampDataSize;
			for ( int32_t i = 0; i < rampDataSize; i++ ) {
				rampData[i] = startAmp;
				startAmp -= delta;
			}
		}
		break;


	case empty:
	default:
		// fill with zeros
		for ( int32_t i = 0; i < rampDataSize; i++ ) {
			rampData[i] = 0.0;
		}
		break;
	}
}




void Ramp::generateRamp( RampType rt, int32_t dataSize, int32_t rampTime, float startAmp, float endAmp )
{
	if ( rampData ) free( rampData );
	rampData = (float*)malloc( dataSize * sizeof ( float ) );
	rampDataSize = dataSize;
	rampTimeMS = rampTime;
	rampType = rt;
	regenerateRamp ( startAmp, endAmp );
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


// privates

int32_t Ramp::calculateDelta ( int32_t offset )
{
	return (int32_t)( ( float )rampDataSize / ( float )rampTimeMS ) * offset;
}

