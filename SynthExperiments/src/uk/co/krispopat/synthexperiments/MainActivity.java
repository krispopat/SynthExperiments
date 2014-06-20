package uk.co.krispopat.synthexperiments;

import uk.co.krispopat.synth.AndroidGlue;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends Activity {
	
	private WaveView waveView;
	private EditText bufferWidthInput;
	private EditText startAmpInput;
	private EditText endAmpInput;
	private AndroidGlue androidGlue;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		waveView = (WaveView) this.findViewById(R.id.waveView);
		bufferWidthInput = (EditText) this.findViewById(R.id.bufferWidth);
		startAmpInput = (EditText) this.findViewById(R.id.startAmp);
		endAmpInput = (EditText) this.findViewById(R.id.endAmp);
		androidGlue = new AndroidGlue();
	}
	
	
	public void displaySineWave(View view ) {
		showWaveTable(androidGlue.getSineWave(getBufferWidth ( )));
	}
	
	public void displayTriangleWave(View view ) {
		showWaveTable( androidGlue.getTriangleWave(getBufferWidth ( )));
	}
	
	public void displaySawtoothWave(View view ) {
		showWaveTable( androidGlue.getSawtoothWave(getBufferWidth ( )));
	}
	
	public void displayBellCurveRamp (View view ) {
		showRampTable ( androidGlue.testRamp(1, getBufferWidth ( ), 125, getStartAmp(), getEndAmp() ));
	}

	public void displaySlopeRamp (View view ) {
		showRampTable ( androidGlue.testRamp(2, getBufferWidth ( ), 125, getStartAmp(), getEndAmp() ));
	}

	private float getStartAmp ( ) {
		String sas = startAmpInput.getText().toString();
		if ( sas == null || sas.equals("")) {
			return (float)0.1;
		}
		return Float.parseFloat(sas);
	}
	
	
	public float getEndAmp ( ) {
		String eas = endAmpInput.getText().toString();
		if ( eas == null || eas.equals("")) {
			return (float)0.8;
		}
		return Float.parseFloat(eas);
	}
	private int getBufferWidth ( ) {
		String bws = bufferWidthInput.getText().toString();
		if (bws == null || bws.equals("")) {
			return 128;
		}
		return Integer.parseInt(bws);
	}
	
	private void showWaveTable ( int[] table ) {
		waveView.setWaveTable(table);
		waveView.invalidate();
	}
	
	private void showRampTable ( float[] table ) {
		waveView.setRampTable(table);
		waveView.invalidate();
	}
}
