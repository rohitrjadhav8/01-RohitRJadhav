package com.rohit_r_jadhav.fullscreen_helloworld;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

//Added by me
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.view.View;

public class MainActivity extends AppCompatActivity {


 	private MyView myview;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_main);
	
		//Get Rid of Title bar
		this.supportRequestWindowFeature(Window.FEATURE_NO_TITLE);

		this.getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | 
												View.SYSTEM_UI_FLAG_IMMERSIVE);

		//Make FullScreen
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
								WindowManager.LayoutParams.FLAG_FULLSCREEN);

		//Forced Landscape Orientation
		this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		//Set BackgroundColor
		this.getWindow().getDecorView().setBackgroundColor(Color.BLACK);

		//Define our Own view
		myview = new MyView(this);

		//Set this as our view
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

