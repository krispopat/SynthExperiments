/*
 * WaveTable.h
 *
 *  Created on: 16 Jun 2014
 *      Author: kris
 */

#ifndef WAVETABLE_H_
#define WAVETABLE_H_

#include <cstdint>


enum TableType {
	sine,
	square,
	saw,
	triangle
};

const double pi = 3.14159265358979323846;
//const int16_t amplitude = 0x4000; // 1<<15

class WaveTable {
public:
				WaveTable(TableType tableType,  int tableSize, float frequency );
	virtual		~WaveTable();

	void 		generateWave();

	double		lookup( int32_t phase ) { return mTable[phase];}

	double		lookup( double phase ); // interpolated

	double*		getTable() { return mTable; }

	int			mTableSize;
	float		mFrequency;

private:

	void 		generateSineWave();
	void 		generateSawWave();
	void 		generateTriangleWave();

	TableType	mTableType;


	double		*mTable;
};

#endif /* WAVETABLE_H_ */
