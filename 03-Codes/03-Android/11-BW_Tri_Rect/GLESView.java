package com.rohit_r_jadhav.bw_tri_rect;

//For OpenGL
import android.opengl.GLSurfaceView;
import android.opengl.GLES32;

import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

import android.opengl.Matrix;

//For  glBufferData
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

//For Event
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;
import android.content.Context;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener{

	private Context context;
	private GestureDetector gestureDetector;

	//For Shader
	private int vertexShaderObject;
	private int fragmentShaderObject;
	private int shaderProgramObject;

	//For Projection
	float perspectiveProjectionMatrix[] = new float[4 * 4];

	//For Uniform
	private int mvpUniform;

	//For Triangle
	private int vao_Tri[] = new int[1];
	private int vbo_Tri_Position[] = new int[1];
	private int vbo_Tri_Color[] = new int[1];

	//For Rectangle
	private int vao_Rect[] = new int[1];
	private int vbo_Rect_Position[] = new int [1];
	private int vbo_Rect_Color[] = new int[1];

	GLESView(Context drawingContext){

		super(drawingContext);
		context = drawingContext;

		gestureDetector = new GestureDetector(drawingContext, this, null, false);
		gestureDetector.setOnDoubleTapListener(this);

		setEGLContextClientVersion(3);
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
	}


	@Override
	public boolean onTouchEvent(MotionEvent event){
		int eventaction = event.getAction();
		if(!gestureDetector.onTouchEvent(event))
			super.onTouchEvent(event);
		return(true);
	}


	/********** Methods in OnDoubleTapListener Interface **********/
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


	/********** Methods in OnGestureListener Interface **********/
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

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY){
		uninitialize();
		System.exit(0);
		return(true);
	}



	/*********** Methods in GLSurfaceView.Renderer Interface **********/
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config){
		String version = gl.glGetString(GL10.GL_VERSION);
		String glsl_version = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
		String vendor = gl.glGetString(GL10.GL_VENDOR);
		String renderer = gl.glGetString(GL10.GL_RENDERER);

		System.out.println("RTR: OpenGL Version: " + version);
		System.out.println("RTR: OpenGLSL Version: " + glsl_version);
		System.out.println("RTR: Vendor: "+ vendor);
		System.out.println("RTR: Renderer: " + renderer);

		initialize();
	}


	@Override
	public void onSurfaceChanged(GL10 unuesd, int width, int height){
		resize(width, height);
	}

	@Override 
	public void onDrawFrame(GL10 unused){
		display();
	}


	/********** Now OpenGL **********/


	private void initialize(){


		/********** Vertex Shader **********/
		vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		final String vertexShaderSourceCode = String.format(
				"#version 320 es" +
				"\n" +
				"in vec4 vPosition;" +
				"in vec4 vColor;" +
				"out vec4 outColor;" +
				"uniform mat4 u_mvp_matrix;" +
				"void main(void)" +
				"{" +
					"gl_Position = u_mvp_matrix * vPosition;" +
					"outColor = vColor;" +
				"}"
			);

		GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);

		GLES32.glCompileShader(vertexShaderObject);

		int iShaderCompileStatus[] = new int[1];
		int iInfoLogLength[] = new int[1];
		String szInfoLog = null;

		GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
		if(iShaderCompileStatus[0] == GLES32.GL_FALSE){
			GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
			if(iInfoLogLength[0] > 0){
				szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
				System.out.println("RTR: Vertex Shader Compilation Error: " + szInfoLog);
				uninitialize();
				System.exit(0);
			}
		}



		/********** Fragment Shader **********/
		fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		final String fragmentShaderSourceCode = String.format(
				"#version 320 es" +
				"\n" +
				"precision highp float;" +
				"in vec4 outColor;" +
				"out vec4 FragColor;" +
				"void main(void)" +
				"{" +
					"FragColor = outColor;" +
				"}"
			);

		GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);

		GLES32.glCompileShader(fragmentShaderObject);

		iShaderCompileStatus[0] = 0;
		iInfoLogLength[0] = 0;
		szInfoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, iShaderCompileStatus, 0);
		if(iShaderCompileStatus[0] != 0){
			GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
			if(iInfoLogLength[0] > 0){
				szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
				System.out.println("RTR: Fragment Shader Compilation Error: "+ szInfoLog);
				uninitialize();
				System.exit(0);
			}
		}



		/********* Shader Program Object **********/
		shaderProgramObject = GLES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
		GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

		GLES32.glBindAttribLocation(shaderProgramObject, GLESMacros.AMC_ATTRIBUTE_POSITION, "vPosition");
		GLES32.glBindAttribLocation(shaderProgramObject, GLESMacros.AMC_ATTRIBUTE_COLOR, "vColor");

		GLES32.glLinkProgram(shaderProgramObject);

		int iProgramLinkStatus[] = new int[1];
		iInfoLogLength[0] = 0;
		szInfoLog = null;

		GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, iProgramLinkStatus, 0);
		if(iProgramLinkStatus[0] == GLES32.GL_FALSE){
			GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, iInfoLogLength, 0);
			if(iInfoLogLength[0] > 0){
				szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);
				System.out.println("RTR: Shader Program Linking Error: "+ szInfoLog);
				uninitialize();
				System.exit(0);
			}
		}


		mvpUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_mvp_matrix");



		/********** Position and Color **********/
		float Tri_Position[] = new float[] {
			0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f
		};

		float Tri_Color[] = new float[]{
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f
		};

		float Rect_Position[] = new float[] {
			1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f
		};

		float Rect_Color[] = new float[] {
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f
		};


		/********** Triangle **********/
		GLES32.glGenVertexArrays(1, vao_Tri, 0);
		GLES32.glBindVertexArray(vao_Tri[0]);

			/********** Position *********/
			GLES32.glGenBuffers(1, vbo_Tri_Position, 0);
			GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Tri_Position[0]);

				/********** For glBufferData **********/

				//Allocate ByteBuffer
				ByteBuffer byteBuffer_Tri_Position = ByteBuffer.allocateDirect(Tri_Position.length * 4);

				//Set Order
				byteBuffer_Tri_Position.order(ByteOrder.nativeOrder());

				//FloatBuffrt
				FloatBuffer positionBuffer_Tri = byteBuffer_Tri_Position.asFloatBuffer();

				//Put Data
				positionBuffer_Tri.put(Tri_Position);

				//Set Position to 0
				positionBuffer_Tri.position(0);

			GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
							Tri_Position.length * 4,
							positionBuffer_Tri,
							GLES32.GL_STATIC_DRAW);

			GLES32.glVertexAttribPointer(GLESMacros.AMC_ATTRIBUTE_POSITION,
									3,
									GLES32.GL_FLOAT,
									false,
									0, 0);
			GLES32.glEnableVertexAttribArray(GLESMacros.AMC_ATTRIBUTE_POSITION);
			GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


			/********** Color **********/
			GLES32.glGenBuffers(1, vbo_Tri_Color, 0);
			GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Tri_Color[0]);

				/*********** For glBufferData **********/
				ByteBuffer byteBuffer_Tri_Color = ByteBuffer.allocateDirect(Tri_Color.length * 4);
				byteBuffer_Tri_Color.order(ByteOrder.nativeOrder());
				FloatBuffer colorBuffer_Tri = byteBuffer_Tri_Color.asFloatBuffer();
				colorBuffer_Tri.put(Tri_Color);
				colorBuffer_Tri.position(0);

			GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
							Tri_Color.length * 4,
							colorBuffer_Tri,
							GLES32.GL_STATIC_DRAW);

			GLES32.glVertexAttribPointer(GLESMacros.AMC_ATTRIBUTE_COLOR, 
									3,
									GLES32.GL_FLOAT,
									false,
									0, 0);
			GLES32.glEnableVertexAttribArray(GLESMacros.AMC_ATTRIBUTE_COLOR);
			GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		GLES32.glBindVertexArray(0);


		/*********** Rectangle **********/
		GLES32.glGenVertexArrays(1, vao_Rect, 0);
		GLES32.glBindVertexArray(vao_Rect[0]);

			/********** Position *********/
			GLES32.glGenBuffers(1, vbo_Rect_Position, 0);
			GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Rect_Position[0]);

				/********** For glBufferData **********/

				//Allocate ByteBuffer
				ByteBuffer byteBuffer_Rect_Position = ByteBuffer.allocateDirect(Rect_Position.length * 4);

				//Set Order
				byteBuffer_Rect_Position.order(ByteOrder.nativeOrder());

				//FloatBuffrt
				FloatBuffer positionBuffer_Rect = byteBuffer_Rect_Position.asFloatBuffer();

				//Put Data
				positionBuffer_Rect.put(Rect_Position);

				//Set Position to 0
				positionBuffer_Rect.position(0);

			GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
							Rect_Position.length * 4,
							positionBuffer_Rect,
							GLES32.GL_STATIC_DRAW);

			GLES32.glVertexAttribPointer(GLESMacros.AMC_ATTRIBUTE_POSITION,
									3,
									GLES32.GL_FLOAT,
									false,
									0, 0);
			GLES32.glEnableVertexAttribArray(GLESMacros.AMC_ATTRIBUTE_POSITION);
			GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);


			/********** Color **********/
			GLES32.glGenBuffers(1, vbo_Rect_Color, 0);
			GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Rect_Color[0]);

				/*********** For glBufferData **********/
				ByteBuffer byteBuffer_Rect_Color = ByteBuffer.allocateDirect(Rect_Color.length * 4);
				byteBuffer_Rect_Color.order(ByteOrder.nativeOrder());
				FloatBuffer colorBuffer_Rect = byteBuffer_Rect_Color.asFloatBuffer();
				colorBuffer_Rect.put(Rect_Color);
				colorBuffer_Rect.position(0);

			GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
							Rect_Color.length * 4,
							colorBuffer_Rect,
							GLES32.GL_STATIC_DRAW);

			GLES32.glVertexAttribPointer(GLESMacros.AMC_ATTRIBUTE_COLOR, 
									3,
									GLES32.GL_FLOAT,
									false,
									0, 0);
			GLES32.glEnableVertexAttribArray(GLESMacros.AMC_ATTRIBUTE_COLOR);
			GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

		GLES32.glBindVertexArray(0);


		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		GLES32.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	}




	private void uninitialize(){

		//Rectangle

		if(vbo_Rect_Color[0] != 0){
			GLES32.glDeleteBuffers(1, vbo_Rect_Color, 0);
			vbo_Rect_Color[0] = 0;
		}

		if(vbo_Rect_Position[0] != 0){
			GLES32.glDeleteBuffers(1, vbo_Rect_Position, 0);
			vbo_Rect_Position[0] = 0;
		}

		if(vao_Rect[0] != 0){
			GLES32.glDeleteVertexArrays(1, vao_Rect, 0);
			vao_Rect[0] = 0;
		}

		System.out.println("RTR: Rectangle Done!!");

		//Triangle
		
		if(vbo_Tri_Color[0] != 0){
			GLES32.glDeleteBuffers(1, vbo_Tri_Color, 0);
			vbo_Tri_Color[0] = 0;
		}

		if(vbo_Tri_Position[0] != 0){
			GLES32.glDeleteBuffers(1, vbo_Tri_Position, 0);
			vbo_Tri_Position[0] = 0;
		}

		if(vao_Tri[0] != 0){
			GLES32.glDeleteVertexArrays(1, vao_Tri, 0);
			vao_Tri[0] = 0;
		}

		System.out.println("RTR: Triangle Done!!");
		

		//Shaders
		if(shaderProgramObject != 0){
			int iShaderCount[] = new int[1];
			int iShaderNo;
			
			System.out.println("RTR: In Shader Program Objects uninitialize !!");
	
			GLES32.glUseProgram(shaderProgramObject);

				/*GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, iShaderCount, 0);
				int iShaders[] = new int[iShaderCount[0]];
				GLES32.glGetAttachedShaders(shaderProgramObject, iShaderCount[0],
										iShaderCount, 0,
										iShaders, 0);

				System.out.println("RTR: iShaderCount: "+ iShaderCount[0]);
	
				for(iShaderNo = 0; iShaderNo < iShaderCount[0]; iShaderNo++){
					System.out.println("RTR: InLoop!");
					GLES32.glDetachShader(shaderProgramObject, iShaders[iShaderNo]);
					GLES32.glDeleteShader(iShaders[iShaderNo]);
					iShaders[iShaderNo] = 0;
					System.out.println("RTR: Shader Deleted!!");
				}*/

				if(fragmentShaderObject != 0){
					GLES32.glDetachShader(shaderProgramObject, fragmentShaderObject);
					GLES32.glDeleteShader(fragmentShaderObject);
					fragmentShaderObject = 0;
				}

				if(vertexShaderObject != 0){
					GLES32.glDetachShader(shaderProgramObject, vertexShaderObject);
					GLES32.glDeleteShader(vertexShaderObject);
					vertexShaderObject = 0;
				}				

			GLES32.glUseProgram(0);
			GLES32.glDeleteProgram(shaderProgramObject);
			shaderProgramObject = 0;
		}

	}



	private void resize(int width, int height){
		if(height == 0)
			height = 1;

		GLES32.glViewport(0, 0, width, height);

		Matrix.setIdentityM(perspectiveProjectionMatrix, 0);

		Matrix.perspectiveM(perspectiveProjectionMatrix, 0,
						45.0f,
						(float)width / (float)height,
						0.1f,
						100.0f);
	}



	private void display(){

		float translateMatrix[] = new float[4 * 4];
		float modelViewMatrix[] = new float[4 * 4];
		float modelViewProjectionMatrix[]  = new float[4 * 4];

		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		GLES32.glUseProgram(shaderProgramObject);

			/********** Triangle **********/
			Matrix.setIdentityM(translateMatrix, 0);
			Matrix.setIdentityM(modelViewMatrix, 0);
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			Matrix.translateM(translateMatrix, 0, -1.50f, 0.0f, -6.0f);
			Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, translateMatrix, 0);
			Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

			GLES32.glUniformMatrix4fv(mvpUniform, 1, false, modelViewProjectionMatrix, 0);

			GLES32.glBindVertexArray(vao_Tri[0]);
				GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 3);
			GLES32.glBindVertexArray(0);


			/********** Rectangle **********/
			Matrix.setIdentityM(translateMatrix, 0);
			Matrix.setIdentityM(modelViewMatrix, 0);
			Matrix.setIdentityM(modelViewProjectionMatrix, 0);

			Matrix.translateM(translateMatrix, 0, 1.50f, 0.0f, -6.0f);
			Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, translateMatrix, 0);
			Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

			GLES32.glUniformMatrix4fv(mvpUniform, 1, false, modelViewProjectionMatrix, 0);

			GLES32.glBindVertexArray(vao_Rect[0]);
				GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
			GLES32.glBindVertexArray(0);


		GLES32.glUseProgram(0);

		requestRender();
	}




};



