/*#include<Windows.h>
#include<stdio.h>
#include<gl/GL.h>
#include<gl/GLU.h>
#include<math.h>*/

#include"HeadersAndLetter.h"
#include"Resource.h"

#pragma comment(lib , "opengl32.lib")
#pragma comment(lib , "glu32.lib")
#pragma comment(lib, "Winmm.lib")



#define WIN_WIDTH 800
#define WIN_HEIGHT 600
//#define PI 3.141592654

//For FullScreen
bool bIsFullScreen = false;
HWND ghwnd = NULL;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
DWORD dwStyle;
MONITORINFO mi;

//For SuperMan
bool bActiveWindow = false;
HDC ghdc = NULL;
HGLRC ghrc = NULL;


//For Matrix
#define World_Matrix_Width 512
#define World_Matrix_Depth 512

#define HousePuzzle_Matrix_Width 4
#define HousePuzzle_Matrix_Height 4


struct PointInfo {
	GLfloat x;
	GLfloat y;
	GLfloat z;

	//For House Puzzle
	GLuint iPosition;
	GLuint iOrientation;

	//For Clock Puzzle
	GLfloat fRadius;
	GLfloat fLengthOfHand;
	GLuint iClockHandPosition;

	struct PointInfo *next;
	struct PointInfo *prev;
};

//For World
struct PointInfo Matrix[World_Matrix_Depth][World_Matrix_Width];
struct PointInfo Direction[4];
GLint iWorldActivate = 1;
GLuint Texture_House;
GLuint Texture_Grass;
GLuint Texture_Water;
GLuint Texture_Tree;
FILE *gbWorld = NULL;


//For House Puzzle
struct PointInfo *gpHeadHouse = (struct PointInfo*)malloc(sizeof(struct PointInfo));
struct PointInfo *gpHouseMovement = NULL;
GLuint Texture_HousePuzzle[9];
	//For Connecting to The OtherParts
GLfloat fHouseDX;
GLfloat fHouseDY;
GLint iHousePuzzle = 0;
GLint iHousePuzzleActivate = 0;
GLint iHousePuzzleHint = 0;
FILE *gbHousePuzzle = NULL;
GLuint Texture_HousePuzzleHint;


//For Clock Puzzle
struct PointInfo *gpClockHead = NULL;
struct PointInfo *gpStart = NULL;
struct PointInfo *gpEnd = NULL;
struct PointInfo *gpNextClockHand = NULL;
GLuint ClockAnswer[] = { 4, 2 , 9, 7 };
FILE *gbClockPuzzle = NULL;
GLint iClockPuzzle = 0;
GLint iClockPuzzleActivate = 0;
GLint iClockPuzzleHint = 0;
GLuint Texture_ClockPuzzleHint;

//For Letters
GLint iInteract;
GLint iMove;
GLint iRotate;
GLint iKey;

//For Door
GLint iDoor = 0;
GLint iOpenDoor = 0;

//For Camera
GLfloat fEyeX = 0.0f;
GLfloat fEyeY = 1.70f;
GLfloat fEyeZ = 8.0f;

GLfloat fCamX = 0.0f;
GLfloat fCamY = 1.70f;
GLfloat fCamZ = 7.98f;

GLfloat fCamDZ = 0.05f;

//For End
GLint iEnding = 0;

//For Credits
GLint iEndingCredit = 0;


//For ModelLoading
GLfloat HouseVertexData[1110][3];
GLfloat HouseTextureData[2310][2];
GLfloat HouseNormalData[8][3];
GLint HouseFaceIndex[2180][9];

//For Tree
GLfloat TreeVertexData[7281][3];
GLfloat TreeTextureData[1][1];
GLfloat TreeNormalData[3121][3];
GLint TreeFaceIndex[14498][9];

//For Model
FILE *gpHouse = NULL;
FILE *gpTree = NULL;


//For Sequencing of Animation
GLint iSequenceOfDisplay = 0;
GLint iStart = 0;
GLint iContinue = 0;


//For Mouse Movements
static GLfloat angle = 78.000000f;
static GLfloat RadAngle = 1.361357f;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	
	//World Log File
	if (fopen_s(&gbWorld, "World.txt", "w") != 0) {
		MessageBox(NULL, TEXT("World Log Creation Failed!!\n"), TEXT("Error"), MB_OK);
		exit(0);
	}
	else
		fprintf(gbWorld, "Log Created!!\n");


	//House Puzzle Log File
	if (fopen_s(&gbHousePuzzle, "HousePuzzle.txt", "w") != 0) {
		MessageBox(NULL, TEXT("HousePuzzle Log Creation Failed!!\n"), TEXT("Error"), MB_OK);
		exit(0);
	}
	else
		fprintf(gbHousePuzzle, "Log Created!!\n");



	//Clock Puzzle Log File
	if (fopen_s(&gbClockPuzzle, "ClockPuzzle.txt", "w") != 0) {
		MessageBox(NULL, TEXT("ClockPuzzle Log Creation Failed!!\n"), TEXT("Error"), MB_OK);
		exit(0);
	}
	else
		fprintf(gbClockPuzzle, "Log Created!!\n");




	//For Models Attributes

	if (fopen_s(&gpHouse, "House3.txt", "r") != 0) {
		MessageBox(NULL, TEXT("Error Opening Obj File!!\n"), TEXT("Error"), MB_OK);
		exit(0);
	}

	if (fopen_s(&gpTree, "Tree.txt", "r") != 0) {
		MessageBox(NULL, TEXT("Error Opening Obj File!!\n"), TEXT("Error"), MB_OK);
		exit(0);
	}




	int initialize(void);
	void display(void);
	void ToggleFullScreen(void);
	void update(void);


	int iRet;
	bool bDone = false;

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szName[] = TEXT("RohitRJadhav-IntegrationPart2");

	wndclass.lpszClassName = szName;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;

	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;

	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szName,
		TEXT("RohitRJadhav-IntegrationPart2"),
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		100, 100,
		WIN_WIDTH, WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	SetFocus(hwnd);
	SetForegroundWindow(hwnd);

	iRet = initialize();
	if (iRet == -1) {
		fprintf(gbWorld, "ChoosePixelFormat() Failed!!\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -2) {
		fprintf(gbWorld, "SetPixelFormat() Failed!!\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -3) {
		fprintf(gbWorld, "wglCreateContext() Failed!!\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -4) {
		fprintf(gbWorld, "wglMakeCurrent() Failed!!\n");
		DestroyWindow(hwnd);
	}
	else
		fprintf(gbWorld, "initialize() done!!\n");

	ShowWindow(hwnd, iCmdShow);
	ToggleFullScreen();

	while (bDone == false) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				bDone = true;
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			if (bActiveWindow == true) {
				update();
			}
			display();
		}
	}
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	//For House Puzzle
	int iHousePuzzleStatus = 0;
	int CheckHousePuzzleAnswer(struct PointInfo*);

	//For Clock Puzzle
	int CheckClockPuzzleAnswer(struct PointInfo*);
	int iClockPuzzleStatus = 0;

	//For House and Hills Detection
	int iHouse = 0;
	int iWaterHills = 0;

	//For Rotation of Camara
	//static GLfloat angle = 0.0f;
	//static GLfloat angle = 82.000000f;
	static GLfloat theta = (GLfloat)(M_PI / 2.0f);
	//static GLfloat RadAngle = 0.0f;
	//static GLfloat RadAngle = 1.431170f;

	//For Mouse Movement
	static GLfloat fMouseX = 0.0f;
	static GLfloat fMouseY = 0.0f;
	static GLfloat fMouseXPrev = 0.0f;
	static GLfloat fMouseYPrev = 0.0f;



	void uninitialize(void);
	void resize(int, int);
	void ToggleFullScreen(void);
	int CheckForWaterHills(GLfloat, GLfloat);
	int CheckForHouse(GLfloat, GLfloat);
	void RotateDirections(GLfloat);

	switch (iMsg) {
	case WM_SETFOCUS:
		bActiveWindow = true;
		break;
	case WM_KILLFOCUS:
		bActiveWindow = false;
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_CHAR:
		switch (wParam) {
		//case 'F':
		//case 'f':
			//ToggleFullScreen();
			//break;

		case 'W':
		case 'w':
			if (iWorldActivate == 1 && iEndingCredit == 0) {
				if (((iHouse = CheckForHouse(Direction[0].x, Direction[0].z)) == 0) &&
					((iWaterHills = CheckForWaterHills(Direction[0].x, Direction[0].z)) == 0)) {
					fEyeX = fCamX;
					fEyeZ = fCamZ;
					RotateDirections(RadAngle);
					fCamX = Direction[0].x;
					fCamZ = Direction[0].z;
				}
			}


			break;


		case 'S':
		case 's':
			if (iWorldActivate == 1 && iEndingCredit == 0) {
				if (((iHouse = CheckForHouse(Direction[2].x, Direction[2].z)) == 0) &&
					((iWaterHills = CheckForWaterHills(Direction[2].x, Direction[2].z)) == 0)) {
					fEyeX = Direction[2].x;
					fEyeZ = Direction[2].z;
					RotateDirections(RadAngle);
					fCamX = Direction[0].x;
					fCamZ = Direction[0].z;
				}
			}
			break;

		case 'A':
		case 'a':
			if (iWorldActivate == 1 && iEndingCredit == 0) {
				if (((iHouse = CheckForHouse(Direction[3].x, Direction[3].z)) == 0) &&
					((iWaterHills = CheckForWaterHills(Direction[3].x, Direction[3].z)) == 0)) {
					fEyeX = Direction[3].x;
					fEyeZ = Direction[3].z;
					RotateDirections(RadAngle);
					fCamX = Direction[0].x;
					fCamZ = Direction[0].z;
				}
			}
			else if (iHousePuzzleActivate == 1) {
				gpHouseMovement = gpHeadHouse->next;
			}
			else if (iClockPuzzleActivate == 1) {
					gpNextClockHand =	gpNextClockHand->prev;
			}
			break;


		case 'D':
		case 'd':
			if (iWorldActivate == 1 && iEndingCredit == 0) {
				if (((iHouse = CheckForHouse(Direction[1].x, Direction[1].z)) == 0) &&
					((iWaterHills = CheckForWaterHills(Direction[1].x, Direction[1].z)) == 0)) {
					fEyeX = Direction[1].x;
					fEyeZ = Direction[1].z;
					RotateDirections(RadAngle);
					fCamX = Direction[0].x;
					fCamZ = Direction[0].z;
				}
			}
			else if (iHousePuzzleActivate == 1) {
				if (gpHouseMovement->next != NULL)
					gpHouseMovement = gpHouseMovement->next;
			}
			else if (iClockPuzzleActivate == 1) {
					gpNextClockHand = gpNextClockHand->next;
			}
			break;


		case 'P':
		case 'p':
			iStart = 1;
			PlaySound(MAKEINTRESOURCE(ID_STARTMUSIC), GetModuleHandle(NULL),
				SND_ASYNC | SND_LOOP | SND_RESOURCE);
			break;

		case 'C':
		case 'c':
			iContinue = 1;
			PlaySound(MAKEINTRESOURCE(ID_PLAYING), GetModuleHandle(NULL),
				SND_ASYNC | SND_LOOP | SND_RESOURCE);
			break;

		case 'E':
		case 'e':

			/*************For Interaction Purpuse!!\n*****************/

			if (iHousePuzzle == 1) {
			
				/********  HOUSE PUZZLE  **********/

				if (iHousePuzzleActivate == 0 && iHousePuzzleHint == 1) {
					//Puzzle is Solved and Showing Answers oR HINT
					iWorldActivate = 1;
					iHousePuzzleActivate = 0;
					iHousePuzzleHint = 0;
					iClockPuzzleActivate = 0;
					iClockPuzzleHint = 0;

				}	
				else if (iHousePuzzleActivate == 0) {
					//Getting Puzzle Puzzel Activate kar
					iHousePuzzleActivate = 1;
					iHousePuzzleHint = 0;
					iWorldActivate = 0;
					iClockPuzzleActivate = 0;
					iClockPuzzleHint = 0;
				}
				else {
					//Returing to World after Puzzle
					iWorldActivate = 1;
					iHousePuzzleActivate = 0;
					iClockPuzzleActivate = 0;
					iHousePuzzle = 0;
					iClockPuzzle = 0;
					iHousePuzzleHint = 0;
					iClockPuzzleHint = 0;
				}
			}
			else if (iClockPuzzle == 1) {

				/**************** Clock Puzzle *****************/

				if (iClockPuzzleActivate == 0 && iClockPuzzleHint == 1) {
						//Puzzle is Solved and Showing Answers oR HINT
						iClockPuzzleActivate = 0;
						iWorldActivate = 1;
						iHousePuzzleActivate = 0;
						iClockPuzzleHint = 0;
				}
				else if (iClockPuzzleActivate == 0) {
					//Getting Puzzle Puzzel Activate kar
					iClockPuzzleActivate = 1;
					iWorldActivate = 0;
					iHousePuzzleActivate = 0;
					iClockPuzzleHint = 0;
				}
				else {
					//Returing to World after Puzzle
					iClockPuzzleActivate = 0;
					iWorldActivate = 1;
					iHousePuzzleActivate = 0;
					iClockPuzzle = 0;
				}
			}
			else if (iDoor == 1 && iOpenDoor >= 2) {
				//For Door Opening 
				//PlaySound(MAKEINTRESOURCE(ID_ENDMUSIC), GetModuleHandle(NULL),
					//SND_ASYNC | SND_RESOURCE);

				fprintf(gbWorld, "\t\tDOOR: %d\n", iOpenDoor);

				for (int i = 0; i < 15; i++) {
					fEyeX = fCamX;
					fEyeZ = fCamZ;
					RotateDirections(RadAngle);
					fCamX = Direction[0].x;
					fCamZ = Direction[0].z;
				}
				iDoor = 0;
				iInteract = 0;
				//Time to End The Game
				iEnding = 1;
				PlaySound(MAKEINTRESOURCE(ID_ENDMUSIC), GetModuleHandle(NULL),
					SND_ASYNC | SND_LOOP | SND_RESOURCE);
				
			}
			
			break;

		case 'R':
		case 'r':
			if (iHousePuzzleActivate == 1) {
				gpHouseMovement->iOrientation++;
				if (gpHouseMovement->iOrientation > 4)
					gpHouseMovement->iOrientation = 1;
			}
			else if (iClockPuzzleActivate == 1) {
				gpNextClockHand->iClockHandPosition++;
				if (gpNextClockHand->iClockHandPosition > 12)
					gpNextClockHand->iClockHandPosition = 1;
			}
			break;

		case 'k':
		case 'K':
			/*************FOR Keys and For Hint Values ************/

			if (iHousePuzzleActivate == 1) {
				//Check For Puzzle Stattus if Solved then Give the key
				
				iHousePuzzleStatus = CheckHousePuzzleAnswer(gpHeadHouse->next);

				if (iHousePuzzleStatus == 9) {

					//PlaySound(MAKEINTRESOURCE(ID_HOUSEMUSIC1), GetModuleHandle(NULL),
						//SND_ASYNC | SND_LOOP | SND_RESOURCE);

					iWorldActivate = 0;
					iHousePuzzleActivate = 0;
					iClockPuzzleActivate = 0;
					iHousePuzzleHint = 1;
					iOpenDoor = iOpenDoor + 1;
				}
			}
			else if (iClockPuzzleActivate == 1) {
				//Check For Puzzle Stattus if Solved then Give the key
				
				iClockPuzzleStatus = CheckClockPuzzleAnswer(gpClockHead->next);

				//fprintf(gbClockPuzzle, "\t\tClock: %d\n", iClockPuzzleStatus);

				if (iClockPuzzleStatus == 4) {
					//PlaySound(MAKEINTRESOURCE(ID_HOUSEMUSIC2), GetModuleHandle(NULL),
						//SND_ASYNC | SND_LOOP | SND_RESOURCE);

					//Answer is Correct
					iHousePuzzleActivate = 0;
					iWorldActivate = 0;
					iClockPuzzleActivate = 0;
					iClockPuzzleHint = 1;
					iOpenDoor = iOpenDoor + 1;
				}
			}
			break;

		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		case 'X':
		case 'x':
			fprintf(gbWorld, "angle: %f\n", angle);
			break;
		}
		break;

	case WM_MOUSEMOVE:

		if (iSequenceOfDisplay == 1) {
			fMouseX = LOWORD(lParam);
			fMouseY = HIWORD(lParam);

			if (fMouseX > fMouseXPrev) {

				angle = angle + 2.0f;
				RadAngle = (GLfloat)(angle * M_PI / 180.0f);
				RotateDirections(RadAngle);
				fCamX = Direction[0].x;
				fCamZ = Direction[0].z;

			}
			else if (fMouseX < fMouseXPrev) {
				angle = angle - 2.00f;

				RadAngle = (GLfloat)(angle * M_PI / 180.0f);
				RotateDirections(RadAngle);
				fCamX = Direction[0].x;
				fCamZ = Direction[0].z;
			}


			if (fMouseY > fMouseYPrev) {
				fMouseYPrev = fMouseY;
				fCamY = fCamY - 0.001f;
			}
			else if (fMouseY < fMouseYPrev) {
				fMouseYPrev = fMouseY;
				fCamY = fCamY + 0.001f;
			}

			fMouseXPrev = fMouseX;
			fMouseYPrev = fMouseY;
		}
		break;

	case WM_ERASEBKGND:
		return(0);
	case WM_DESTROY:
		uninitialize();
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen(void) {



	if (bIsFullScreen == false) {

		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		mi = { sizeof(MONITORINFO) };
		if (dwStyle & WS_OVERLAPPEDWINDOW) {
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi)) {
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					(mi.rcMonitor.right - mi.rcMonitor.left),
					(mi.rcMonitor.bottom - mi.rcMonitor.top),
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		//ShowCursor(FALSE);
		bIsFullScreen = true;
	}
	else {
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
			HWND_TOP,
			0, 0, 0, 0,
			SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		bIsFullScreen = false;
	}
}

int initialize(void) {

	void resize(int, int);
	BOOL LoadTexture(GLuint*, TCHAR[]);

	//For World
	void WorldCalculation(void);
	void GenrateTerrain(void);
	void LoadHouseData(void);
	void LoadTreeData(void);
	void RotateDirections(GLfloat);

	//House Puzzle
	void HouseCalculation(GLfloat*, GLfloat*);
	void CreateHouseLinkedList(void);
	void GenrateRandomOrientation(void);

	//Clock Puzzle
	void CreateClockLinkedList(struct PointInfo*, int);
	void FillClockLinkedListData(struct PointInfo*);

	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	memset(&pfd, NULL, sizeof(PIXELFORMATDESCRIPTOR));

	ghdc = GetDC(ghwnd);

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;

	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	pfd.cDepthBits = 32;

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
		return(-1);

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
		return(-2);

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
		return(-3);

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
		return(-4);

	//************ World Part *****************
	//Calculations
	WorldCalculation();

	//GenrateTerrain
	GenrateTerrain();

	//Starting Co-ordinating For Starting Animation

/*Eye: -20.962618     1.700000    5.222321
	Cam : -20.893707     1.704000    5.207673
	RadAngle : 1.361357
	Angle : 78.000000*/


	fEyeX = -20.962618f;
	 fEyeY = 1.700000f;
	 fEyeZ = 5.222321f;
	 fCamX = -20.893707f;
	 fCamY = 1.704000f;
	 fCamZ = 5.207673f;
	 RotateDirections(RadAngle);


	/*//Player Co-Ordinate
	fEyeX = Matrix[234][345].x;
	fEyeZ = Matrix[234][345].z;
	fCamX = Matrix[233][345].x;
	fCamZ = Matrix[233][345].z;

	RotateDirections(0.0f);*/



	//Loading Model
	LoadHouseData();
	LoadTreeData();

	//******************* House Puzzle Part **********/
	gpHeadHouse->next = NULL;
	CreateHouseLinkedList();
	GenrateRandomOrientation();
	HouseCalculation(&fHouseDX, &fHouseDY);
	gpHouseMovement = gpHeadHouse->next;


	/******************* CLOCK PUZZLE PART   ********************/
	gpClockHead = (struct PointInfo*)malloc(sizeof(struct PointInfo));
	gpClockHead->next = NULL;
	gpClockHead->prev = NULL;

	CreateClockLinkedList(gpClockHead, 4);
	FillClockLinkedListData(gpClockHead);
	gpNextClockHand = gpClockHead->next;

	/******************* House Starting Music   ********************/
	//PlaySound(MAKEINTRESOURCE(ID_HOUSEMUSIC0), GetModuleHandle(NULL),
		//SND_ASYNC | SND_LOOP | SND_RESOURCE);


	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glClearDepth(1.0f);
	//Texture World
	glEnable(GL_TEXTURE_2D);
	LoadTexture(&Texture_House, MAKEINTRESOURCE(ID_HOUSE));
	LoadTexture(&Texture_Grass, MAKEINTRESOURCE(ID_LAND));
	LoadTexture(&Texture_Water, MAKEINTRESOURCE(ID_WATER));
	LoadTexture(&Texture_Tree, MAKEINTRESOURCE(ID_TREE));
	
	//Texture House Puzzle
	LoadTexture(Texture_HousePuzzle, MAKEINTRESOURCE(HOUSE_1));
	LoadTexture(Texture_HousePuzzle + 1, MAKEINTRESOURCE(HOUSE_2));
	LoadTexture(Texture_HousePuzzle + 2, MAKEINTRESOURCE(HOUSE_3));
	LoadTexture(Texture_HousePuzzle + 3, MAKEINTRESOURCE(HOUSE_4));
	LoadTexture(Texture_HousePuzzle + 4, MAKEINTRESOURCE(HOUSE_5));
	LoadTexture(Texture_HousePuzzle + 5, MAKEINTRESOURCE(HOUSE_6));
	LoadTexture(Texture_HousePuzzle + 6, MAKEINTRESOURCE(HOUSE_7));
	LoadTexture(Texture_HousePuzzle + 7, MAKEINTRESOURCE(HOUSE_8));
	LoadTexture(Texture_HousePuzzle + 8, MAKEINTRESOURCE(HOUSE_9));
	
	//Texture Clock Hint
	LoadTexture(&Texture_ClockPuzzleHint, MAKEINTRESOURCE(ID_CLOCK_HINT));

	//Texture House Hint
	LoadTexture(&Texture_HousePuzzleHint, MAKEINTRESOURCE(ID_HOUSE_HINT));

	glClearColor(0.0f, 0.0f, 0.05f, 0.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	resize(WIN_WIDTH, WIN_HEIGHT);
	return(0);
}


BOOL LoadTexture(GLuint *texture, TCHAR imageResourceID[]) {

	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	BOOL bStatus = FALSE;

	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if (hBitmap) {
		bStatus = TRUE;

		//Image cha data store kela bmp madhe
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		//
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		//GPU Texture Memory Create kar
		glGenTextures(1, texture);

		//Bind it
		glBindTexture(GL_TEXTURE_2D, *texture);

		//Setting the Parameter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//Store Texturee into GPU
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight,
			GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

		DeleteObject(hBitmap);
	}
	return(bStatus);
}

void uninitialize(void) {

	void DeleteHouse(void);
	void DeleteClockLinkedList(void);

	if (bIsFullScreen == true) {
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
			HWND_TOP,
			0, 0, 0, 0,
			SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		bIsFullScreen = false;
	}

	if (wglGetCurrentContext() == ghrc)
		wglMakeCurrent(NULL, NULL);

	if (ghrc) {
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc) {
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}


	if (gpHeadHouse) {
		DeleteHouse();
		free(gpHeadHouse);
		gpHeadHouse = NULL;
	}

	if (gpClockHead) {
		DeleteClockLinkedList();
		//free(gpClockHead);
		//gpClockHead = NULL;
	}

	if (gpTree) {
		fprintf(gpTree, "gpTree Close!!\n");
		fclose(gpTree);
		gpTree = NULL;
	}

	if (gpHouse) {
		fprintf(gpHouse, "gpHouse Close!!\n");
		fclose(gpHouse);
		gpHouse = NULL;
	}

	if (gbClockPuzzle) {
		fprintf(gbClockPuzzle, "Log Close!!\n");
		fclose(gbClockPuzzle);
		gbClockPuzzle = NULL;
	}

	if (gbHousePuzzle) {
		fprintf(gbHousePuzzle, "Log Close!!\n");
		fclose(gbHousePuzzle);
		gbHousePuzzle = NULL;
	}

	if (gbWorld) {
		fprintf(gbWorld, "Log Close!!\n");
		fclose(gbWorld);
		gbWorld = NULL;
	}


}

void resize(int width, int height) {
	if (height == 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}

void display(void) {
	void PatternTwo_Tri(void);
	void PatternTwo_Tri_Water(void);
	void House(GLfloat, GLfloat, GLfloat);
	void Tree(GLfloat, GLfloat, GLfloat);
	void DisplayHouse(void);
	void SquareFrame(void);
	void DrawClock(struct PointInfo*);
	void DrawRing(struct PointInfo*);
	void My_Cube(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat,GLfloat);
	void RotateDirections(GLfloat);

	static GLfloat fEndCount = 0.0f;
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	if (iSequenceOfDisplay == 0 && iStart == 1) {
		
		/***********Starting ****************/


		static GLfloat fPauseCounter = 0.0f;
		static int iSequence = 1;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(fEyeX, fEyeY, fEyeZ,
			fCamX, fCamY, fCamZ,
			0.0f, 1.0f, 0.0f);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, Texture_Water);
		PatternTwo_Tri_Water();

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, Texture_Grass);
		PatternTwo_Tri();

		//House
		glScalef(0.18f, 0.18f, 0.25f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_2D, Texture_House);
		House(-30.0f, 6.0f, 0.0f);
		My_Cube(-30.10f, -30.0f,
			Matrix[260][325].z, Matrix[208][325].z,
			13.10f, 6.0f);

		//Trees
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(fEyeX, fEyeY, fEyeZ,
			fCamX, fCamY, fCamZ,
			0.0f, 1.0f, 0.0f);
		glScalef(0.2f, 0.2f, 0.2f);
		glBindTexture(GL_TEXTURE_2D, Texture_Tree);
		//glBindTexture(GL_TEXTURE_2D, 0);

		//Front
		Tree(5.0f, 5.0f, -10.0f);
		Tree(50.0f, 5.0f, -30.0f);
		Tree(40.0f, 5.0f, 30.0f);



		 switch (iSequence) {
			
		 case 1:

			 /*********** Static View **********/
			 if (fPauseCounter > 20.0f) {
				 iSequence = 2;
				 fPauseCounter = 0.0f;
			 }
			 else
				 fPauseCounter += 0.5f;

			 break;

		 case 2:

			 /************ Approching Window ***********/

			SendMessage(ghwnd, WM_CHAR, (WPARAM)'W', NULL);
			 if (fEyeX > -3.315524) {
				 iSequence = 3;
				 fPauseCounter = 0.0f;

			 }

			 break;


		 case 3:
			
			 /********** Static Window **********/
			 glBindTexture(GL_TEXTURE_2D, 0);
			 glMatrixMode(GL_MODELVIEW);
			 glLoadIdentity();
			 glTranslatef(-0.060f, -0.050f, -0.25f);
			 glScalef(0.05f, 0.05f, 0.0f);
			 My_C();
			 My_To(-0.035f, 0.050f);
			 My_Continue(0.0f, 0.050f);

			 if (iContinue == 1) {
				 fPauseCounter = 0.0f;

				 fEyeX = Matrix[234][345].x;
				 fEyeZ = Matrix[234][345].z;
				 fCamX = Matrix[233][345].x;
				 fCamZ = Matrix[233][345].z;

				 RotateDirections(0.0f);
				 RadAngle = 0.0f;
				 angle = 0.0f;
				 iSequenceOfDisplay = 1;
			 }
			 break;

		 }

	}
	else if (iSequenceOfDisplay == 1){

		/********** Play The Game **********/

		if (fEndCount <= 500.0f) {	//477
			if (iWorldActivate == 1) {

				/********* World ********/

				//Terrain
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(fEyeX, fEyeY, fEyeZ,
					fCamX, fCamY, fCamZ,
					0.0f, 1.0f, 0.0f);

				glBindTexture(GL_TEXTURE_2D, 0);
				glBindTexture(GL_TEXTURE_2D, Texture_Water);
				PatternTwo_Tri_Water();

				glBindTexture(GL_TEXTURE_2D, 0);
				glBindTexture(GL_TEXTURE_2D, Texture_Grass);
				PatternTwo_Tri();

				//House
				glScalef(0.18f, 0.18f, 0.25f);
				glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindTexture(GL_TEXTURE_2D, Texture_House);
				House(-30.0f, 6.0f, 0.0f);
				My_Cube(-30.10f, -30.0f,
					Matrix[260][325].z, Matrix[208][325].z,
					13.10f, 6.0f);

				//Trees
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(fEyeX, fEyeY, fEyeZ,
					fCamX, fCamY, fCamZ,
					0.0f, 1.0f, 0.0f);
				glScalef(0.2f, 0.2f, 0.2f);
				glBindTexture(GL_TEXTURE_2D, Texture_Tree);
				//glBindTexture(GL_TEXTURE_2D, 0);

				//Front
				Tree(5.0f, 5.0f, -10.0f);
				Tree(50.0f, 5.0f, -30.0f);
				Tree(40.0f, 5.0f, 30.0f);

				/*Tree(5.0f, 5.0f, -30.0f);
				Tree(10.0f, 5.0f, 30.0f);
				Tree(10.0f, 5.0f, -30.0f);

				Tree(0.0f, 5.0f, -30.0f);
				Tree(00.0f, 5.0f, 30.0f);
				Tree(-10.0f, 5.0f, -30.0f);


				Tree(-10.0f, 5.0f, -60.0f);
				Tree(-10.0f, 5.0f, 0.0f);
				Tree(-10.0f, 5.0f, -60.0f);

				Tree(-20.0f, 5.0f, -60.0f);
				Tree(-20.0f, 5.0f, 0.0f);
				Tree(-20.0f, 5.0f, -60.0f);*/

				if (iInteract == 1) {
					glBindTexture(GL_TEXTURE_2D, 0);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(-0.060f, 0.0f, -0.25f);
					glScalef(0.06f, 0.06f, 0.0f);
					My_E();
					My_To(-0.035f, 0.0f);
					My_Interact(0.0f, 0.0f);
				}

			}
			else if (iHousePuzzleActivate == 1) {

				/**************HOUSE PUZZLE************/

				//Here we display our house Puzzle
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -3.0f);
				DisplayHouse();


				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -3.0f);
				SquareFrame();

				if (iInteract == 1) {
					glBindTexture(GL_TEXTURE_2D, 0);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(-0.150f, 0.080f, -0.25f);
					glScalef(0.05f, 0.05f, 0.0f);
					My_R();
					My_To(-0.13f, -0.080f);
					My_Rotate(-0.160f, -0.040f);

					glBindTexture(GL_TEXTURE_2D, 0);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(-0.160f, -0.0200f, -0.25f);
					glScalef(0.05f, 0.05f, 0.0f);
					My_A();
					glTranslatef(-0.0500f, -0.50f, 0.0f);
					My_D();
					My_To(-0.13f, 0.050f);
					My_Move(-0.130f, 0.080f);


					glBindTexture(GL_TEXTURE_2D, 0);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(0.140f, 0.03500f, -0.25f);
					glScalef(0.05f, 0.05f, 0.0f);
					My_K();
					My_To(0.13f, 0.00f);
					My_Key(0.130f, 0.0350f);

				}

			}
			else if (iClockPuzzleActivate == 1) {

				/************CLOCK PUZZLE*********/

				//Here We Play Clock Puzzle
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -3.0f);
				glBindTexture(GL_TEXTURE_2D, 0);
				DrawClock(gpClockHead);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -3.0f);
				glBindTexture(GL_TEXTURE_2D, 0);
				DrawRing(gpNextClockHand);

				if (iInteract == 1) {
					glBindTexture(GL_TEXTURE_2D, 0);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(-0.150f, 0.080f, -0.25f);
					glScalef(0.05f, 0.05f, 0.0f);
					My_R();
					My_To(-0.13f, -0.080f);
					My_Rotate(-0.160f, -0.050f);

					glBindTexture(GL_TEXTURE_2D, 0);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(0.050f, 0.0800f, -0.25f);
					glScalef(0.05f, 0.05f, 0.0f);
					My_A();
					glTranslatef(0.000f, -0.50f, 0.0f);
					My_D();
					My_To(0.09f, -0.06f);
					My_Move(0.150f, -0.070f);


					glBindTexture(GL_TEXTURE_2D, 0);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					glTranslatef(-0.030f, -0.0700f, -0.25f);
					glScalef(0.05f, 0.05f, 0.0f);
					My_K();
					My_To(-0.0050f, 0.070f);
					My_Key(0.030f, 0.070f);
				}

			}
			else if (iClockPuzzleHint == 1 || iHousePuzzleHint == 1) {

				/**********HINT SCREEN***********/


				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -2.00f);

				glBindTexture(GL_TEXTURE_2D, 0);
				if (iHousePuzzleHint == 1)
					glBindTexture(GL_TEXTURE_2D, Texture_HousePuzzleHint);
				else if (iClockPuzzleHint == 1)
					glBindTexture(GL_TEXTURE_2D, Texture_ClockPuzzleHint);

				glBegin(GL_QUADS);
				glColor3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(1.50f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-1.50f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-1.50f, -1.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(1.50f, -1.0f, 0.0f);
				glEnd();

			}
		}
		else if (fEndCount < 1000.0f) {	//1200
			//For Pause
			ShowCursor(FALSE);
			iEndingCredit = 1;

		}
		else if (fEndCount > 1000.0f) {	//1200


			//Credits and Render Name
			static GLfloat fChangeCount = 0.0f;
			static GLint iSequence = 1;
			switch (iSequence) {
			case 1:
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -2.0f);
				glBindTexture(GL_TEXTURE_2D, 0);
				My_RohitJadhav(-1.30f, 0.0f);
				if (fChangeCount > 200.0f) {
					fChangeCount = 0.0f;
					iSequence = 2;
				}
				else
					fChangeCount = fChangeCount + 0.50f;
				break;

			case 2:
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -2.0f);
				glBindTexture(GL_TEXTURE_2D, 0);
				My_VikasKamble(-1.0f, 0.0f);
				if (fChangeCount > 200.0f) {
					fChangeCount = 0.0f;
					iSequence = 3;
				}
				else
					fChangeCount = fChangeCount + 0.50f;
				break;

			case 3:
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -2.0f);
				glBindTexture(GL_TEXTURE_2D, 0);
				My_Render(-0.90f, 0.0f);
				if (fChangeCount > 300.0f) {
					fChangeCount = 0.0f;
					iSequence = 4;
					//DestroyWindow(ghwnd);
				}
				else
					fChangeCount = fChangeCount + 0.50f;
				break;

			case 4:
				if (fChangeCount > 100.0f) {
					fChangeCount = 0.0f;
					DestroyWindow(ghwnd);
				}
				else
					fChangeCount = fChangeCount + 0.50f;
				break;
			}
		}

	}
	
	if (iEnding == 1)
		fEndCount = fEndCount + 1.0f;
	
	SwapBuffers(ghdc);

}

void update(void) {
	GLfloat fangle = 0.0f;
	static GLfloat fDX = 0.0f;

	if (iEnding == 1 || iSequenceOfDisplay == 0) {
		for (int i = 0; i < World_Matrix_Depth; i++) {
			for (int j = 0; j < 200; j++) {
				Matrix[i][j].y = (GLfloat)((sin(fangle * 0.8f + fDX) - sin(fangle * 0.6 + fDX) - sin(fangle * 0.4f + fDX) + sin(fangle * 2.0f + fDX) + sin(fangle * 0.5f + fDX)) / 15.0f);
			}
			fDX = fDX + 0.0002f;
			fangle = fangle + 0.1f;
		}
	}

}







/**************************************** WORLD PART *********************************************/

void House(GLfloat x, GLfloat y, GLfloat z) {
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 2180; i++) {
		for (int j = 0; j < 3; j++) {
			int vi = HouseFaceIndex[i][j] - 1;
			int vt = HouseFaceIndex[i][j + 3];
			int vn = HouseFaceIndex[i][j + 6];

			glTexCoord2f(HouseTextureData[vt][0], HouseTextureData[vt][1]);

			glNormal3f(HouseNormalData[vn][0], HouseNormalData[vn][1], HouseNormalData[vn][2]);

			glVertex3f(HouseVertexData[vi][0] + x, HouseVertexData[vi][1] + y, HouseVertexData[vi][2] + z);
		}
	}
	glEnd();
}


void Tree(GLfloat x, GLfloat y, GLfloat z) {

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < 14498; i++) {
		for (int j = 0; j < 3; j++) {
			int vi = TreeFaceIndex[i][j] - 1;
			int vt = TreeFaceIndex[i][j + 3];
			int vn = TreeFaceIndex[i][j + 6];

			glTexCoord2f(TreeTextureData[vt][0], TreeTextureData[vt][1]);

			glNormal3f(TreeNormalData[vn][0], TreeNormalData[vn][1], TreeNormalData[vn][2]);

			glVertex3f(TreeVertexData[vi][0] + x, TreeVertexData[vi][1] + y, TreeVertexData[vi][2] + z);
		}
	}
	glEnd();
}


void WorldCalculation(void) {

	GLfloat fBack = -18.0f;
	GLfloat fLeft = -18.0f;

	GLfloat fX = fLeft;
	GLfloat fZ = fBack;

	GLfloat fMaxWidth = 36.0f;
	GLfloat fMaxDepth = 36.0f;


	GLfloat dx = fMaxWidth / (GLfloat)(World_Matrix_Width - 1);
	GLfloat dz = fMaxDepth / (GLfloat)(World_Matrix_Depth - 1);

	fCamDZ = dz;

	for (int i = 0; i < World_Matrix_Depth; i++) {

		for (int j = 0; j < World_Matrix_Width; j++) {

			Matrix[i][j].x = fX;
			Matrix[i][j].y = 0.0f;
			Matrix[i][j].z = fZ;

			fX = fX + dx;
		}
		fZ = fZ + dz;
		fX = fLeft;
	}
}



void PatternTwo_Tri(void) {

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < World_Matrix_Depth - 1; i++) {
		for (int j = 200; j < World_Matrix_Width - 1; j++) {


			//1st Tri
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(Matrix[i][j].x, Matrix[i][j].y, Matrix[i][j].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(Matrix[i + 1][j].x, Matrix[i + 1][j].y, Matrix[i + 1][j].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(Matrix[i][j + 1].x, Matrix[i][j + 1].y, Matrix[i][j + 1].z);

			//2nd Tri
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(Matrix[i][j + 1].x, Matrix[i][j + 1].y, Matrix[i][j + 1].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(Matrix[i + 1][j].x, Matrix[i + 1][j].y, Matrix[i + 1][j].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(Matrix[i + 1][j + 1].x, Matrix[i + 1][j + 1].y, Matrix[i + 1][j + 1].z);


		}
	}

	glEnd();
}



void PatternTwo_Tri_Water(void) {

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);

	for (int i = 0; i < World_Matrix_Depth - 1; i++) {
		for (int j = 0; j < 200; j++) {


			//1st Tri
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(Matrix[i][j].x, Matrix[i][j].y, Matrix[i][j].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(Matrix[i + 1][j].x, Matrix[i + 1][j].y, Matrix[i + 1][j].z);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(Matrix[i][j + 1].x, Matrix[i][j + 1].y, Matrix[i][j + 1].z);

			//2nd Tri
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(Matrix[i][j + 1].x, Matrix[i][j + 1].y, Matrix[i][j + 1].z);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(Matrix[i + 1][j].x, Matrix[i + 1][j].y, Matrix[i + 1][j].z);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(Matrix[i + 1][j + 1].x, Matrix[i + 1][j + 1].y, Matrix[i + 1][j + 1].z);


		}
	}

	glEnd();
}


void GenrateTerrain(void) {

	GLfloat fScale;
	GLfloat fY;
	GLint iSmallLand = 0;
	GLfloat fangle = 0.0f;

	for (int i = 0; i < World_Matrix_Depth; i++) {
		for (int j = 0; j < World_Matrix_Width; j++) {


			if ((i < 55 && j > 201) || (i > 450 && i < 512 && j > 201)) {
				fScale = (GLfloat)(0.50f / RAND_MAX);
				fY = fScale * rand();
				Matrix[i][j].y = (GLfloat)(4.0f * fabs(sin(fangle))) + fY;
			}
			else if ((i > 0 && i < 512 && j > 450 && j < 512)) {
				fScale = (GLfloat)(0.50f / RAND_MAX);
				fY = fScale * rand();
				Matrix[i][j].y = (GLfloat)(4.0f * fabs(sin(fangle))) + fY;
				if (j == 511) {
					fangle = 0.0f;
				}
				else
					fangle = fangle + 0.05f;
			}
			else if (i > 55 && i < 512 && j > 201) {
				fScale = (GLfloat)(0.1f / RAND_MAX);
				fY = fScale * rand();
				Matrix[i][j].y = 1.0f + fY;

			}
		}
		fangle = fangle + 0.05f;

	}
}


void LoadHouseData(void) {

	int My_strtok(char*, char);

	char line[4096];

	char *firstToken;
	char *secTok;
	char *cContext = NULL;

	GLint vertex = 0;
	GLint texture = 0;
	GLint normal = 0;

	int c = 0;

	static int iVertex = 0;
	static int iVertexTex = 0;
	static int iVertexNor = 0;
	static int iFace = 0;

	while (fgets(line, sizeof(line), gpHouse) != NULL) {
		firstToken = strtok_s(line, " ", &cContext);

		if (strcmp(firstToken, "v") == 0) {

			for (int i = 0; i < 3; i++) {
				secTok = strtok_s(NULL, " ", &cContext);
				HouseVertexData[iVertex][i] = (GLfloat)atof(secTok);
			}
			iVertex++;
		}
		else if (strcmp(firstToken, "vt") == 0) {

			for (int i = 0; i < 2; i++) {
				secTok = strtok_s(NULL, " ", &cContext);
				HouseTextureData[iVertexTex][i] = (GLfloat)atof(secTok);
			}
			iVertexTex++;
		}
		else if (strcmp(firstToken, "vn") == 0) {
			for (int i = 0; i < 3; i++) {
				secTok = strtok_s(NULL, " ", &cContext);
				HouseNormalData[iVertexTex][i] = (GLfloat)atof(secTok);
			}
			iVertexNor++;
		}
		else if (strcmp(firstToken, "f") == 0) {

			for (int i = 0; i < 4; i++) {
				secTok = strtok_s(NULL, " ", &cContext);

				vertex = My_strtok(secTok, '/');
				texture = My_strtok(secTok, '/');
				normal = My_strtok(secTok, '/');
				if (i < 3) {
					HouseFaceIndex[iFace][i] = vertex;
					HouseFaceIndex[iFace][i + 3] = texture;
					HouseFaceIndex[iFace][i + 6] = normal;
				}
				else if (i == 3) {
					iFace = iFace + 1;
					for (int j = 0; j < 3; j++) {
						if (j == 0) {
							HouseFaceIndex[iFace][j] = HouseFaceIndex[iFace - 1][j];
							HouseFaceIndex[iFace][j + 3] = HouseFaceIndex[iFace - 1][j + 3];
							HouseFaceIndex[iFace][j + 6] = HouseFaceIndex[iFace - 1][j + 6];
						}
						else if (j == 1) {
							HouseFaceIndex[iFace][j] = HouseFaceIndex[iFace - 1][j + 1];
							HouseFaceIndex[iFace][j + 3] = HouseFaceIndex[iFace - 1][j + 1 + 3];
							HouseFaceIndex[iFace][j + 6] = HouseFaceIndex[iFace - 1][j + 1 + 6];
						}
						else if (j == 2) {
							HouseFaceIndex[iFace][j] = vertex;
							HouseFaceIndex[iFace][j + 3] = texture;
							HouseFaceIndex[iFace][j + 6] = normal;
						}
					}
				}

			}
			iFace++;
		}
	}

}


void LoadTreeData(void) {

	int My_strtok(char*, char);

	char line[4096];

	char *firstToken;
	char *secTok;
	char *cContext = NULL;

	GLint vertex = 0;
	GLint texture = 0;
	GLint normal = 0;

	int c = 0;

	static int iVertex = 0;
	static int iVertexTex = 0;
	static int iVertexNor = 0;
	static int iFace = 0;

	while (fgets(line, sizeof(line), gpTree) != NULL) {
		firstToken = strtok_s(line, " ", &cContext);

		if (strcmp(firstToken, "v") == 0) {

			for (int i = 0; i < 3; i++) {
				secTok = strtok_s(NULL, " ", &cContext);
				TreeVertexData[iVertex][i] = (GLfloat)atof(secTok);
			}
			iVertex++;
		}
		else if (strcmp(firstToken, "vt") == 0) {

			for (int i = 0; i < 2; i++) {
				secTok = strtok_s(NULL, " ", &cContext);
				TreeTextureData[iVertexTex][i] = (GLfloat)atof(secTok);
			}
			iVertexTex++;
		}
		else if (strcmp(firstToken, "vn") == 0) {
			for (int i = 0; i < 3; i++) {
				secTok = strtok_s(NULL, " ", &cContext);
				TreeNormalData[iVertexTex][i] = (GLfloat)atof(secTok);
			}
			iVertexNor++;
		}
		else if (strcmp(firstToken, "f") == 0) {

			for (int i = 0; i < 4; i++) {
				secTok = strtok_s(NULL, " ", &cContext);

				vertex = My_strtok(secTok, '/');
				texture = My_strtok(secTok, '/');
				normal = My_strtok(secTok, '/');
				if (i < 3) {
					TreeFaceIndex[iFace][i] = vertex;
					TreeFaceIndex[iFace][i + 3] = texture;
					TreeFaceIndex[iFace][i + 6] = normal;
				}
				else if (i == 3) {
					iFace = iFace + 1;
					for (int j = 0; j < 3; j++) {
						if (j == 0) {
							TreeFaceIndex[iFace][j] = TreeFaceIndex[iFace - 1][j];
							TreeFaceIndex[iFace][j + 3] = TreeFaceIndex[iFace - 1][j + 3];
							TreeFaceIndex[iFace][j + 6] = TreeFaceIndex[iFace - 1][j + 6];
						}
						else if (j == 1) {
							TreeFaceIndex[iFace][j] = TreeFaceIndex[iFace - 1][j + 1];
							TreeFaceIndex[iFace][j + 3] = TreeFaceIndex[iFace - 1][j + 1 + 3];
							TreeFaceIndex[iFace][j + 6] = TreeFaceIndex[iFace - 1][j + 1 + 6];
						}
						else if (j == 2) {
							TreeFaceIndex[iFace][j] = vertex;
							TreeFaceIndex[iFace][j + 3] = texture;
							TreeFaceIndex[iFace][j + 6] = normal;
						}
					}
				}

			}
			iFace++;
		}
	}


}

int My_strtok(char *str, char delim) {

	static int i = 0;
	int j = 0;
	int c;
	char buffer[64];

	while ((c = str[i++]) != delim && c != '\0') {
		buffer[j++] = c;
	}
	buffer[j] = '\0';

	if (c == '\0')
		i = 0;

	return(atoi(buffer));
}

int CheckForHouse(GLfloat x, GLfloat z) {
	int i = 0;

	iInteract = 0;
	iClockPuzzle = 0;
	iHousePuzzle = 0;
	iDoor = 0;
	if ((x > Matrix[215][325].x && x < Matrix[215][420].x) &&
		((z > Matrix[282][325].z && z < Matrix[284][325].z) ||
		(z > Matrix[276][325].z && z < Matrix[278][325].z))) {
		//Right Side Wall Front and Back
		i++;
	}
	else if (((x > Matrix[264][326].x && x < Matrix[264][328].x) ||
		(x > Matrix[264][334].x && x < Matrix[264][336].x)) &&
		((z > Matrix[208][325].z && z < Matrix[260][325].z) ||
		(z > Matrix[266][325].z && z < Matrix[282][325].z))) {
		//Front Walls Front and Back
		i++;
	}
	else if (((x > Matrix[264][326].x && x < Matrix[264][328].x) ||
		(x > Matrix[264][334].x && x < Matrix[264][336].x)) && 
		(z > Matrix[260][325].z && z < Matrix[265][325].z)) {
		//For Door
		i++;
		iInteract = 1;
		iDoor = 1;
	}
	else if ((z > Matrix[208][420].z && z < Matrix[282][420].z) &&
		((x > Matrix[208][422].x && x < Matrix[208][424].x) ||
		((x > Matrix[208][415].x && x < Matrix[208][417].x)))) {
		//Back Wall Front and Back
		
		i++;
	}
	else if ((x > Matrix[208][325].x && x < Matrix[208][400].x) &&
		((z > Matrix[212][325].z && z < Matrix[214][325].z) ||
		(z > Matrix[205][325].z && z < Matrix[207][325].z))) {
		//LeftWall 1 Front and Back
		
		i++;
	}
	else if ((x > Matrix[208][400].x && x < Matrix[208][420].x) &&
		(z > Matrix[216][400].z && z < Matrix[218][400].z)) {
		//LeftWall 2 With House Puzzle 1 Front 

		//For Telling You Can able to Play Puzzle1
		iHousePuzzle = 1;
		iInteract = 1;
		i++;
	}
	else if ((x > Matrix[208][400].x && x < Matrix[208][420].x) &&
		(z > Matrix[205][400].z && z < Matrix[207][400].z)) {
		//Left Wall 2 With House Puzzle 1 Back
	
		i++;
	}
	else if ((z > Matrix[208][360].z && z < Matrix[268][360].z) &&
		((x > Matrix[208][358].x && x < Matrix[208][360].x) ||
		(x > Matrix[208][364].x && x < Matrix[208][366].x))) {
		//1st Internal Wall Front and Back
		
		i++;
	}
	else if ((z > Matrix[208][390].z && z < Matrix[268][390].z) &&
		((x > Matrix[208][388].x && x < Matrix[208][390].x) ||
		(x > Matrix[208][394].x && x < Matrix[208][396].x))) {
		//2st Internal Wall Front and Back
		
		i++;
	}
	else if ((x > Matrix[265][355].x && x < Matrix[265][383].x) &&
		(z > Matrix[265][355].z && z < Matrix[267][355].z)) {
		//Puzzle 2 Back
		
		i++;
	}
	else if ((x > Matrix[265][360].x && x < Matrix[265][383].x) &&
		(z > Matrix[254][355].z && z < Matrix[256][355].z)) {
		//Clock Puzzle 2 Front
		iClockPuzzle = 1;
		iInteract = 1;
		i++;
	}
	return(i);
}

int CheckForWaterHills(GLfloat x, GLfloat z) {
	int i = 0;

	if (x > Matrix[55][201].x && z < Matrix[60][201].z) {
		//Left Hill
		i++;
	}
	else if (x > Matrix[450][201].x && z > Matrix[448][201].z && z < Matrix[450][201].z) {
		//Right Hill
		i++;
	}
	else if (x > Matrix[55][446].x) {
		//Back Hill
		i++;
	}
	else if (x < Matrix[55][210].x && iEnding == 1) {
		//Water End
		i++;
	}
	return(i);
}

void RotateDirections(GLfloat angle) {

	Direction[0].x = (GLfloat)((fCamDZ * cos(3 * M_PI / 2.0f + angle)) + fEyeX);
	Direction[0].z = (GLfloat)((fCamDZ * sin(3 * M_PI / 2.0f + angle)) + fEyeZ);


	Direction[1].x = (GLfloat)((fCamDZ * cos(angle)) + fEyeX);
	Direction[1].z = (GLfloat)((fCamDZ * sin(angle)) + fEyeZ);


	Direction[2].x = (GLfloat)((fCamDZ * cos(M_PI / 2.0f + angle)) + fEyeX);
	Direction[2].z = (GLfloat)((fCamDZ * sin(M_PI / 2.0f + angle)) + fEyeZ);


	Direction[3].x = (GLfloat)((fCamDZ * cos(M_PI + angle)) + fEyeX);
	Direction[3].z = (GLfloat)((fCamDZ * sin(M_PI + angle)) + fEyeZ);
}


void My_Cube(GLfloat X1, GLfloat X2, GLfloat Z1, GLfloat Z2, GLfloat Y1, GLfloat Y2) {

	glBegin(GL_QUADS);

	//Top
	glTexCoord2f(X2, 0.0f);
	glVertex3f(X1, Y1, Z2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(X2, Y1, Z2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(X2, Y1, Z1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(X1, Y1, Z1);

	//Bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(X1, Y2, Z2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(X2, Y2, Z2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(X2, Y2, Z1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(X1, Y2, Z1);

	//Face
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(X1, Y1, Z1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(X2, Y1, Z1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(X2, Y2, Z1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(X1, Y2, Z1);

	//Back
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(X2, Y1, Z2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(X1, Y1, Z2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(X1, Y2, Z2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(X2, Y2, Z2);

	//Right
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(X1, Y1, Z2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(X1, Y1, Z1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(X1, Y2, Z1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(X1, Y2, Z2);

	//Left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(X2, Y1, Z1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(X2, Y1, Z2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(X2, Y2, Z2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(X2, Y2, Z1);

	glEnd();
}










/********************************* HOUSE PUZZLE PART ********************************************/

void DisplayHouse(void) {

	struct PointInfo *Temp = gpHeadHouse->next;

	//fprintf(gbFile, "In DisplayHouse!!\n");

	while (Temp) {

		GLfloat X = Temp->x;
		GLfloat Y = Temp->y;
		GLfloat XNext = Temp->x + fHouseDX;
		GLfloat YNext = Temp->y - fHouseDY;

		glBindTexture(GL_TEXTURE_2D, Texture_HousePuzzle[Temp->iPosition - 1]);

		glBegin(GL_QUADS);
		if (Temp->iOrientation == 1) {
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(XNext, Y, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(X, Y, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(X, YNext, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(XNext, YNext, 0.0f);
		}
		else if (Temp->iOrientation == 2) {
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(XNext, Y, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(X, Y, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(X, YNext, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(XNext, YNext, 0.0f);
		}
		else if (Temp->iOrientation == 3) {
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(XNext, Y, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(X, Y, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(X, YNext, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(XNext, YNext, 0.0f);
		}
		else if (Temp->iOrientation == 4) {
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(XNext, Y, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(X, Y, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(X, YNext, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(XNext, YNext, 0.0f);
		}

		glEnd();

		Temp = Temp->next;

		//(gbFile, "End While!!\n");
	}
	//fprintf(gbFile, "End DH()\n");
}

void DeleteHouse(void) {

	struct PointInfo *Temp = gpHeadHouse->next;
	struct PointInfo *Prev = gpHeadHouse;
	fprintf(gbHousePuzzle, "\n\n");

	while (Temp) {
		while (Temp->next != NULL) {
			Prev = Temp;
			Temp = Temp->next;
		}

		fprintf(gbHousePuzzle, "Node Deleted!!\n");

		Prev->next = NULL;
		free(Temp);

		Temp = gpHeadHouse->next;
		Prev = gpHeadHouse;
	}
}

void SquareFrame(void) {

	GLfloat X = gpHouseMovement->x;
	GLfloat Y = gpHouseMovement->y;
	GLfloat XNext = gpHouseMovement->x + fHouseDX;
	GLfloat YNext = gpHouseMovement->y - fHouseDY;

	glBindTexture(GL_TEXTURE_2D, 0);
	glLineWidth(3.0f);
	glBegin(GL_LINE_LOOP);

	//glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(XNext, Y, 0.0f);
	glVertex3f(X, Y, 0.0f);
	glVertex3f(X, YNext, 0.0f);
	glVertex3f(XNext, YNext, 0.0f);

	glEnd();
	
}


void HouseCalculation(GLfloat *Dx, GLfloat *Dy) {

	struct PointInfo *Temp = gpHeadHouse->next;
	int iPos = 1;

	GLfloat fTop = 1.0f;
	GLfloat fLeft = -1.0f;

	GLfloat fX = fLeft;
	GLfloat fY = fTop;

	GLfloat fMaxWidth = 2.0f;
	GLfloat fMaxHeight = 2.0f;


	GLfloat dx = fMaxWidth / (GLfloat)(HousePuzzle_Matrix_Width - 1);
	GLfloat dy = fMaxHeight / (GLfloat)(HousePuzzle_Matrix_Height - 1);

	*Dx = dx;
	*Dy = dy;

	while (Temp) {
		if (iPos <= 3) {
			Temp->x = fX;
			Temp->y = fY;

			Temp = Temp->next;
			iPos++;
			fX = fX + dx;
		}

		if (iPos > 3) {
			iPos = 1;
			fX = fLeft;
			fY = fY - dy;
		}
	}
}


void AddHouseElement() {

	static int i = 1;

	struct PointInfo *Temp = gpHeadHouse;
	struct PointInfo *toAdd = (struct PointInfo*)malloc(sizeof(struct PointInfo));
	if (toAdd == NULL) {
		fprintf(gbHousePuzzle, "Error Creating Node!!\n");
		uninitialize();
		exit(0);
	}

	toAdd->next = NULL;
	toAdd->iPosition = i;
	i++;

	while (Temp) {
		if (Temp->next == NULL) {
			Temp->next = toAdd;
			fprintf(gbHousePuzzle, "Node Added!!\n");
			break;
		}
		else {
			Temp = Temp->next;
		}
	}
}

void CreateHouseLinkedList(void) {

	void AddHouseElement(void);

	for (int i = 0; i < 9; i++) {
		AddHouseElement();
	}
}


void GenrateRandomOrientation(void) {

	int RandomNoGen(int, int);

	int random = 5;
	int i = 1;
	struct PointInfo *Temp = gpHeadHouse->next;


	while (Temp) {

		random = RandomNoGen(1, 4);

		Temp->iOrientation = random;
		fprintf(gbHousePuzzle, "LinkedList i: %d Random Number: %d  Position: %d\n", i, Temp->iOrientation, Temp->iPosition);
		i++;
		Temp = Temp->next;
	}
}

int RandomNoGen(int iStart, int iEnd) {
	int No;

	No = (rand() % iEnd + 1);

	if (No < iStart)
		No = iStart;
	else if (No == iEnd)
		No = iEnd;
	return(No);
}


int CheckHousePuzzleAnswer(struct PointInfo *temp) {
	int iCount = 0;

	while (temp) {
		if (temp->iOrientation == 1)
			iCount++;
		temp = temp->next;
	}

	return(iCount);
}





/*************************************  CLOCK PUZZLE   *************************************/


void AddNode(struct PointInfo *pHead) {

	static int i = 1;

	struct PointInfo *temp = gpEnd;
	struct PointInfo *toAdd = (struct PointInfo*)malloc(sizeof(struct PointInfo));

	if (pHead->next == NULL) {
		pHead->next = toAdd;
		gpStart = toAdd;
		gpEnd = toAdd;
		toAdd->next = gpStart;
		toAdd->prev = gpEnd;
		fprintf(gbClockPuzzle, "%d Node Added!!\n", i);
	}
	else {
		if (temp->next == gpStart) {
			temp->next = toAdd;
			toAdd->next = gpStart;
			toAdd->prev = temp;
			gpEnd = toAdd;
			gpStart->prev = gpEnd;
			fprintf(gbClockPuzzle, "%d Node Added!!\n", i);
		}
	}
	//fprintf(gbFile, "%d Node Added!!\n", i);
	i++;
}

void CreateClockLinkedList(struct PointInfo *pHead, int iNumberOfNodes) {

	void AddNode(struct PointInfo*);

	for (int i = 0; i < iNumberOfNodes; i++) {
		AddNode(pHead);
	}
}

void FillClockLinkedListData(struct PointInfo *pHead) {

	int RandomNoGen(int, int);

	struct PointInfo *temp = pHead->next;

	GLfloat fLeft = -1.450f;

	GLfloat fX = fLeft;
	GLfloat fY = 0.0f;

	GLfloat Max_Width = 4.0f;
	GLfloat Max_Height = 0.5f;
	GLint iNumberOfCuts = 4;

	GLfloat dx = (GLfloat)(Max_Width / (GLfloat)iNumberOfCuts);

	for (int i = 0; i < iNumberOfCuts; i++) {
		temp->x = fX;
		temp->y = fY;
		temp->iPosition = i + 1;
		temp->iClockHandPosition = RandomNoGen(1, 12);
		temp->fRadius = 0.4f;
		temp->fLengthOfHand = 0.30f;
		fX = fX + dx;

		fprintf(gbClockPuzzle, "\n");
		fprintf(gbClockPuzzle, "x: %f\n", temp->x);
		fprintf(gbClockPuzzle, "y: %f\n", temp->y);
		fprintf(gbClockPuzzle, "iPosition : %d\n", temp->iPosition);
		fprintf(gbClockPuzzle, "iOrientaion : %d\n", temp->iClockHandPosition);
		fprintf(gbClockPuzzle, "Radius: %f\n", temp->fRadius);
		fprintf(gbClockPuzzle, "Len of Hand: %f\n", temp->fLengthOfHand);

		temp = temp->next;
	}
}


void DrawClock(struct PointInfo *pHead) {

	struct PointInfo *temp = pHead->next;
	GLfloat angle = 0.0f;
	GLfloat angleHand = 0.0f;
	int i = 1;

	while (temp) {
		while (angle <= 360.0f) {

			GLfloat X = (GLfloat)(temp->fRadius * cos(angle) + temp->x);
			GLfloat Y = (GLfloat)(temp->fRadius * sin(angle) + temp->y);
			glPointSize(15.0f);
			glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(X, Y, 0.0f);
			glEnd();

			angle = angle + 1.0f;
		}

		angleHand = (GLfloat)((-M_PI / 6.0f) * temp->iClockHandPosition) + (GLfloat)(M_PI / 2.0f);

		GLfloat lX = (GLfloat)(temp->fLengthOfHand * cos(angleHand) + temp->x);
		GLfloat lY = (GLfloat)(temp->fLengthOfHand * sin(angleHand) + temp->y);

		glLineWidth(4.0f);
		glBegin(GL_LINES);

		glVertex3f(temp->x, temp->y, 0.0f);
		glVertex3f(lX, lY, 0.0f);

		glEnd();

		temp = temp->next;

		angle = 0.0f;

		if (i <= 4)
			i++;
		else
			break;
	}
}

void DrawRing(struct PointInfo *temp) {
	GLfloat angle = 0.0f;

	while (angle <= 360.0f) {

		GLfloat X = (GLfloat)(temp->fRadius * cos(angle) + temp->x);
		GLfloat Y = (GLfloat)(temp->fRadius * sin(angle) + temp->y);

		glPointSize(15.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(X, Y, 0.0f);
		glEnd();

		angle = angle + 1.0f;
	}
}

int CheckClockPuzzleAnswer(struct PointInfo *temp) {

	int iCount = 0;
	//GLuint Answer[4];

	for (int i = 0; i < 4; i++) {
		if (temp->iClockHandPosition == ClockAnswer[i]) {
			//Answer[i] = 1;
			iCount = iCount + 1;
		}
		temp = temp->next;
	}

	/*for (int i = 0; i < 4; i++) {
		if (Answer[i] == 1)
			iCount++;
	}*/
	fprintf(gbClockPuzzle, "\n");
	fprintf(gbClockPuzzle, "iCount: %d\n", iCount);

	return(iCount);
}


void DeleteClockLinkedList(void) {

	//struct PointInfo *temp = gpClockHead->next;
	struct PointInfo *temp = gpEnd;
	fprintf(gbClockPuzzle, "\n\n");
	while (temp) {
		if (gpStart == gpEnd) {
			temp->next = NULL;
			temp->prev = NULL;
			free(temp);
			temp = NULL;
			gpStart = gpEnd = NULL;
			fprintf(gbClockPuzzle, "Node Deleted!!\n");
		}
		else if (temp->next == gpStart && gpStart != gpEnd) {
			temp->next = NULL;
			temp->prev->next = gpStart;
			gpEnd = temp->prev;
			temp->prev = NULL;
			free(temp);
			//temp = gpClockHead->next;
			temp = gpEnd;
			fprintf(gbClockPuzzle, "Node Deleted!!\n");
		}
		else
			temp = temp->next;
	}

}








/********************************************** DEATH Clock **********************************************/