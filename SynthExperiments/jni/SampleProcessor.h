/*
 * SampleProcessor.h
 *
 *  Created on: 14 Jul 2014
 *      Author: kris
 */

#ifndef SAMPLEPROCESSOR_H_
#define SAMPLEPROCESSOR_H_

class SampleProcessor {
public:
				SampleProcessor(){}
	virtual		~SampleProcessor(){}
	virtual bool	Process ( int16_t* inputBuffer, int16_t* outputBuffer, int32_t bufferSize )=0;

};
#endif /* SAMPLEPROCESSOR_H_ */
