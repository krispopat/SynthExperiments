package uk.co.krispopat.synth;

public class AndroidGlue {
	
	public native void synthInit(boolean suspended, int sampleRate, int bufferSize );
	
	public native void synthStart();
	
	public native void synthStop();
	
	public native void synthDestroy();
	
	public native void synthMIDIMessage(byte[] msg);
	
	public native float[] testRamp ( int rampType, int rampSize, int rampLengthMS, float startAmp, float endAmp );
	
	public native int[] getSineWave(int size);
	
	public native int[] getSawtoothWave(int size);
	
	public native int[] getTriangleWave(int size);
	
	public native short[] getOutputBuffer();
	
	
	static {
		System.loadLibrary("KPSynth");
	}
}
