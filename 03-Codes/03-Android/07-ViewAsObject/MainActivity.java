package com.rohit_r_jadhav.view_asa_object;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;

import androidx.appcompat.widget.AppCompatTextView;
import android.graphics.Color;
import android.view.Gravity;
//import android.content.Context;


public class MainActivity extends AppCompatActivity{


	@Override 
	protected void onCreate(Bundle savedInstanceState){

		super.onCreate(savedInstanceState);

		//For Action Bar Disable
		this.supportRequestWindowFeature(Window.FEATURE_NO_TITLE);

		//For FullScreen
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		//For Forced Orientation Landscape
		MainActivity.this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		AppCompatTextView myView = new AppCompatTextView(this);	

		myView.setTextColor(Color.rgb(0, 255, 0));
		myView.setTextSize(60);
		myView.setGravity(Gravity.CENTER);
		myView.setText("This is View As a Object Created using AppCompatTextView Class");

		setContentView(myView);
	}

	@Override 
	protected void onPause(){
		super.onPause();
	}

	@Override
	protected void onResume(){
		super.onResume();
	}
}