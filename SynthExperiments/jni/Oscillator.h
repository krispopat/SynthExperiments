/*
 * Oscillator.h
 *
 *  Created on: 4 Jul 2014
 *      Author: Kris Popat
 */

#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

#include <SampleGenerator.h>
#include <cstdint>

class WaveTable;

class Oscillator: public SampleGenerator {
public:
				Oscillator( WaveTable* table, double sampleRate );
	virtual		~Oscillator();

	int16_t	GetSample ( int16_t amplitude, double offset );
	double		CalculateDeltaForFrequency ( double frequency );

private:
	WaveTable* waveTable;
};

#endif /* OSCILLATOR_H_ */
