package com.rohit_r_jadhav.with_exit;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;
import android.view.View;

public class MainActivity extends AppCompatActivity{
	
	private MyView myView;

	@Override
	protected void onCreate(Bundle savedInstanceState){
		
		//Action Bar Hide Karnya Sathi
		this.supportRequestWindowFeature(Window.FEATURE_NO_TITLE);

		this.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | 
												View.SYSTEM_UI_FLAG_IMMERSIVE);

		//Full Screen Karnya Sathi
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		super.onCreate(savedInstanceState);
		
		this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		myView =  new MyView(this);

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

