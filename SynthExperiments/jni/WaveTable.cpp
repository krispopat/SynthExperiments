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


WaveTable::WaveTable(TableType tableType,  int tableSize, float frequency )
{
	mTableType = tableType;

	mTableSize = tableSize;
	mFrequency = frequency;
	if ( tableType != square ) {
		mTable = (double*)malloc(mTableSize * sizeof(double));
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


// floating point interpolated array lookup
double WaveTable::lookup( double phase )
{
	double intPart;
	double fracPart = modf( phase, &intPart );
	double v1 = mTable[ (int)intPart ];
	double v2 = mTable[ (int)intPart + 1 ];
	return v1 + ( (v2 - v1 ) * fracPart );
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
		mTable[i] = sin( sineRotation );
		deltaInc += delta;
	}
}


// simplest way possible - divide the buffer into ramp segments
void WaveTable::generateTriangleWave()
{
	double delta = 1.0 / ( ( ( double )mTableSize ) / 4 );
	double increment = 0;
	int i;
	for ( i = 0; i < ( mTableSize / 4 ); i++ ) {
		mTable[i] = increment;
		increment += delta;
	}
	for ( ; i < ( mTableSize / 4 ) *3 ; i++ ) {
		mTable[i] = increment;
		increment -= delta;
	}
	for ( ; i < mTableSize; i++ ) {
		mTable[i] = increment;
		increment += delta;
	}
}


// again simple way divide the buffer into two ascending ramps
void WaveTable::generateSawWave()
{
	float delta = 1.0 / ( ( ( double )mTableSize ) / 2 );
	double increment = 0;
	int i;
	for ( i = 0; i < ( mTableSize / 2 ); i++ ) {
		mTable[i] = increment;
		increment += delta;
	}
	increment = -1.0;
	for ( ; i < mTableSize; i++ ) {
		mTable[i] = increment;
		increment += delta;
	}
}



