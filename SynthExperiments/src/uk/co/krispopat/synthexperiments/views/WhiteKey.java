package uk.co.krispopat.synthexperiments.views;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;


public class WhiteKey extends Key {
	
	private Paint outline;
	private Paint fill;

	public WhiteKey(Rect inBounds, byte inMIDINote) {
		super(inBounds, inMIDINote);
		outline = new Paint();
		outline.setStyle(Paint.Style.STROKE);
		outline.setColor(Color.BLACK);
		
		fill = new Paint();
		fill.setStyle(Paint.Style.FILL);
		fill.setColor(Color.WHITE);
	}

	@Override
	public void draw(Canvas canvas) {
		canvas.drawRect(getBounds(), fill);
		canvas.drawRect(getBounds(), outline);
	}
	
}
