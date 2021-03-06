#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#define _USE_MATH_DEFINES 1
#include<math.h>


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

enum {
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXCOORD0
};


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

//For FullScreen
bool bIsFullScreen = false;

//For OpenGL
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColormap;
Window gWindow;

//For OpenGL
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
GLXFBConfig gGLXFBConfig;
GLXContext gGLXContext;

//For Uniform
GLuint mvpUniform;

//For Perspective
mat4 perspectiveProjectionMatrix;

//For Blue_Grid
GLuint vao_Grid;
GLuint vbo_Grid_Position;

//For Red_And_Blue_Axis
GLuint vao_Axis;
GLuint vbo_Axis_Position;
GLuint vbo_Axis_Color;

//For Yellow Triangle
GLuint vao_Triangle;
GLuint vbo_Triangle_Position;
GLuint vbo_Triangle_Color;

//For Yellow Rect
GLuint vao_Rect;
GLuint vbo_Rect_Position;
GLuint vbo_Rect_Color;

//For Yellow InCircle
GLuint vao_Circle;
GLuint vbo_Circle_Position;
GLuint vbo_Circle_Color;

GLfloat Incircle_Center[3];
GLfloat Incircle_Radius;


//For Program
GLuint gShaderProgramObject;

//For Error
FILE *gbFile = NULL;


int main(void){
	
	void CreateWindow(void);
	void initialize(void);
	void resize(int, int);
	void ToggleFullScreen(void);
	void display(void);
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
		fprintf(gbFile, "Log Created!!\n");


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
							printf("Left Button!!\n");
							break;
						case 2:
							printf("Middle Button!!\n");
							break;
						case 3:
							printf("Right Button!!\n");
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
	
	static int frameBufferAttributes[] = {
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_DOUBLEBUFFER, True,
		None
	};

	GLXFBConfig *pGLXFBConfig = NULL;
	int iNumberOfFBConfig;
	XVisualInfo *pTempXVisualInfo = NULL;
	GLXFBConfig bestFBConfig;



	gpDisplay = XOpenDisplay(NULL);
	if(gpDisplay == NULL){
		printf("XOpenDisplay() Failed!!\n");
		uninitialize();
		exit(1);
	}

	defaultScreen = XDefaultScreen(gpDisplay);


	pGLXFBConfig = glXChooseFBConfig(gpDisplay, defaultScreen, frameBufferAttributes, &iNumberOfFBConfig);
	fprintf(gbFile, "Match: %d\n", iNumberOfFBConfig);

	int bestFrameBufferConfig = -1;
	int bestSamples = -1;
       	int worstFrameBufferConfig = -1;
	int worstSamples = -1;

	for(int i = 0; i < iNumberOfFBConfig; i++){
		pTempXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, pGLXFBConfig[i]);
		if(pTempXVisualInfo){
			
			GLint samples, sampleBuffers;

			glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], GLX_SAMPLES, &samples);
			glXGetFBConfigAttrib(gpDisplay, pGLXFBConfig[i], GLX_SAMPLE_BUFFERS, &sampleBuffers);

			if(bestFrameBufferConfig < 0 || sampleBuffers && samples > bestSamples){
				bestFrameBufferConfig = i;
				bestSamples = samples;
			}

			if(worstFrameBufferConfig < 0 || sampleBuffers && samples < worstSamples){
				worstFrameBufferConfig = i;
				worstSamples = samples;
			}
		}
		XFree(pTempXVisualInfo);
		pTempXVisualInfo = NULL;
	}

	bestFBConfig = pGLXFBConfig[bestFrameBufferConfig];
	gGLXFBConfig = bestFBConfig;
	XFree(pGLXFBConfig);
	pGLXFBConfig = NULL;

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

	styleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;

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
		printf("XCreateWindow() Failed!!\n");
		uninitialize();
		exit(1);
	}

	XStoreName(gpDisplay, gWindow, "12-GraphPaper_WithShapes");

	Atom windowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(gpDisplay, gWindow, &windowManagerDelete, 1);

	XMapWindow(gpDisplay, gWindow);
}

void ToggleFullScreen(void){

	Atom wm_state;
	Atom fullscreen;
	XEvent xev = {0};

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
	
	void resize(int, int);
	void uninitialize(void);
	void FillGridPosition(GLfloat[]);
	void Calculation(GLfloat[]);


	GLenum Result;
	GLuint vertexShaderObject;
	GLuint fragmentShaderObject;

	
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
		fprintf(gbFile, "glXCreateContextAttribsARB() Failed For 4.5 Version!!\n");
		fprintf(gbFile, "Getting Best From System!!\n");

		const int Attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
			GLX_CONTEXT_MINOR_VERSION_ARB, 0,
			None
		};

		gGLXContext = glXCreateContextAttribsARB(gpDisplay, gGLXFBConfig, NULL, True, Attribs);
	}

	if(!glXIsDirect(gpDisplay, gGLXContext))
		fprintf(gbFile, "S/W Context!!\n");
	else
		fprintf(gbFile, "H/W Context!!\n");


	glXMakeCurrent(gpDisplay, gWindow, gGLXContext);


	Result = glewInit();
	if(Result != GLEW_OK){
		printf("glewInit() Failed!!\n");
		uninitialize();
		exit(1);
	}
	
	/********** Vertex Shader **********/
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	const char *vertexShaderSourceCode = 
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec4 vColor;" \
		"out vec4 outColor;" \
		"uniform mat4 u_mvp_matrix;" \
		"void main(void)" \
		"{" \
			"gl_Position = u_mvp_matrix * vPosition;" \
			"outColor = vColor;" \
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
			if(szInfoLog){
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gbFile, "Vertex Shader Compilation Error: %s\n", szInfoLog);
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
		"in vec4 outColor;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
			"FragColor = outColor;" \
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
			if(szInfoLog){
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gbFile, "Fragment Shader Compilation Error: %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
				uninitialize();
				exit(1);
			}
		}
	}


	/********** Program Object **********/
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
			if(szInfoLog){
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gbFile, "Shader Program Linking Error: %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
				uninitialize();
				exit(1);
			}
		}
	}

	mvpUniform = glGetUniformLocation(gShaderProgramObject, "u_mvp_matrix");

	/********** Position  **********/
	GLfloat Grid_Position[40 * 6];
	FillGridPosition(Grid_Position);
	
	GLfloat Triangle_Position[] = {
		0.0f, 0.70f, 0.0f,
		-0.70f, -0.70f, 0.0f,
		0.70f, -0.70f, 0.0f,
		None
	};

	GLfloat Triangle_Color[] = {
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		None
	};

	GLfloat Rect_Position[] = {
		0.70f, 0.70f, 0.0f,
		-0.70f, 0.70f, 0.0f,
		-0.70f, -0.70f, 0.0f,
		0.70f, -0.70f, 0.0f,
		None
	};

	GLfloat Rect_Color[] = {
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		None
	};


	/********** To Calculate InCircle Radius and Center **********/
	Calculation(Triangle_Position);


	/********** Grid **********/
	glGenVertexArrays(1, &vao_Grid);
	glBindVertexArray(vao_Grid);
			
		/********** Position **********/
		glGenBuffers(1, &vbo_Grid_Position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Grid_Position);
		glBufferData(GL_ARRAY_BUFFER, 
				sizeof(Grid_Position),
				Grid_Position,
				GL_STATIC_DRAW);
		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);
		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/********** Color **********/
		glVertexAttrib3f(AMC_ATTRIBUTE_COLOR, 0.0f, 0.0f, 1.0f);

	glBindVertexArray(0);




	/********** Axis **********/
	glGenVertexArrays(1, &vao_Axis);
	glBindVertexArray(vao_Axis);

		/********** Position **********/
		glGenBuffers(1, &vbo_Axis_Position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Axis_Position);
		glBufferData(GL_ARRAY_BUFFER,
				6 * sizeof(GLfloat),
				NULL,
				GL_DYNAMIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);
		
		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/********** Color **********/
		glGenBuffers(1, &vbo_Axis_Color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Axis_Color);
		glBufferData(GL_ARRAY_BUFFER,
				6 * sizeof(GLfloat),
				NULL,
				GL_DYNAMIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_COLOR,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	/********** Triangle **********/
	glGenVertexArrays(1, &vao_Triangle);
	glBindVertexArray(vao_Triangle);

		/********** Position **********/
		glGenBuffers(1, &vbo_Triangle_Position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Triangle_Position);
		glBufferData(GL_ARRAY_BUFFER,
				sizeof(Triangle_Position),
				Triangle_Position,
				GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/********** Color **********/
		glGenBuffers(1, &vbo_Triangle_Color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Triangle_Color);
		glBufferData(GL_ARRAY_BUFFER, 
				sizeof(Triangle_Color),
				Triangle_Color,
				GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_COLOR,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);



	/********** Rectangle **********/
	glGenVertexArrays(1, &vao_Rect);
	glBindVertexArray(vao_Rect);

		/********** Position **********/
		glGenBuffers(1, &vbo_Rect_Position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Rect_Position);
		glBufferData(GL_ARRAY_BUFFER,
				sizeof(Rect_Position),
				Rect_Position,
				GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/********** Color **********/
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


	/********** Circle **********/
	glGenVertexArrays(1, &vao_Circle);
	glBindVertexArray(vao_Circle);

		/********** Position **********/
		glGenBuffers(1, &vbo_Circle_Position);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Circle_Position);
		glBufferData(GL_ARRAY_BUFFER,
				1000 * 3 * sizeof(GLfloat),
				NULL,
				GL_DYNAMIC_DRAW);
		
		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, NULL);

		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/********** Color **********/
		glGenBuffers(1, &vbo_Circle_Color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_Circle_Color);
		glBufferData(GL_ARRAY_BUFFER,
				3 * 3000 * sizeof(GLfloat),
				NULL,
				GL_DYNAMIC_DRAW);

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
	

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	resize(WIN_WIDTH, WIN_HEIGHT);
}

void uninitialize(void){
	
	GLXContext currentContext = glXGetCurrentContext();

	if(vbo_Circle_Color){
		glDeleteBuffers(1, &vbo_Circle_Color);
		vbo_Circle_Color = 0;
	}

	if(vbo_Circle_Position){
		glDeleteBuffers(1, &vbo_Circle_Position);
		vbo_Circle_Position = 0;
	}

	if(vao_Circle){
		glDeleteVertexArrays(1, &vao_Circle);
		vao_Circle = 0;
	}

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

	if(vbo_Triangle_Color){
		glDeleteBuffers(1, &vbo_Triangle_Color);
		vbo_Triangle_Color = 0;
	}

	if(vbo_Triangle_Position){
		glDeleteBuffers(1, &vbo_Triangle_Position);
		vbo_Triangle_Position = 0;
	}

	if(vao_Triangle){
		glDeleteVertexArrays(1, &vao_Triangle);
		vao_Triangle = 0;
	}

	if(vbo_Axis_Color){
		glDeleteBuffers(1, &vbo_Axis_Color);
		vbo_Axis_Color = 0;
	}

	if(vbo_Axis_Position){
		glDeleteBuffers(1, &vbo_Axis_Position);
		vbo_Axis_Position = 0;
	}
	
	if(vao_Axis){
		glDeleteVertexArrays(1, &vao_Axis);
		vao_Axis = 0;
	}


	if(vbo_Grid_Position){
		glDeleteBuffers(1, &vbo_Grid_Position);
		vbo_Grid_Position = 0;
	}

	if(vao_Grid){
		glDeleteVertexArrays(1, &vao_Grid);
		vao_Grid = 0;
	}

	if(gShaderProgramObject){
		
		GLint iShaderNo = 0;
		GLint iShaderCount = 0;

		glUseProgram(gShaderProgramObject);

			glGetProgramiv(gShaderProgramObject, GL_ATTACHED_SHADERS, &iShaderCount);
			GLuint *pShaders = (GLuint*)malloc(sizeof(GLuint) * iShaderCount);

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
	
	if(currentContext != NULL && currentContext == gGLXContext){
		glXMakeCurrent(gpDisplay, 0, 0);
	}

	if(gGLXContext){
		glXDestroyContext(gpDisplay, gGLXContext);
	}

	if(gWindow){
		XDestroyWindow(gpDisplay, gWindow);
	}

	if(gColormap){
		XFreeColormap(gpDisplay, gColormap);
	}

	if(gpXVisualInfo){
		free(gpXVisualInfo);
		gpXVisualInfo = NULL;
	}

	if(gpDisplay){
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}
}

void resize(int width, int height){

	if(height == 0){
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	perspectiveProjectionMatrix = mat4::identity();
	perspectiveProjectionMatrix = perspective(45.0f,
					(GLfloat)width / (GLfloat)height,
					0.1f,
					100.0f);
}

void display(void){
	
	void FillCircle_Position(GLfloat[], GLfloat[], int);

	mat4 translateMatrix;
	mat4 modelViewMatrix;
	mat4 modelViewProjectionMatrix;

	GLfloat Axis_Position[6];
	GLfloat Axis_Color[6];

	GLfloat Circle_Position[3000 * 3];
	GLfloat Circle_Color[3000 * 3];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glUseProgram(gShaderProgramObject);
		
		/********** Grid **********/
		translateMatrix = mat4::identity();
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();

		translateMatrix = translate(0.0f, 0.0f, -3.0f);
		modelViewMatrix = modelViewMatrix * translateMatrix;
		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniformMatrix4fv(mvpUniform, 
				1,
				GL_FALSE,
			modelViewProjectionMatrix);

		glBindVertexArray(vao_Grid);

			glDrawArrays(GL_LINES, 0, 6 * 40);

		glBindVertexArray(0);


		/********** Axis **********/
		
		for(int i = 1; i <= 2; i++){
		
			translateMatrix = mat4::identity();
			modelViewMatrix = mat4::identity();
			modelViewProjectionMatrix = mat4::identity();

			translateMatrix = translate(0.0f, 0.0f, -3.0f);
			modelViewMatrix = modelViewMatrix * translateMatrix;
			modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

			glUniformMatrix4fv(mvpUniform,
					1,
					GL_FALSE,
				modelViewProjectionMatrix);


			if(i == 1){
				Axis_Position[0] = 0.0f;
				Axis_Position[1] = 1.0f;
				Axis_Position[2] = 0.0f;
			
				Axis_Position[3] = 0.0f;
				Axis_Position[4] = -1.0f;
				Axis_Position[5] = 0.0f;

				Axis_Color[0] = 1.0f;
				Axis_Color[1] = 0.0f;
				Axis_Color[2] = 0.0f;

				Axis_Color[3] = 1.0f;
				Axis_Color[4] = 0.0f;
				Axis_Color[5] = 0.0f;
			}
			else if( i == 2){
				Axis_Position[0] = -1.0f;
				Axis_Position[1] = 0.0f;
				Axis_Position[2] = 0.0f;

				Axis_Position[3] = 1.0f;
				Axis_Position[4] = 0.0f;
				Axis_Position[5] = 0.0f;


				Axis_Color[0] = 0.0f;
				Axis_Color[1] = 1.0f;
				Axis_Color[2] = 0.0f;
				
				Axis_Color[3] = 0.0f;
				Axis_Color[4] = 1.0f;
				Axis_Color[5] = 0.0f;

			}

			glBindVertexArray(vao_Axis);

				/***** Position *****/
				glBindBuffer(GL_ARRAY_BUFFER, vbo_Axis_Position);
					
					glBufferData(GL_ARRAY_BUFFER,
							sizeof(Axis_Position),
							Axis_Position,
							GL_DYNAMIC_DRAW);
				
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				/***** Color *****/
				glBindBuffer(GL_ARRAY_BUFFER, vbo_Axis_Color);
					
					glBufferData(GL_ARRAY_BUFFER,
							sizeof(Axis_Color),
							Axis_Color,
							GL_DYNAMIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, 0);

				
				glDrawArrays(GL_LINES, 0, 6);

			glBindVertexArray(0);

		}
	
		

		/********** Triangle **********/
		translateMatrix = mat4::identity();
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();

		translateMatrix = translate(0.0f, 0.0f, -3.0f);
		modelViewMatrix = modelViewMatrix * translateMatrix;
		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniformMatrix4fv(mvpUniform, 
				1,
				GL_FALSE,
			modelViewProjectionMatrix);

		glBindVertexArray(vao_Triangle);

			glDrawArrays(GL_LINE_LOOP, 0, 3);

		glBindVertexArray(0);
		
		
		
		
		/********** Rectangle **********/
		translateMatrix = mat4::identity();
		modelViewMatrix = mat4::identity();
		modelViewProjectionMatrix = mat4::identity();

		translateMatrix = translate(0.0f, 0.0f, -3.0f);
		modelViewMatrix = modelViewMatrix * translateMatrix;
		modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

		glUniformMatrix4fv(mvpUniform, 
				1,
				GL_FALSE,
			modelViewProjectionMatrix);

		glBindVertexArray(vao_Rect);
	
			glDrawArrays(GL_LINE_LOOP, 0, 4);

		glBindVertexArray(0);

		
		
		/********** Circle **********/
		for(int i = 1; i <= 2; i++){
			translateMatrix = mat4::identity();
			modelViewMatrix = mat4::identity();
			modelViewProjectionMatrix = mat4::identity();

			translateMatrix = translate(0.0f, 0.0f, -3.0f);
			modelViewMatrix = modelViewMatrix * translateMatrix;
			modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;

			glUniformMatrix4fv(mvpUniform, 	
					1,
					GL_FALSE,
				modelViewProjectionMatrix);

			if(i == 1){
				FillCircle_Position(Circle_Position, Circle_Color, 1);
			}
			else if(i == 2){
				FillCircle_Position(Circle_Position, Circle_Color, 2);
			}


			glPointSize(1.500f);
			glBindVertexArray(vao_Circle);
					
				glBindBuffer(GL_ARRAY_BUFFER, vbo_Circle_Position);
					
					glBufferData(GL_ARRAY_BUFFER,
							sizeof(Circle_Position),
							Circle_Position,
							GL_DYNAMIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, 0);


				glBindBuffer(GL_ARRAY_BUFFER, vbo_Circle_Color);

					glBufferData(GL_ARRAY_BUFFER,
							sizeof(Circle_Color),
							Circle_Color,
							GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				
				glDrawArrays(GL_POINTS, 0, 3 * 3000);

			glBindVertexArray(0);

		}
	glUseProgram(0);



	glXSwapBuffers(gpDisplay, gWindow);
}


void FillGridPosition(GLfloat arr[]){
	
	GLint j = 0;


	//Vertical Lines 20
	
	for(GLfloat i = 0.1f; i < 1.1f; i = i + 0.1f){
	
		//1
		arr[j] = 0.0f + i;
		arr[j + 1] = 1.0f;
		arr[j + 2] = 0.0f;

		arr[j + 3] = 0.0f + i;
		arr[j + 4] = -1.0f;
		arr[j + 5] = 0.0f;

		//2
		arr[j + 6] = 0.0f - i;
		arr[j + 7] = 1.0f;
		arr[j + 8] = 0.0f;
		
		arr[j + 9] = 0.0f - i;
		arr[j + 10] = -1.0f;
		arr[j + 11] = 0.0f;

		j = j + 12;
		fprintf(gbFile, "j: %d\n", j);
	}


	//Horizontal Lines 20 
	fprintf(gbFile, "               j: %d\n", j);
	for(GLfloat i = 0.1f; i < 1.1f; i = i + 0.1f){
		//1
		arr[j] = -1.0f;
		arr[j + 1] = 0.0f + i;
		arr[j + 2] = 0.0f;

		arr[j + 3] = 1.0f;
		arr[j + 4] = 0.0f + i;
	       	arr[j + 5] = 0.0f;

		//2
		arr[j + 6] = -1.0f;
		arr[j + 7] = 0.0f - i;
		arr[j + 8] = 0.0f;

		arr[j + 9] = 1.0f;
		arr[j + 10] = 0.0f - i;
		arr[j + 11] = 0.0f;

		j = j + 12;	
		fprintf(gbFile, "j: %d\n", j);
	}
}

void Calculation(GLfloat arr[]){
	GLfloat a, b, c;
	GLfloat s;

	//Distance Formula
	a = (GLfloat)sqrt(pow((arr[6] - arr[3]), 2) + pow((arr[7] - arr[4]), 2));
	b = (GLfloat)sqrt(pow((arr[6] - arr[0]), 2) + pow((arr[7] - arr[1]), 2));
	c = (GLfloat)sqrt(pow((arr[3] - arr[0]), 2) + pow((arr[4] - arr[1]), 2));

	s = (a + b + c) / 2;

	Incircle_Radius = (GLfloat)(sqrt(s * (s - a) * (s - b) * (s - c)) / s);

	Incircle_Center[0] = (a * arr[0] + b * arr[3] + c * arr[6]) / (a+ b+ c);
	Incircle_Center[1] = (a * arr[1] + b * arr[4] + c * arr[7]) / (a+ b+ c);
	Incircle_Center[2] = 0.0f;


	fprintf(gbFile, "Incircle_Radius: %f\n", Incircle_Radius);
	fprintf(gbFile, "InCenter x: %f      y: %f      z: %f     \n", Incircle_Center[0], Incircle_Center[1], Incircle_Center[2]);

}


void FillCircle_Position(GLfloat arr[], GLfloat arrColor[], int iFlag){
	
	memset(arr, 0, sizeof(GLfloat) * 3000 * 3);

	if(iFlag == 1){
		//InCircle
		int i = 0;
		for(int i = 0; i < 3000; i = i + 3){
			GLfloat x = (GLfloat)(2.0f * M_PI * i / 3000);
			arr[i] = (GLfloat)(Incircle_Radius * cos(x)) + Incircle_Center[0];
			arr[i + 1] = (GLfloat)(Incircle_Radius * sin(x)) + Incircle_Center[1];
			arr[i + 2] = 0.0f;


			arrColor[i] = 1.0f;		//R
			arrColor[i + 1] = 1.0f;		//G
			arrColor[i + 2] = 0.0f;		//B
		}
		//fprintf(gbFile, "i: %d\n", i);

	}
	else{
		//Outer Circle
		for(int i = 0; i < 3000; i = i + 3){
			GLfloat x = (GLfloat)(2.0f * M_PI * i / 3000);
			arr[i] = (GLfloat)(1.0f * cos(x));
			arr[i + 1] = (GLfloat)(1.0f * sin(x));
			arr[i + 2] = 0.0f;

			arrColor[i] = 1.0f;		//R
			arrColor[i + 1] = 1.0f;		//G
			arrColor[i + 2] = 0.0f; 	//B

			//fprintf(gbFile, "x: %f    y: %f    z: %f \n", arr[i], arr[i + 1], arr[i + 2]);
		}
	}
	
}

