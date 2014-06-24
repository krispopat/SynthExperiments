/*
 * OpenSLManager.h
 *
 *  Created on: 10 Jun 2014
 *      Author: Kris Popat
 */

#ifndef OPENSLMANAGER_H_
#define OPENSLMANAGER_H_

#include <stdint.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class SoundDistributor;

extern "C" {
	void playerCallback(SLAndroidSimpleBufferQueueItf queueItf, void *data);
}

const int cNumberOfBuffers = 2;



class OpenSLManager {
public:
	OpenSLManager( SoundDistributor* source, int sampleRate, int bufferSize );
	~OpenSLManager();

	void Start();
	void Stop();

	//void MIDIMessage ( int8_t *msg );

	friend void playerCallback(SLAndroidSimpleBufferQueueItf queueItf, void *data);

protected:
	void transferSamples ( SLAndroidSimpleBufferQueueItf queueItf, void *data);

private:
	int					mBufferSize;
	int					mSampleRate;
	int					mCurrentBuffer;
	int16_t*			mBuffer;
	SoundDistributor*	mSource;

	// OpenSL refs
	SLObjectItf 		mEngineObject;
	SLEngineItf			mEngine;

	SLObjectItf			mOutputMix;

	SLObjectItf			mPlayerObject;
	SLPlayItf			mPlayer;

	SLAndroidSimpleBufferQueueItf	mPlayerBufferQueue;
	//SLBufferQueueItf				mBufferQueue;
};




#endif /* OPENSLMANAGER_H_ */
