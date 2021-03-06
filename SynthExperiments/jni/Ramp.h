/*
 * Ramp.h
 *
 *  Created on: 18 Jun 2014
 *      Author: kris
 */

#ifndef RAMP_H_
#define RAMP_H_

#include <cstdint>


enum RampType {
	empty,
	bellCurve,
	slope
};


const int32_t	DEFAULT_RAMP_SIZE		= 1024;
const int32_t	DEFAULT_RAMP_TIME		= 125; //milliseconds
const RampType	DEFAULT_RAMP_TYPE		= slope;
const float		DEFAULT_START_AMP		= 0;
const float		DEFAULT_END_AMP			= 1;


class Ramp {
public:
				Ramp( );
				Ramp( RampType rampType, int32_t dataSize, int32_t rampTime, float startAmp, float endAmp );
	virtual		~Ramp( );

	void		generateRamp( RampType rampType, int32_t dataSize, int32_t rampTime, float startAmp, float endAmp );

	void		regenerateRamp ( float startAmp, float endAmp ); // for on the fly changes

	float		calculatedLookup ( float startAmp, float endAmp, int32_t offset );

	float		lookup ( int32_t milliSecondsOffset );

	float*		getRampData( ) const;
	int32_t		getRampDataSize( ) const;
	int32_t		getRampTimeMs( ) const;
	void		setRampTimeMs( int32_t rampTimeMs );

	int32_t			rampTimeMS;

private:

	int32_t		calculateDelta( int32_t offset );

	float*  		rampData;
	RampType		rampType;
	int32_t			rampDataSize;
};

#endif /* RAMP_H_ */
