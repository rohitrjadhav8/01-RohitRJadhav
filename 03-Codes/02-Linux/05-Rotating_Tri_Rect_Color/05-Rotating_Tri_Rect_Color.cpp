#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>

#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include"vmath.h"

using namespace std;
using namespace vmath;

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

enum {
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXCOORD0
};

//For FullScreen
bool bIsFullScreen = false;

//For Window
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColormap;
Window gWindow;

//For OpenGL
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig gGLXFBConfig;
GLXContext gGLXContext;

//For Triangle
GLuint vao_Tri;
GLuint vbo_Tri_Position;
GLuint vbo_Tri_Color;
GLfloat angle_Tri =  0.0f;

//For Rectangle
GLuint vao_Rect;
GLuint vbo_Rect_Position;
GLuint vbo_Rect_Color;
GLfloat angle_Rect = 0.0f;

//For Matrix
GLuint mvpUniform;
mat4 perspectiveProjectionMatrix;

//For Shader Program Object
GLuint gShaderProgramObject;

//For Error
FILE *gbFile = NULL;

int main(void){
	
	void CreateWindow(void);
	void initialize(void);
	void ToggleFullScreen(void);
	void resize(int, int);
	void display(void);
	void update(void);
	void uninitialize(void);


	int winWidth = WIN_WIDTH;
	int winHeight = WIN_HEIGHT;
	bool bDone = false;
	XEvent event;
	KeySym keysym;

	gbFile = fopen("Log.txt", "w");
	if(gbFile == NULL){
		printf("Log Creation Failed!!\n");
		uninitialize();
		exit(1);
	}
	else
		fprintf(gbFile, "Log Close!!\n");


	CreateWindow();
	ToggleFullScreen();
	initialize();

	while(bDone == false){
		while(XPending(gpDisplay)){
			XNextEvent(gpDisplay, &event);
			switch(event.type){
				case MapNotify:
					break;
				case Expose:
					break;
				case MotionNotify:
					break;
				case DestroyNotify:
					break;
					
				case ConfigureNotify:
					winWidth = event.xconfigure.width;
					winHeight = event.xconfigure.height;
					resize(winWidth, winHeight);
					break;

				case KeyPress:
					keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);
					switch(keysym){
						case XK_Escape:
							bDone = true;
							break;

						case XK_F:
						case XK_f:
							if(bIsFullScreen == false){
								ToggleFullScreen();
								bIsFullScreen = true;
							}
							else{
								ToggleFullScreen();
								bIsFullScreen = false;
							}
							break;
						default:
							break;
					}
					break;

				case ButtonPress:
					switch(event.xbutton.button){
						case 1:
							break;
						case 2:
							break;
						case 3:
							break;
						default:
							break;	
					}
					break;

				case 33:
					bDone = true;
					break;

				default:
					break;
			}	
				
		}
		update();
		display();
	}
	uninitialize();
	return(0);
}

void CreateWindow(void){
	
	void uninitialize(void);

	int defaultScreen;
	XSetWindowAttributes winAttribs;
	int styleMask;

	static int frameBufferAttribs[] = {
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DOUBLEBUFFER, True,
		None
	};

	GLXFBConfig bestFBConfig;
	GLXFBConfig *pGLXFBConfig = NULL;
	XVisualInfo *pTempXVisualInfo = NULL;
	int iNumberOfFBConfig = 0;

	gpDisplay = XOpenDisplay(NULL);
	if(gpDisplay == NULL){
		fprintf(gbFile, "XOpenDisplay() Failed!!\n");
		uninitialize();
		exit(1);
	}

	defaultScreen = XDefaultScreen(gpDisplay);
	
	pGLXFBConfig = glXChooseFBConfig(gpDisplay, defaultScreen, frameBufferAttribs, &iNumberOfFBConfig);
	fprintf(gbFile, "Matching FBConfig: %d\n", iNumberOfFBConfig);

	int bestFrameBufferConfig = -1;
	int bestNumberOfSamples = -1;
	int worstFrameBufferConfig = -1;
	int worstNumberOfSamples = -1;

	for(int i = 0; i < iNumberOfFBConfig; i++){
		pTempXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, pGLXFBConfig[i]);
		if(pTempXVisualInfo){
			int samples, sampleBuffers;

			glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);
			glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], GLX_SAMPLES, &samples);

			if(bestFrameBufferConfig < 0 || sampleBuffers && samples > bestNumberOfSamples){
				bestFrameBufferConfig = i;
				bestNumberOfSamples = samples;
			}

			if(worstFrameBufferConfig < 0 || sampleBuffers && samples < worstNumberOfSamples){
				worstFrameBufferConfig = i;
				worstNumberOfSamples = samples;
			}
		}
		free(pTempXVisualInfo);
		pTempXVisualInfo = NULL;
	}

	bestFBConfig = pGLXFBConfig[bestFrameBufferConfig];
	gGLXFBConfig = bestFBConfig;
	XFree(pGLXFBConfig);

	gpXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, bestFBConfig);
	if(gpXVisualInfo == NULL){
		fprintf(gbFile, "glXGetVisualFromFBConfig() Failed!!\n");
		uninitialize();
		exit(1);
	}

	winAttribs.border_pixel = 0;
	winAttribs.border_pixmap = 0;
	winAttribs.background_pixel = BlackPixel(gpDisplay, defaultScreen);
	winAttribs.background_pixmap = 0;
	winAttribs.colormap = XCreateColormap(gpDisplay,
					RootWindow(gpDisplay, gpXVisualInfo->screen),
					gpXVisualInfo->visual,
					AllocNone);
	gColormap = winAttribs.colormap;
	winAttribs.event_mask = ExposureMask | VisibilityChangeMask | PointerMotionMask | 
				KeyPressMask | ButtonPressMask | StructureNotifyMask;

	styleMask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

	gWindow = XCreateWindow(gpDisplay,
			RootWindow(gpDisplay, gpXVisualInfo->screen),
			0, 0, 
			WIN_WIDTH, WIN_HEIGHT,
			0,
			gpXVisualInfo->depth,
			InputOutput,
			gpXVisualInfo->visual,
			styleMask,
			&winAttribs);

	if(!gWindow){
		fprintf(gbFile, "XCreateWindow() Failed!!\n");
		uninitialize();
		exit(1);
	}

	XStoreName(gpDisplay, gWindow, "05-Rotating_Triangle_and_Rectangle_Color");

	Atom windowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(gpDisplay, gWindow, &windowManagerDelete, 1);

	XMapWindow(gpDisplay, gWindow);
}

void ToggleFullScreen(void){
	
	Atom wm_state;
	Atom fullscreen;
	XEvent xev;

	wm_state = XInternAtom(gpDisplay, "_NET_WM_STATE", False);

	memset(&xev, 0, sizeof(XEvent));

	xev.type = ClientMessage;
	xev.xclient.window = gWindow;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = bIsFullScreen ? 0 : 1;

	fullscreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
	xev.xclient.data.l[1] = fullscreen;

	XSendEvent(gpDisplay,
		RootWindow(gpDisplay, gpXVisualInfo->screen),
		False,
		StructureNotifyMask,
		&xev);
}

void initialize(void){
	
	void uninitialize(void);
	void resize(int, int);

	GLuint vertexShaderObject;
	GLuint fragmentShaderObject;
	GLenum Result;

	
	/********** Context **********/

	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte*)"glXCreateContextAttribsARB");
	if(glXCreateContextAttribsARB == NULL){
		fprintf(gbFile, "glXGetProcAddressARB() Failed!!\n");
		uninitialize();
		exit(1);
	}

	const int Attributes[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
		GLX_CONTEXT_MINOR_VERSION_ARB, 5,
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None
	};

	gGLXContext = glXCreateContextAttribsARB(gpDisplay, gGLXFBConfig, NULL, True, Attributes);
	if(gGLXContext == NULL){
		fprintf(gbFile, "Not get Core Profile Context!!\n");
		fprintf(gbFile, "Geting Best From System\n");

		const int Attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
			GLX_CONTEXT_MINOR_VERSION_ARB, 0,
			None
		};

		gGLXContext  = glXCreateContextAttribsARB(gpDisplay, gGLXFBConfig, NULL, True, Attribs);
	}

	if(!glXIsDirect(gpDisplay, gGLXContext))
		fprintf(gbFile, "S/W Context!!\n");
	else
		fprintf(gbFile, "H/W Context!!\n");

	glXMakeCurrent(gpDisplay, gWindow, gGLXContext);



	Result = glewInit();
	if(Result != GLEW_OK){
		fprintf(gbFile, "glewInit() Failed!!\n");
		uninitialize();
		exit(1);
	}

	

	/********** Vertex Shader **********/
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	const char *vertexShaderSourceCode = 
		"#version 450 core" \ 
		"\n" \ 
		"uniform mat4 u_mvp_matrix;" \
		"in vec4 vPosition;" \
		"in vec4 vColor;" \
		"out vec4 Out_Color;" \ 
		"void main(void)" \ 
		"{" \ 
			"gl_Position = u_mvp_matrix * vPosition;" \
			"Out_Color = vColor;" \ 
		"}";

	glShaderSource(vertexShaderObject, 1,
		(const char**)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObject);

	int iShaderCompileStatus;
	int iInfoLogLength;
	char *szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if(iShaderCompileStatus == GL_FALSE){
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if(iInfoLogLength > 0){
			szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength);
			if(szInfoLog != NULL){
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gbFile, "VertexShader Compilation Error: %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
				uninitialize();
				exit(1);
			}
		}
	}



	/********** Fragment Shader **********/
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	const char *fragmentShaderSourceCode = 
		"#version 450 core" \ 
		"\n" \
		"in vec4 Out_Color;" \ 
		"out vec4 FragColor;" \
		"void main(void)" \ 
		"{" \ 
			"FragColor = Out_Color;" \
		"}";

	glShaderSource(fragmentShaderObject, 1,
		(const char**)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

	iShaderCompileStatus = 0;
	iInfoLogLength = 0;
	szInfoLog = NULL;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);
	if(iShaderCompileStatus == GL_FALSE){
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if(iInfoLogLength > 0){
			szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength);
			if(szInfoLog != NULL){
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gbFile, "FragmentShader Compilation Error: %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
				uninitialize();
				exit(1);
			}
		}
	}

	/*********** Shader Program **********/
	gShaderProgramObject = glCreateProgram();

	glAttachShader(gShaderProgramObject, vertexShaderObject);
	glAttachShader(gShaderProgramObject, fragmentShaderObject);

	glBindAttribLocation(gShaderProgramObject, AMC_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject, AMC_ATTRIBUTE_COLOR, "vColor");

	glLinkProgram(gShaderProgramObject);

	int iProgramLinkStatus;
	iInfoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iProgramLinkStatus);
	if(iProgramLinkStatus == GL_FALSE){
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if(iInfoLogLength > 0){
			szInfoLog = (char*)malloc(sizeof(char) * iInfoLogLength);
			if(szInfoLog != NULL){
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gbFile, "ShaderProgram Link Error: %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
				uninitialize();
				exit(1);
			}
		}
	}


	mvpUniform = glGetUniformLocation(gShaderProgramObject, "u_mvp_matrix");

	
	
	/********** Position **********/
	GLfloat Tri_Vertices[] = {
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	GLfloat Rect_Vertices[] = {
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	/********** Color **********/
	GLfloat Tri_Color[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLfloat Rect_Color[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	

	/********** Triangle Vao **********/
	glGenVertexArrays(1, &vao_Tri);
	glBindVertexArray(vao_Tri);

		/********** Position **********/
		glGenBuffers(1, &vbo_Tri_Position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Tri_Position);
		glBufferData(GL_ARRAY_BUFFER,
				sizeof(Tri_Vertices),
				Tri_Vertices,
				GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);
		
		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		/********** Color *********/
		glGenBuffers(1, &vbo_Tri_Color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Tri_Color);
		glBufferData(GL_ARRAY_BUFFER, 
				sizeof(Tri_Color),
				Tri_Color,
				GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_COLOR,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);

		glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);


	/********** Rectangle Vao **********/
	glGenVertexArrays(1, &vao_Rect);
	glBindVertexArray(vao_Rect);
		
		/********** Position **********/
		glGenBuffers(1, &vbo_Rect_Position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Rect_Position);
		glBufferData(GL_ARRAY_BUFFER, 
				sizeof(Rect_Vertices),
				Rect_Vertices,
				GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		/********** Color *********/
		glGenBuffers(1, &vbo_Rect_Color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Rect_Color);
		glBufferData(GL_ARRAY_BUFFER,
				sizeof(Rect_Color),
				Rect_Color,
				GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	glDisable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//perspectiveProjectionMatrix = mat4::identity();


	resize(WIN_WIDTH, WIN_HEIGHT);
}

void uninitialize(void){

	GLXContext currentContext = glXGetCurrentContext();

	if(vbo_Rect_Color){
		glDeleteBuffers(1, &vbo_Rect_Color);
		vbo_Rect_Color = 0;
	}

	if(vbo_Rect_Position){
		glDeleteBuffers(1, &vbo_Rect_Position);
		vbo_Rect_Position = 0;
	}	

	if(vao_Rect){
		glDeleteVertexArrays(1, &vao_Rect);
		vao_Rect = 0;
	}

	if(vbo_Tri_Color){
		glDeleteBuffers(1, &vbo_Tri_Color);
		vbo_Tri_Color = 0;
	}

	if(vbo_Tri_Position){
		glDeleteBuffers(1, &vbo_Tri_Position);
		vbo_Tri_Position = 0;
	}

	if(vao_Tri){
		glDeleteVertexArrays(1, &vao_Tri);
		vao_Tri = 0;
	}

	if(gShaderProgramObject){
		
		glUseProgram(gShaderProgramObject);

			GLuint *pShaders = NULL;
			int iShaderNo = 0;
			int iShaderCount;

			glGetProgramiv(gShaderProgramObject, GL_ATTACHED_SHADERS, &iShaderCount);
			pShaders = (GLuint*)malloc(sizeof(GLuint) * iShaderCount);
			if(pShaders){
					
				glGetAttachedShaders(gShaderProgramObject, iShaderCount, &iShaderCount, pShaders);

				for(iShaderNo = 0; iShaderNo < iShaderCount; iShaderNo++){
					glDetachShader(gShaderProgramObject, pShaders[iShaderNo]);
					glDeleteShader(pShaders[iShaderNo]);
					pShaders[iShaderNo] = 0;
				}
				free(pShaders);
				pShaders = NULL;

			}

		glUseProgram(0);
		glDeleteProgram(gShaderProgramObject);
		gShaderProgramObject = 0;
	}

	if(currentContext != NULL && currentContext == gGLXContext)
		glXMakeCurrent(gpDisplay, 0, 0);

	if(gGLXContext)
		glXDestroyContext(gpDisplay, gGLXContext);

	if(gWindow)
		XDestroyWindow(gpDisplay, gWindow);

	if(gColormap)
		XFreeColormap(gpDisplay, gColormap);

	if(gpXVisualInfo){
		XFree(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}

	if(gpDisplay){
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}

	if(gbFile){
		fprintf(gbFile, "Log Close!!\n");
		fclose(gbFile);
		gbFile = NULL;
	}
}

void resize(int width, int height){
	
	if(height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	perspectiveProjectionMatrix = mat4::identity();
	perspectiveProjectionMatrix = perspective(45.0f,
						(GLfloat)width / (GLfloat)height,
						0.1f,
						100.0f);
}

void display(void){
	

	mat4 translateMatrix;
	mat4 scaleMatrix;
	mat4 rotateMatrix;
	mat4 modelViewMatrix;
	mat4 modelViewProjectionMatrix;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(gShaderProgramObject);
		
		/********** Triangle **********/
		translateMatrix = mat4::identity();
		rotateMatrix = mat4::identity();
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();

		translateMatrix = translate(-1.5f, 0.0f, -6.0f);
		rotateMatrix = rotate(angle_Tri, 0.0f, 1.0f, 0.0f);
		modelViewMatrix = modelViewMatrix * translateMatrix * rotateMatrix;
		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;
		glUniformMatrix4fv(mvpUniform,
				1,
				GL_FALSE,
				modelViewProjectionMatrix);

		glBindVertexArray(vao_Tri);
			glDrawArrays(GL_TRIANGLES,
					0,
					3);

		glBindVertexArray(0);


		/********** Rectangle *********/
		translateMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();

		translateMatrix = translate(1.5f, 0.0f, -6.0f);
		scaleMatrix = scale(0.9f, 0.9f, 0.9f);
		//rotateMatrix = rotate(angle_Rect, 1.0f, 0.0f, 0.0f);
		rotateMatrix = rotate(angle_Rect, 0.0f, 0.0f);
		modelViewMatrix = modelViewMatrix * translateMatrix * scaleMatrix * rotateMatrix;
		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniformMatrix4fv(mvpUniform,
				1,
				GL_FALSE,
				modelViewProjectionMatrix);

		glBindVertexArray(vao_Rect);
			glDrawArrays(GL_TRIANGLE_FAN,
					0,
					4);

		glBindVertexArray(0);


	glUseProgram(0);

	glXSwapBuffers(gpDisplay, gWindow);
}

void update(void){
	
	angle_Tri = angle_Tri + 0.20f;
	angle_Rect = angle_Rect + 0.20f;

	if(angle_Tri > 360.0f)
	       angle_Tri = 0.0f;

	if(angle_Rect > 360.0f)
		angle_Rect = 0.0f;	
}


