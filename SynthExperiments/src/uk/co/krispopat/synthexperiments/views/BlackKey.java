package uk.co.krispopat.synthexperiments.views;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;

public class BlackKey extends Key {
	
	private Paint fill;
	
	public BlackKey(Rect inBounds, byte inMIDINote) {
		super(inBounds, inMIDINote);
		fill = new Paint();
		fill.setStyle(Paint.Style.FILL);
		fill.setColor(Color.BLACK);
	}

	@Override
	public void draw(Canvas canvas) {
		canvas.drawRect(getBounds(), fill);
	}

}
