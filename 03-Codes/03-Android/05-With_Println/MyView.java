package com.rohit_r_jadhav.with_println;

import androidx.appcompat.widget.AppCompatTextView;

import android.content.Context;

import android.graphics.Color;

import android.view.Gravity;
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;


public class MyView extends AppCompatTextView implements OnGestureListener, OnDoubleTapListener{
	
	private GestureDetector gestureDetector;

	public MyView(Context context){
		super(context);


		setTextSize(60);
		setGravity(Gravity.CENTER);
		setTextColor(Color.rgb(0, 255, 0));
		setText("Hello World!!");

		gestureDetector = new GestureDetector(context, this, null, false);

		gestureDetector.setOnDoubleTapListener(this);
	}
		@Override 
		public boolean onTouchEvent(MotionEvent event){
			int eventaction = event.getAction();
			if(!gestureDetector.onTouchEvent(event)){
				super.onTouchEvent(event);
			}
			return(true);
		}

		@Override
		public boolean onDoubleTap(MotionEvent e){
			setText("Double Tap!!");
			System.out.println("RRJ: onDoubleTap()!!");
			return(true);
		}

		@Override
		public boolean onDoubleTapEvent(MotionEvent e){
						
			return(true);
		}


		@Override 
		public boolean onSingleTapConfirmed(MotionEvent e){
			setText("Single Tap!!");
			System.out.println("RRJ: onSingleTapConfirm()!!");
			return(true);
		}
	
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
			setText("Long Press!!");
			System.out.println("RRJ: onLongPress()!!");
		
		}

		@Override
		public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY){
			setText("Scroll!!");
			System.out.println("RRJ: onScroll()!!");
			return(true);
		}

		@Override
		public void onShowPress(MotionEvent e){
			
		}

		@Override 
		public boolean onSingleTapUp(MotionEvent e){
			//setText("Single Tap Up!!");
			return(true);
		}

}

