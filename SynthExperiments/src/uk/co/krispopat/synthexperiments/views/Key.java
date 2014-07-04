package uk.co.krispopat.synthexperiments.views;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;

public abstract class Key {
	private Rect bounds;
	private byte MIDINote;
	private Rect spot;
	private Paint paint;
	
	public Key ( Rect inBounds, byte inMIDINote ) {
		setBounds(inBounds);
		setMIDINote(inMIDINote);
		paint = new Paint();
		paint.setStyle(Paint.Style.FILL);
		paint.setColor(Color.CYAN);
		
		spot = new Rect();
		
		int s = ( bounds.right - bounds.left ) - 6;
		spot.left = bounds.left + 3;
		spot.right = spot.left + s;
		spot.bottom = bounds.bottom - 3;
		spot.top = spot.bottom - s * 2;
	}
	
	
	abstract public void draw ( Canvas canvas );
	
	
	public void drawSpot ( Canvas canvas ) {

		canvas.drawRect(spot, paint);
		
	}


	public Rect getBounds() {
		return bounds;
	}

	public void setBounds(Rect bounds) {
		this.bounds = bounds;
	}
	
	public boolean hit ( float x, float y ) {
		return bounds.contains((int)x, (int)y);
	}


	public byte getMIDINote() {
		return MIDINote;
	}


	public void setMIDINote(byte mIDINote) {
		MIDINote = mIDINote;
	}
}
