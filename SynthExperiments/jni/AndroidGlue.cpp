#include <jni.h>
#include <OpenSLManager.h>
#include <Ramp.h>
#include <stddef.h>
#include <Synthesizer.h>
#include <WaveTable.h>
#include <cstdint>

Synthesizer* gSynth;
OpenSLManager* gSLManager;



jshortArray convertToJavaArray ( JNIEnv *env, short* nativeArray, int32_t nativeArraySize )
{
	jshortArray javaArray = env->NewShortArray(nativeArraySize);

	jshort javaIntArray[nativeArraySize];
	int i;
	for ( i = 0; i < nativeArraySize; i++ ) {
		javaIntArray[i] = nativeArray[i];
	}
	env->SetShortArrayRegion( javaArray, 0, nativeArraySize, javaIntArray) ;
	return javaArray;
}


jfloatArray convertFloatArray ( JNIEnv *env, float* nativeArray, int32_t arraySize )
{
	jfloatArray returnArray = env->NewFloatArray( arraySize );
	jfloat floats[arraySize];
	int i;
	for ( i = 0; i < arraySize; i++ ) {
		floats[i] = nativeArray[i];
	}
	env->SetFloatArrayRegion(returnArray, 0, arraySize, floats );
	return returnArray;
}


extern "C" {
	void Java_uk_co_krispopat_synth_AndroidGlue_synthInit( JNIEnv *env, jobject obj, jboolean suspended, jint sampleRate, jint bufferSize )
	{
		gSynth = new Synthesizer( bufferSize );
		gSLManager = new OpenSLManager ( gSynth, sampleRate, bufferSize );
		if ( !suspended ) {
			gSLManager->Start();
		}
	}


	void Java_uk_co_krispopat_synth_AndroidGlue_synthStart( JNIEnv *env, jobject obj )
	{
		if ( gSLManager ) {
			gSLManager->Start();
		}
	}


	void Java_uk_co_krispopat_synth_AndroidGlue_synthStop ( JNIEnv *env, jobject obj )
	{
		if ( gSLManager ) {
			gSLManager->Stop();
		}
	}


	void Java_uk_co_krispopat_synth_AndroidGlue_synthDestroy( JNIEnv *env, jobject obj )
	{
		if ( gSLManager ) {
			delete gSLManager;
			gSLManager = NULL;
		}
		if ( gSynth ) {
			delete gSynth;
			gSynth = NULL;
		}
	}


	void Java_uk_co_krispopat_synth_AndroidGlue_synthMIDIMessage ( JNIEnv *env, jobject obj, jbyteArray jmsg  )
	{
		if ( gSynth ) {
			jboolean copy = false;
			uint8_t* message = ( uint8_t* )env->GetByteArrayElements(jmsg, &copy);
			int messageLength = env->GetArrayLength( jmsg );
			gSynth->MIDIMessage( message, messageLength );
			env->ReleaseByteArrayElements(jmsg, ( jbyte* )message, JNI_ABORT );
		}
	}

	jfloatArray Java_uk_co_krispopat_synth_AndroidGlue_testRamp ( JNIEnv *env, jobject object, jint rampType, jint rampSize, jint rampLengthMS, jfloat startAmp, jfloat endAmp )
	{
		Ramp* ramp = new Ramp( (RampType)rampType, rampSize, rampLengthMS, startAmp, endAmp );
		float* arr = ramp->getRampData();
		jfloatArray array = convertFloatArray ( env, arr, rampSize );
		delete ramp;
		return array;
	}


	jshortArray Java_uk_co_krispopat_synth_AndroidGlue_getSineWave( JNIEnv *env, jobject object, jint s )
	{
		WaveTable* sineTable = new WaveTable(sine, s, 44100);
		sineTable->generateWave();
		jshortArray returnArray = convertToJavaArray(env, sineTable->getTable(), s );
		delete sineTable;
		return returnArray;
	}


	jshortArray Java_uk_co_krispopat_synth_AndroidGlue_getSawtoothWave ( JNIEnv *env, jobject object, jint s )
	{
		WaveTable* sawtoothTable = new WaveTable(saw, s, 44100 );
		sawtoothTable->generateWave();
		jshortArray returnArray = convertToJavaArray ( env, sawtoothTable->getTable(), s );
		delete sawtoothTable;
		return returnArray;
	}


	jshortArray Java_uk_co_krispopat_synth_AndroidGlue_getTriangleWave ( JNIEnv *env, jobject object, jint s )
	{
		WaveTable* triangleTable = new WaveTable(triangle, s, 44100 );
		triangleTable->generateWave();
		jshortArray returnArray = convertToJavaArray ( env, triangleTable->getTable(), s );
		delete triangleTable;
		return returnArray;
	}
}
