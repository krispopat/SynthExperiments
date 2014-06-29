package uk.co.krispopat.synthexperiments;

import uk.co.krispopat.synth.AndroidGlue;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.media.AudioManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.ToggleButton;

public class MainActivity extends Activity {
	
	private AndroidGlue androidGlue;
	private WaveView waveView;
	private int interval = 40; // 25 fps
	private Handler handler;
	
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



	public void noteButtonClicked ( View v )
	{
		ToggleButton button = (ToggleButton)v;
		
		int buttonId = button.getId();
		boolean state = button.isChecked();
		
		int note = 0;
		
		switch ( buttonId ) {
		case R.id.tbc:
			note = 60;
			break;
			
		case R.id.tbd:
			note = 62;
			break;
			
		case R.id.tbe:
			note = 64;
			break;
			
		case R.id.tbf:
			note = 65;
			break;
			
		case R.id.tbg:
			note = 67;
			break;
			
		case R.id.tba:
			note = 69;
			break;
			
		case R.id.tbb:
			note = 71;
			break;

		}
		
		byte msg[] = new byte[3];
		
		msg[0] = (byte) ((state == true )?0x90:0x80);
		msg[1] = (byte) note;
		msg[2] = (byte) 90;
		
		androidGlue.synthMIDIMessage(msg);
		
		
	}
	
	
}
