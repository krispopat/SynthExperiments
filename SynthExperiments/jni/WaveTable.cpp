/*
 * WaveTable.cpp
 *
 *  Created on: 16 Jun 2014
 *      Author: Kris Popat
 */

#include <WaveTable.h>
#include <cstdint>
#include <cstdlib>
#include <cmath>


WaveTable::WaveTable(TableType tableType,  int tableSize, float frequency) {
	mTableType = tableType;

	mTableSize = tableSize;
	mFrequency = frequency;
	if ( tableType != square ) {
		mTable = (int16_t*)malloc(mTableSize * sizeof(int16_t));
	}
	else {
		mTable = NULL;
	}
}

WaveTable::~WaveTable() {
	if ( mTable != NULL) free(mTable);
}


void WaveTable::generateWave()
{
	switch ( mTableType ) {
	case sine:
		generateSineWave();
		break;

	case triangle:
		generateTriangleWave();
		break;

	case saw:
		generateSawWave();
		break;

	case square:
		//no need to fill this in we can do it in lookup no probs.
		break;
	}
}

int16_t WaveTable::lookup(int32_t phase )
{
	return mTable[phase];
}


// privates!

void WaveTable::generateSineWave()
{
	double circularFrequency = 2 * pi * ( mFrequency / mTableSize );
	double delta = ( double )1 / ( double )mFrequency;
	double deltaInc = 0;
	int i;
	for( i = 0; i < mTableSize; i++ ) {
		double sineRotation = circularFrequency * ( deltaInc );
		mTable[i] = round( amplitude * sin( sineRotation ) );
		deltaInc += delta;
	}
}


// simplest way possible - divide the buffer into ramp segments
void WaveTable::generateTriangleWave()
{
	float delta = ( float )amplitude / ( ( ( float )mTableSize ) / 4 );
	float increment = 0;
	int i;
	for ( i = 0; i < ( mTableSize / 4 ); i++ ) {
		mTable[i] = floor( increment );
		increment += delta;
	}
	for ( ; i < ( mTableSize / 4 ) *3 ; i++ ) {
		mTable[i] = floor( increment );
		increment -= delta;
	}
	for ( ; i < mTableSize; i++ ) {
		mTable[i] = floor( increment );
		increment += delta;
	}
}


// again simple way divide teh buffer into two ascending ramps
void WaveTable::generateSawWave()
{
	float delta = ( float )amplitude / ( ( ( float )mTableSize ) / 2 );
	float increment = 0;
	int i;
	for ( i = 0; i < ( mTableSize / 2 ); i++ ) {
		mTable[i] = floor(increment);
		increment += delta;
	}
	increment = -1 * amplitude;
	for ( ; i < mTableSize; i++ ) {
		mTable[i] = floor(increment);
		increment += delta;
	}
}



