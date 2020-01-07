package com.rohit_r_jadhav.event_handler;

import androidx.appcompat. app.AppCompatActivity;

import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;
import android.view.View;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;


public class MainActivity extends AppCompatActivity{
	
	private MyView myview;
	
	@Override
	protected void onCreate(Bundle saveInstanceState){

		super.onCreate(saveInstanceState);
		//setContentView(R.layout.activity_main);

		
		//Get rid of Title Bar
		this.supportRequestWindowFeature(Window.FEATURE_NO_TITLE);


		this.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
												View.SYSTEM_UI_FLAG_IMMERSIVE);


		//Get Rid of Navigation Bar
		this.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

		//Do FullScreen
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

		//Forced Landscape
		this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		//Set Background Color
		this.getWindow().getDecorView().setBackgroundColor(Color.BLACK);

		//Define our Own View
		myview = new MyView(this);
		
		//Set Our own View
		setContentView(myview);
				
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

