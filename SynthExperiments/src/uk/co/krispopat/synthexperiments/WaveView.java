package uk.co.krispopat.synthexperiments;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

/**
 * TODO: document your custom view class.
 */
public class WaveView extends View {
	
	private int contentWidth;
	private int contentHeight;
	private short[] waveTable;
	private float[] rampTable;
	private Rect bounds = null;
	private Paint paint;
	


	public WaveView(Context context) {
		super(context);
		init(null, 0);

	}
	
	public WaveView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(attrs, 0);
	}

	public WaveView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(attrs, defStyle);
	}
	
	
	private void init(AttributeSet attrs, int defStyle) {
		paint = new Paint();
		paint.setStrokeWidth(1);
		paint.setColor(Color.BLACK);
		paint.setStyle(Paint.Style.STROKE);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		if ( bounds != null ) {
			canvas.drawRect(bounds, paint);
		}
		float points[] = null;
		if ( waveTable != null ) {
			int maxAmplitude = 0x8000;
			
			int totalWaveRange = maxAmplitude * 2;
			
			float sampleWidthDelta = (float)waveTable.length / (float)contentWidth;
			float indexDelta = ( sampleWidthDelta > 1 )?sampleWidthDelta:1;
			float sampleAmpDelta = (float)totalWaveRange / (float)contentHeight;
			int nx = ( contentWidth > waveTable.length ) ? waveTable.length : contentWidth;
			nx *= 2;
			points = new float[nx];
			float hpos = 0;
			
			for ( int i = 0; i < nx; i+=2 ) {
				int arrayIndex = (int)((i / 2) * indexDelta);
				points[i] = hpos;
				int amp = waveTable[arrayIndex];
				points[i+1] = ( contentHeight / 2 ) - ( (float)amp / sampleAmpDelta );
				hpos += (sampleWidthDelta < 1 ) ? 1/sampleWidthDelta : 1;
			}
			
		}
		else if ( rampTable != null ) {
			float sampleWidthDelta = (float)rampTable.length / (float)contentWidth;
			float indexDelta = ( sampleWidthDelta > 1 )?sampleWidthDelta:1;
			int nx = ( contentWidth > rampTable.length ) ? rampTable.length : contentWidth;
			nx *= 2;
			points = new float[nx];
			float hpos = 0;
			
			for ( int i = 0; i < nx; i+=2 ) {
				int arrayIndex = (int)((i / 2) * indexDelta);
				points[i] = hpos;
				points[i+1] = contentHeight - (rampTable[arrayIndex] * contentHeight);
				hpos += (sampleWidthDelta < 1 ) ? 1/sampleWidthDelta : 1;
			}
		}
		if ( points != null ) {
			for ( int i = 0; i < (points.length-3); i += 2 ) {
				canvas.drawLine(points[i], points[i+1], points[i+2], points[i+3], paint);
			}
		}
	}
	
	
	protected void initVars () {
		int paddingLeft = getPaddingLeft();
		int paddingTop = getPaddingTop();
		int paddingRight = getPaddingRight();
		int paddingBottom = getPaddingBottom();

		contentWidth = getWidth() - paddingLeft - paddingRight;
		contentHeight = getHeight() - paddingTop - paddingBottom;
		bounds = new Rect(0,0,getWidth(),getHeight());
	}

	public short[] getWaveTable() {
		return waveTable;
	}

	public void setWaveTable(short[] waveTable) {
		this.waveTable = waveTable;
		this.rampTable = null;
		initVars();
	}
	
	public float[] getRampTable() {
		return rampTable;
	}
	
	public void setRampTable(float[] rampTable ) {
		this.rampTable = rampTable;
		this.waveTable = null;
		initVars();
	}
}
