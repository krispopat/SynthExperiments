package uk.co.krispopat.synth;

public class AndroidGlue {
	
	public native void synthInit(boolean suspended, int sampleRate, int bufferSize, int volume );
	
	public native void synthStart();
	
	public native void synthStop();
	
	public native void synthDestroy();
	
	public native void synthMIDIMessage(byte[] msg);
	
	public native float[] testRamp ( int rampType, int rampSize, int rampLengthMS, float startAmp, float endAmp );
	
	public native float[] getSineWave(int size);
	
	public native float[] getSawtoothWave(int size);
	
	public native float[] getTriangleWave(int size);
	
	public native short[] getOutputBuffer();
	
	
	static {
		System.loadLibrary("KPSynth");
	}
}
