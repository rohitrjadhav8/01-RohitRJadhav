package com.rohit_r_jadhav.event_handler;

import androidx.appcompat.widget.AppCompatTextView;

import android.view.Gravity;
import android.content.Context;
import android.graphics.Color;

import android.view.MotionEvent;

import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

public class MyView extends AppCompatTextView implements OnGestureListener, OnDoubleTapListener{
	
	private GestureDetector gestureDetector;

	MyView(Context context){
		super(context);

		setTextColor(Color.rgb(0, 255, 0));
		setTextSize(60);
		setGravity(Gravity.CENTER);
		setText("Hello World !!!");

		gestureDetector = new GestureDetector(context, this, null, false);

		gestureDetector.setOnDoubleTapListener(this);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event){
		int eventaction = event.getAction();
		if(gestureDetector.onTouchEvent(event) == false)
			super.onTouchEvent(event);
		return(true);
	}

	@Override
	public boolean onDoubleTap(MotionEvent e){
		setText("Double Tap");
		return(true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e){
		return(true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e){
		setText("Single Tap");
		return(true);
	}

	//onGestureListener ch functions down below

	@Override
	public boolean onDown(MotionEvent e){
		return(true);
	}
		
	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY){
		return(true);
	}

	@Override
	public void onLongPress(MotionEvent e){
		setText("Long Press");
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY){
		setText("Scroll");
		return(true);
	}

	@Override
	public void onShowPress(MotionEvent e){

	}

	@Override
	public boolean onSingleTapUp(MotionEvent e){
		return(true);
	}
}	

 
