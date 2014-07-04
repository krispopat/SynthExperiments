package uk.co.krispopat.synthexperiments;

import uk.co.krispopat.synth.AndroidGlue;
import uk.co.krispopat.synthexperiments.views.MIDIKeys;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.media.AudioManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;

public class MainActivity extends Activity {
	
	private AndroidGlue androidGlue;
	private WaveView waveView;
	private int interval = 40; // 25 fps
	private Handler handler;
	private MIDIKeys MIDIKeysView;
	
	@TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR1)
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		androidGlue = new AndroidGlue();
		
		
		AudioManager audioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
		
		String srs = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
		String fpbs = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
		
		int sampleRate = Integer.parseInt(srs);
		int bufferSize = Integer.parseInt(fpbs);
		androidGlue.synthInit(false, sampleRate, bufferSize);
		
		waveView = (WaveView) this.findViewById(R.id.waveView1);
		MIDIKeysView = (MIDIKeys) this.findViewById(R.id.mIDIKeys1);
		MIDIKeysView.setSynthGlue(androidGlue);
		updateWaveView();
		handler = new Handler();
		handler.postDelayed(updater, interval);
		
	}
	
	
	protected Runnable updater = new Runnable() {
		public void run () {
			updateWaveView();
			handler.postDelayed(updater, interval);
		}
	};
	
	
	protected void updateWaveView ( ) {
		short[] buffer = androidGlue.getOutputBuffer();
		waveView.setWaveTable(buffer);
		waveView.invalidate();
	}
	
	
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		handler.removeCallbacks(updater);
		androidGlue.synthDestroy();
	}

	
	
}
