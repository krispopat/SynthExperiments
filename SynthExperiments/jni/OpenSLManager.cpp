/*
 * OpenSLManager.cpp
 *
 *  Created on: 10 Jun 2014
 *      Author: Kris Popat
 */


#include <OpenSLManager.h>
#include <assert.h>
#include "SoundDistributor.h"

extern "C" {
	void playerCallback(SLAndroidSimpleBufferQueueItf queueItf, void *data)
	{
		OpenSLManager* manager = (OpenSLManager*)data;
		manager->transferSamples(queueItf, data);
	}
}


OpenSLManager::OpenSLManager(SoundDistributor* source, int sampleRate, int bufferSize  )
{
	mSource = source;
	mSampleRate = sampleRate;
	mBufferSize = bufferSize;
	mCurrentBuffer = 0;

	// sanity
	mPlayerObject = NULL;
	mPlayer = NULL;
	mPlayerBufferQueue = NULL;
	//mBufferQueue = NULL;

	// create open sl interface objects
	SLresult result;
	result = slCreateEngine(&mEngineObject, 0, NULL, 0, NULL, NULL);
	assert (result == SL_RESULT_SUCCESS);

	result = (*mEngineObject)->Realize(mEngineObject, SL_BOOLEAN_FALSE);
	assert (result == SL_RESULT_SUCCESS);

	result = (*mEngineObject)->GetInterface(mEngineObject, SL_IID_ENGINE, &mEngine);
	assert (result == SL_RESULT_SUCCESS);

	result = (*mEngine)->CreateOutputMix(mEngine, &mOutputMix,0, NULL, NULL);
	assert (result == SL_RESULT_SUCCESS);

	result = (*mOutputMix)->Realize(mOutputMix, SL_BOOLEAN_FALSE);
	assert (result == SL_RESULT_SUCCESS);
}



OpenSLManager::~OpenSLManager()
{
	if ( mPlayerObject != NULL ) {
		(*mPlayerObject)->Destroy(mPlayerObject);
	}
	if ( mOutputMix != NULL ) {
		(*mOutputMix)->Destroy(mOutputMix);
	}
	if ( mEngineObject != NULL ) {
		(*mEngineObject)->Destroy(mEngineObject);
	}
}


void OpenSLManager::Start()
{
	// check if we've done this already but simply stopped the player
	if ( mPlayer != NULL ) {
		SLresult result = (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PLAYING);
		assert (result == SL_RESULT_SUCCESS);
		return;
	}
	// else
	// setup formats (locators!)
	SLDataLocator_AndroidSimpleBufferQueue androidBufferLocator = {
			SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
			cNumberOfBuffers
	};
	SLDataFormat_PCM pcmFormat = {
			SL_DATAFORMAT_PCM,
			1,
			mSampleRate * 1000,
			SL_PCMSAMPLEFORMAT_FIXED_16,
			SL_PCMSAMPLEFORMAT_FIXED_16,
			SL_SPEAKER_FRONT_CENTER,
			SL_BYTEORDER_LITTLEENDIAN
	};
	SLDataLocator_OutputMix outputMixLocator = {
			SL_DATALOCATOR_OUTPUTMIX,
			mOutputMix
	};
	SLDataSource audioSource = {
			&androidBufferLocator,
			&pcmFormat
	};
	SLDataSink audioSink = {
			&outputMixLocator,
			NULL
	};
	const SLInterfaceID interfaceIds[2] = {
			SL_IID_BUFFERQUEUE, SL_IID_VOLUME
	};
	const SLboolean interfacesRequired[2] = {
			SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE
	};

	//Create and start audio player
	SLresult result;

	result = (*mEngine)->CreateAudioPlayer(
			mEngine,
			&mPlayerObject,
			&audioSource,
			&audioSink,
			2,
			interfaceIds,
			interfacesRequired);
	assert (result == SL_RESULT_SUCCESS);

	result = (*mPlayerObject)->Realize(mPlayerObject, SL_BOOLEAN_FALSE);
	assert (result == SL_RESULT_SUCCESS);

	result = (*mPlayerObject)->GetInterface(
			mPlayerObject,
			SL_IID_PLAY,
			&mPlayer);
	assert (result == SL_RESULT_SUCCESS);

	result = (*mPlayerObject)->GetInterface(mPlayerObject,
			SL_IID_BUFFERQUEUE,
			&mPlayerBufferQueue );
	assert (result == SL_RESULT_SUCCESS);

	result = (*mPlayerBufferQueue)->RegisterCallback (
			mPlayerBufferQueue,
			&playerCallback,
			NULL);
	assert (result == SL_RESULT_SUCCESS);

	result = (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PLAYING);
	assert (result == SL_RESULT_SUCCESS);
}


void OpenSLManager::Stop()
{
	if ( mPlayer != NULL ) {
		SLresult result = (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PLAYING);
		assert (result == SL_RESULT_SUCCESS);
	}
}



// protected methods
// This function treats mBuffer as three buffers
// by using pointer arithmetic.
// This practice is borrowed from Music Synthesizer for Android (url...)
void OpenSLManager::transferSamples( SLAndroidSimpleBufferQueueItf queue, void* data )
{
	int16_t *bufferPointer = mBuffer + (mBufferSize * mCurrentBuffer);
	mSource->GetAudioSamples( mBufferSize, bufferPointer );
	SLresult result = (*queue)->Enqueue(mPlayerBufferQueue, bufferPointer, mBufferSize * 2);
	assert ( result == SL_RESULT_SUCCESS);
	mCurrentBuffer = (mCurrentBuffer + 1) % cNumberOfBuffers;
}
