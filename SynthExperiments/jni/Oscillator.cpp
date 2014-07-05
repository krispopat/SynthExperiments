/*
 * Oscillator.cpp
 *
 *  Created on: 4 Jul 2014
 *      Author: Kris Popat
 */

#include <Oscillator.h>
#include <WaveTable.h>
#include <cmath>

Oscillator::Oscillator( WaveTable* table, double sampleRate )
				:SampleGenerator(sampleRate)
{
	waveTable = table;
}

Oscillator::~Oscillator()
{
}


int16_t Oscillator::GetSample( int16_t amplitude, double offset )
{
	double offsetMod = fmod( offset, waveTable->mTableSize );
	double tval;
	if ( ceil ( offsetMod ) >= waveTable->mTableSize ) {
		tval = waveTable->lookup( waveTable->mTableSize - 1 );
	}
	else {
		tval = waveTable->lookup ( offsetMod - 1.0 );
	}
	return amplitude * tval;
}


double Oscillator::CalculateDeltaForFrequency ( double noteFrequency )
{
	double tableRate = sampleRate / waveTable->mTableSize;
	offsetDelta = noteFrequency / tableRate;
	return offsetDelta;
}

