package com.rohit_r_jadhav.ortho_blue_screen;

import android.opengl.GLSurfaceView;
import android.opengl.GLES32; 			//V3.1 Best is V3.2 

import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

import android.content.Context;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener{

	private GestureDetector gestureDetector;
	private final Context context;

	/*private void initialize(void);
	private void resize(int, int);
	private void display(void);*/

	GLESView(Context drawingContext){
		super(drawingContext);

		context = drawingContext;

		gestureDetector = new GestureDetector(drawingContext, this, null, false);
		gestureDetector.setOnDoubleTapListener(this);

		setEGLContextClientVersion(3);
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
	}


	//For Event Handling Like WndProc()

	@Override
	public boolean onTouchEvent(MotionEvent event){

		int eventaction = event.getAction();
		if(!gestureDetector.onTouchEvent(event))
			super.onTouchEvent(event);
		return(true);
	}


	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY){
		System.exit(0);
		return(true);
	}

	@Override
	public boolean onDoubleTap(MotionEvent e){
		return(true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e){
		return(true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e){
		return(true);
	}

	@Override
	public boolean onDown(MotionEvent e){
		return(true);
	}

	@Override
	public boolean onSingleTapUp(MotionEvent e){
		return(true);
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY){
		return(true);
	}

	@Override
	public void onLongPress(MotionEvent e){
		
	}

	@Override
	public void onShowPress(MotionEvent e){
		
	}


	/********** Now OpenGL **********/

	//Methods From Interface GLSurfaceView.Renderer

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config){
		String version = gl.glGetString(GL10.GL_VERSION);
		System.out.println("RTR: OpenGL ES Version: "+ version);

		initialize();
	}

	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height){
		resize(width, height);
	}

	@Override
	public void onDrawFrame(GL10 unused){
		display();
	}


	private void initialize(){
		GLES32.glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	}

	private void resize(int width, int height){
		if(height == 0)
			height = 1;

		GLES32.glViewport(0, 0, width, height);
	}

	private void display(){

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		requestRender();
	}


}

