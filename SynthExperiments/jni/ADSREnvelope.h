/*
 * ADSREnvelope.h
 *
 *  Created on: 19 Jun 2014
 *      Author: Kris Popat
 */

#ifndef ADSRENVELOPE_H_
#define ADSRENVELOPE_H_

#include <Envelope.h>

const uint32_t		DEFAULT_ATTACK = 25;
const uint32_t		DEFAULT_DECAY = 2000;
const float			DEFAULT_SUSTAIN = 0.4;
const uint32_t		DEFAULT_RELEASE = 128;

class ADSREnvelope: public Envelope {
public:
				ADSREnvelope( );
				ADSREnvelope( uint32_t attack, uint32_t decay, float sustain, uint32_t release );

	void		generateEnvelopeRamps ( uint32_t attack, uint32_t decay, float sustain, uint32_t release );

	uint32_t	getAttackTime( ) const;
	void		setAttackTime( uint32_t attackTime );
	uint32_t	getDecayTime( ) const;
	void		setDecayTime( uint32_t decayTime );
	uint32_t	getReleaseTime( ) const;
	void		setReleaseTime( uint32_t releaseTime );

private:
	uint32_t	attackTime;
	uint32_t	decayTime;
	uint32_t	releaseTime;
};

#endif /* ADSRENVELOPE_H_ */
