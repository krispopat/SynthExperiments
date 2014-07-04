package uk.co.krispopat.synthexperiments.views;

import java.util.ArrayList;

import uk.co.krispopat.synth.AndroidGlue;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.support.v4.view.MotionEventCompat;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;



public class MIDIKeys extends View {
	
	final static int noteCount = 16;
	
	private Rect bounds;
	private ArrayList<WhiteKey> whiteKeys;
	private ArrayList<BlackKey> blackKeys;
	private Paint backgroundPaint;
	private byte[] midiMessage;
	private int[] velocities;
	private int[] notes;
	
	
	private AndroidGlue synthGlue;

	
	

	public MIDIKeys(Context context) {
		super(context);
		init(null, 0);
	}

	public MIDIKeys(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(attrs, 0);
	}

	public MIDIKeys(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(attrs, defStyle);
	}

	private void init(AttributeSet attrs, int defStyle) {
		backgroundPaint = new Paint();
		backgroundPaint.setStyle(Paint.Style.FILL);
		backgroundPaint.setColor(Color.BLACK);
		
		midiMessage = new byte[3];
		velocities = new int[128];
		for ( int i = 0; i < 128; i++ ) {
			velocities[i] = 0;
		}
		notes = new int[noteCount];
		for ( int i = 0; i < noteCount; i++ ) {
			notes[i] = -1;
		}
	}
	
	
	public void buildKeys ( ) {
		bounds = new Rect();
		getDrawingRect( bounds );
		whiteKeys = new ArrayList<WhiteKey>();
		blackKeys = new ArrayList<BlackKey>();
		int eigth = ( bounds.right - bounds.left)  / 8;
		int whiteKeyWidth = eigth - 4;
		byte baseMIDI = 60; // c
		for ( int i = 0; i < 8; i++ ) {
			
			Rect keyBounds = new Rect();
			keyBounds.left = i * eigth + 1;
			keyBounds.right = keyBounds.left + whiteKeyWidth;
			keyBounds.bottom = bounds.bottom - 1;
			keyBounds.top = bounds.top;
			if ( i == 3 ) {
				// e to f
				baseMIDI -= 1;
			}
			else if ( i == 7 ) {
				// b to c
				baseMIDI -= 1;
			}
			WhiteKey key = new WhiteKey( keyBounds, (byte) (baseMIDI + (i * 2)) );
			whiteKeys.add(key);
		}
		baseMIDI = 61; // c#
		int blackKeyWidth = whiteKeyWidth / 2;
		int left = (int) (bounds.left + ( eigth * .75 ));
		for ( int i = 0; i < 5; i++ ) {
			Rect keyBounds = new Rect();
			if ( i == 2 ) {
				// F#
				baseMIDI += 1;
				left += eigth;
			}
			keyBounds.left = left + ( eigth * i );
			keyBounds.top = bounds.top;
			keyBounds.bottom = (int) ((bounds.bottom - bounds.top) * 0.62);
			keyBounds.right = keyBounds.left + blackKeyWidth;
			BlackKey key = new BlackKey ( keyBounds, (byte) (baseMIDI + (i*2)) );
			blackKeys.add(key);
			
		}
	
	}
	
	
	

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		buildKeys();
		this.getDrawingRect(bounds);
		canvas.drawRect(bounds, backgroundPaint);
		for ( WhiteKey key : whiteKeys ) {
			key.draw(canvas);
			byte note = key.getMIDINote();
			int v = velocities[note];
			if ( v > 0 ) {
				key.drawSpot(canvas);
			}
		}
		for ( BlackKey key : blackKeys ) {
			key.draw(canvas);
			byte note = key.getMIDINote();
			int v = velocities[note];
			if ( v > 0 ) {
				key.drawSpot(canvas);
			}
		}
		
	}

	@Override
	public boolean onTouchEvent( MotionEvent event ) {
		int action = event.getActionMasked(  );
		int index, pointerId;
		boolean redraw = false;
		float x,y;
		int c;
		switch ( action ) {
		case MotionEvent.ACTION_POINTER_DOWN:
		case MotionEvent.ACTION_DOWN:
			if ( action == MotionEvent.ACTION_POINTER_DOWN ) {
				index = event.getActionIndex();
			}
			else {
				index = 0;
			}
			pointerId = event.getPointerId( index );
			if (pointerId < noteCount && pointerId >= 0 ) {
				x = event.getX( index );
				y = event.getY(  index );
				redraw = touchDown ( pointerId, x, y, event.getPressure( index ) );
			}
			break;
			
		case MotionEvent.ACTION_POINTER_UP:
		case MotionEvent.ACTION_UP:
			if ( action == MotionEvent.ACTION_POINTER_UP ) {
				index = event.getActionIndex();
			}
			else {
				index = 0;
			}
			pointerId = event.getPointerId( index );
				if ( pointerId < noteCount && pointerId >= 0 ) {
				x = event.getX( index );
				y = event.getY(  index );
				redraw = touchUp ( pointerId, x, y );
			}
			break;
			
		case MotionEvent.ACTION_MOVE:
			c = MotionEventCompat.getPointerCount(event);
			for ( int i = 0; i < c; i++ ) {
				pointerId = event.getPointerId(i);
				if ( pointerId < noteCount && pointerId >= 0 ) {
					x = MotionEventCompat.getX( event, i );
					y = MotionEventCompat.getY( event, i );
					redraw |= touchMoved ( pointerId, x, y, event.getPressure( i ) );
				}
			}
			break;
			
		}
		if ( redraw ) {
			invalidate();
		}
		
		return true;
	}
	
	
	private boolean touchDown ( int pointerId, float x, float y, float pressure ) {
		byte note = hit ( x, y );
		if ( note == 0 ) {
			return false;
		}
		int n = notes[pointerId];
		if ( n == -1 ) {
			// not already there so we are good to go
			byte velocity = pressureToVelocity ( pressure );
			sendNoteOn ( note, velocity );
			velocities[note] = velocity;
			notes[pointerId] = note;
			return true;
		}
		return false;
	}
	
	
	private boolean touchMoved ( int pointerId, float x, float y, float pressure ) {
		byte note = hit ( x, y );
		if ( note == 0 ) {
			return false;
		}
		int oldNote = notes[pointerId];
		if ( oldNote == note ) {
			return false;
		}
		else {
			if ( oldNote != -1 ) {
				sendNoteOff ( (byte) oldNote );
				velocities[oldNote] = 0;
			}
			int v = pressureToVelocity(pressure);
			sendNoteOn ( note,(byte)v);
			velocities[note] = v;
			notes[pointerId] = note;
			return true;
		}
	}
	
	
	private boolean touchUp ( int pointerId, float x, float y ) {
		int note = notes[pointerId];
		if ( note == -1 ) {
			//something has gone wrong;
			return false;
		}
		sendNoteOff ( (byte) note );
		notes[pointerId] = -1;
		velocities[note] = 0;
		return true;
	}
	
	// borrowed from Music Synthesizer for Android
	private byte pressureToVelocity ( float pressure ) {
	    int velocity = (int) (0.5 * (pressure - 0.5f) * 127.0f + 64 + 0.5f);
	    if (velocity < 1) {
	      velocity = 1;
	    } else if (velocity > 127) {
	      velocity = 127;
	    }
	    return (byte)velocity;
	}
	
	
	private byte hit ( float x, float y ) {
		for ( BlackKey key: blackKeys ) {
			if ( key.hit(x, y)) {
				return key.getMIDINote();
			}
		}
		for ( WhiteKey key: whiteKeys ) {
			if ( key.hit(x, y)) {
				return key.getMIDINote();
			}
		}
		return 0;
	}
	
	private void sendNoteOn ( byte note, byte vol ) {
		midiMessage[0] = (byte) 0x90;
		midiMessage[1] = note;
		midiMessage[2] = vol;
		synthGlue.synthMIDIMessage(midiMessage);
		
	}
	
	
	private void sendNoteOff ( byte note ) {
		midiMessage[0] = (byte) 0x80;
		midiMessage[1] = note;
		midiMessage[2] = 0;
		synthGlue.synthMIDIMessage(midiMessage);
		
	}

	public AndroidGlue getSynthGlue() {
		return synthGlue;
	}

	public void setSynthGlue(AndroidGlue synthGlue) {
		this.synthGlue = synthGlue;
	}
	

}
