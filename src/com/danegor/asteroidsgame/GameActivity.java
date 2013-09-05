package com.danegor.asteroidsgame;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Point;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.WindowManager;

public class GameActivity extends Activity {

	private static GLSurfaceView myGLView;
	
	static {
		System.loadLibrary("asteroids");
	}
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getActionBar().hide();
        myGLView = new GameSurfaceView(this);
       
        setContentView(myGLView);
    }

	@Override
	protected void onPause() {
		super.onPause();
		myGLView.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		myGLView.onResume();
	}
	
	public static Point getSurfaceSize() {
		Point size = new Point();
		size.x = myGLView.getMeasuredWidth();
		size.y = myGLView.getMeasuredHeight();
		return size;
	}
}

class GameSurfaceView extends GLSurfaceView {

	GLSurfaceView.Renderer myRenderer;
	
	public GameSurfaceView(Context context) {
		super(context);
		setEGLContextClientVersion(2);
		
		myRenderer = new GameRenderer();
		setRenderer(myRenderer);
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event) {
		final int pointerCount = event.getPointerCount();
		float xs[] = new float[pointerCount];
		float ys[] = new float[pointerCount];
		for (int p = 0; p < pointerCount; ++p) {
			xs[p] = event.getX(p);
			ys[p] = event.getY(p);
		}
        nativeTouch(xs, ys, xs.length, event.getAction() & 0x0000000F);
        return true;
    }

	@Override
    public void onPause() {
        super.onPause();
        nativePause();
    }

   @Override
    public void onResume() {
        super.onResume();
        nativeResume();
    }
	
   private static native void nativePause();
   private static native void nativeResume();
   private static native void nativeTouch(float[] xs, float[] ys, int size, int ev);
   
}

class GameRenderer implements GLSurfaceView.Renderer {

	@Override
	public void onDrawFrame(GL10 gl) {
		Point size = GameActivity.getSurfaceSize();
		nativeRender(size.x, size.y);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
//        nativeResize(width, height);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		Point size = GameActivity.getSurfaceSize();
		nativeInit(size.x, size.y);
	}
	
	private static native void nativeInit(int width, int height);
    private static native void nativeResize(int width, int height);
    private static native void nativeRender(int width, int height);
    private static native void nativeDone();
	
}