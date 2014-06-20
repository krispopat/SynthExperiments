/*
 * SoundDistributor.h
 *
 *  Created on: 12 Jun 2014
 *      Author: Kris Popat
 */

#ifndef SOUNDDISTRIBUTOR_H_
#define SOUNDDISTRIBUTOR_H_

#include <stdio.h>

class SoundDistributor {

public:
	virtual ~SoundDistributor(){}
	virtual void GetAudioSamples( int audioSampleCount, int16_t *buffer )=0;

};


#endif /* SOUNDDISTRIBUTOR_H_ */
