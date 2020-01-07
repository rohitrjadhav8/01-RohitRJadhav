#include<Windows.h>
#include"MyResource.h"
#include<math.h>
#include<Mmsystem.h>

#pragma comment(lib,"Winmm.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI Race(LPVOID);
DWORD WINAPI Group_Name(LPVOID);
DWORD WINAPI Our_Name(LPVOID);

int Block(HDC, int);
int BlockMove(HDC, RECT*, int, double, int, int);
int Road(HDC, int, int, int);
int Grass(HDC hdc, int iX, int iY, int iFlag);
int hInst;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szName[] = TEXT("Race");

	hInst = hInstance;
	wndclass.lpszClassName = szName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszMenuName = NULL;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;

	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(Myicon));
	wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(Myicon));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_HAND);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szName,
		TEXT("Race"),
		WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		HWND_DESKTOP,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}
static HANDLE hThreadR = NULL;
static HANDLE hThreadS = NULL;
static HANDLE hThreadE = NULL;


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	TCHAR szStr[] = TEXT("FOR");

	switch (iMsg) {
	case WM_CREATE:
		//Car
		hThreadR = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Race,
			(LPVOID)hwnd,
			CREATE_SUSPENDED,
			NULL);

		if (hThreadR == NULL) {
			MessageBox(hwnd, TEXT("Thread Creation Failed"), TEXT("Error"), MB_OK | MB_ICONERROR);
			DestroyWindow(hwnd);
		}
		
		// Starting Thread
		hThreadS = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Group_Name,
			(LPVOID)hwnd,
			0,
			NULL);
		if (hThreadS == NULL) {
			MessageBox(hwnd, TEXT("Start Thread Creation Failed!!"), TEXT("Error"), MB_OK | MB_ICONERROR);
			DestroyWindow(hwnd);
		}


		//Ending Thread
		hThreadE = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Our_Name,
			(LPVOID)hwnd,
			CREATE_SUSPENDED,
			NULL);
		if (hThreadE == NULL) {
			MessageBox(hwnd, TEXT("End Thread Creation Failed!!"), TEXT("Error"), MB_OK | MB_ICONERROR);
			DestroyWindow(hwnd);
		}

	
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		SelectObject(hdc, hBrush);
		Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		DeleteObject(hBrush);
		EndPaint(hwnd, &ps);
		break;


	case WM_DESTROY:
		CloseHandle(hThreadS);
		CloseHandle(hThreadR);
		CloseHandle(hThreadE);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}



int Block(HDC hdc, int iBye) {
	POINT pt1[5];
	POINT pt2[4];
	POINT pt3[5];
	POINT pt4[2];
	POINT pt5[2];
	POINT pt6[2];
	POINT pt7[2];

	//Block
	pt1[0].x = 500 + 30 + iBye / 3;
	pt1[0].y = 700 - 20 - iBye * 3;
	pt1[1].x = 500 + 30 + iBye / 3;
	pt1[1].y = 625 - 20 - iBye * 3;
	pt1[2].x = 800 + 30 - iBye / 3;
	pt1[2].y = 625 - 20 - iBye * 3;
	pt1[3].x = 800 + 30 - iBye / 3;
	pt1[3].y = 700 - 20 - iBye * 3;
	pt1[4].x = 500 + 30 + iBye / 3;
	pt1[4].y = 700 - 20 - iBye * 3;


	//OuterBlock
	pt2[0].x = 500 + 30 + iBye / 3;
	pt2[0].y = 625 - 20 - iBye * 3;
	pt2[1].x = 500 + 30 + iBye / 3;
	pt2[1].y = 300 - 20 - iBye * 3;
	pt2[2].x = 800 + 30 - iBye / 3;
	pt2[2].y = 300 - 20 - iBye * 3;
	pt2[3].x = 800 + 30 - iBye / 3;
	pt2[3].y = 625 - 20 - iBye * 3;


	// TOP
	pt3[0].x = 530 + 30 + iBye / 3;
	pt3[0].y = 575 - 20 - iBye * 3;
	pt3[1].x = 530 + 30 + iBye / 3;
	pt3[1].y = 370 - 20 - iBye * 3;
	pt3[2].x = 770 + 30 - iBye / 3;
	pt3[2].y = 370 - 20 - iBye * 3;
	pt3[3].x = 770 + 30 - iBye / 3;
	pt3[3].y = 575 - 20 - iBye * 3;
	pt3[4].x = 530 + 30 + iBye / 3;
	pt3[4].y = 575 - 20 - iBye * 3;


	pt4[0].x = 500 + 30 + iBye / 3;
	pt4[0].y = 625 - 20 - iBye * 3;
	pt4[1].x = pt3[0].x;
	pt4[1].y = pt3[0].y;

	pt5[0].x = 500 + 30 + iBye / 3;
	pt5[0].y = 350 - 20 - iBye * 3;
	pt5[1].x = pt3[1].x;
	pt5[1].y = pt3[1].y;

	pt6[0].x = 800 + 30 - iBye / 3;
	pt6[0].y = 350 - 20 - iBye * 3;
	pt6[1].x = pt3[2].x;
	pt6[1].y = pt3[2].y;

	pt7[0].x = 800 + 30 - iBye / 3;
	pt7[0].y = 625 - 20 - iBye * 3;
	pt7[1].x = pt3[3].x;
	pt7[1].y = pt3[3].y;

	Polyline(hdc, pt1, 5);
	Polyline(hdc, pt2, 4);
	Polyline(hdc, pt3, 5);
	Polyline(hdc, pt4, 2);
	Polyline(hdc, pt5, 2);
	Polyline(hdc, pt6, 2);
	Polyline(hdc, pt7, 2);

	return(0);
}


int BlockMove(HDC hdc, RECT *rc, int iLen, double dAngle, int iFlag, int iTilt) {
	int iBCX = iLen * cos(dAngle);
	int iBSY = iLen * sin(dAngle);


	int iRCX = 205 * cos(dAngle);
	int iRSY = 205 * sin(dAngle);

	int iPCX = 275 * cos(dAngle);
	int iPSY = 275 * sin(dAngle);

	int iXY[3][2];
	int iPT[2][2];
	POINT ptOuter[5];
	POINT ptTop[5];

	if (iFlag == 1) {
		iXY[0][0] = rc->left - iTilt;
		iXY[0][1] = rc->top - iTilt;
		iXY[1][0] = rc->right - iTilt;
		iXY[1][1] = rc->top - iTilt;
		iXY[2][0] = rc->right - iTilt;
		iXY[2][1] = rc->bottom - iTilt;

		//Top
		ptTop[0].x = 530 + 30 - iTilt * 2;
		ptTop[0].y = 575 - 20 - iTilt * 2;
		ptTop[1].x = ptTop[0].x + iRCX;
		ptTop[1].y = ptTop[0].y - iRSY;
		ptTop[3].x = 770 + 30 - iTilt * 2;
		ptTop[3].y = 575 - 20 - iTilt * 2;
		ptTop[2].x = ptTop[3].x + iRCX;
		ptTop[2].y = ptTop[3].y - iRSY;
		ptTop[4].x = 530 + 30 - iTilt * 2;
		ptTop[4].y = 575 - 20 - iTilt * 2;

	}
	else {

		iXY[0][0] = rc->right + iTilt;
		iXY[0][1] = rc->top - iTilt;
		iXY[1][0] = rc->left + iTilt;
		iXY[1][1] = rc->top - iTilt;
		iXY[2][0] = rc->left + iTilt;
		iXY[2][1] = rc->bottom - iTilt;

		//Top
		ptTop[0].x = 770 + 30 + iTilt * 2;
		ptTop[0].y = 575 - 20 - iTilt * 2;
		ptTop[1].x = ptTop[0].x + iRCX;
		ptTop[1].y = ptTop[0].y - iRSY;
		ptTop[3].x = 530 + 30 + iTilt * 2;
		ptTop[3].y = 575 - 20 - iTilt * 2;
		ptTop[2].x = ptTop[3].x + iRCX;
		ptTop[2].y = ptTop[3].y - iRSY;
		ptTop[4].x = 770 + 30 + iTilt * 2;
		ptTop[4].y = 575 - 20 - iTilt * 2;


	}

	//Point
	iPT[0][0] = iXY[0][0] + iPCX;
	iPT[0][1] = iXY[0][1] - iPSY;
	iPT[1][0] = iXY[1][0] + iPCX;
	iPT[1][1] = iXY[1][1] - iPSY;



	//Block
	ptOuter[0].x = iXY[0][0];
	ptOuter[0].y = iXY[0][1];

	ptOuter[1].x = iXY[0][0] + iBCX;
	ptOuter[1].y = iXY[0][1] - iBSY;
	ptOuter[2].x = iXY[1][0] + iBCX;
	ptOuter[2].y = iXY[1][1] - iBSY;
	ptOuter[3].x = iXY[2][0] + iBCX;
	ptOuter[3].y = iXY[2][1] - iBSY;

	ptOuter[4].x = iXY[2][0];
	ptOuter[4].y = iXY[2][1];



	//Drawing
	if (iFlag == 1)
		Rectangle(hdc, rc->left - iTilt, rc->top - iTilt, rc->right - iTilt, rc->bottom - iTilt);
	else
		Rectangle(hdc, rc->left + iTilt, rc->top - iTilt, rc->right + iTilt, rc->bottom - iTilt);

	Polyline(hdc, ptOuter, 5);
	Polyline(hdc, ptTop, 5);
	MoveToEx(hdc, iXY[1][0], iXY[1][1], NULL);
	LineTo(hdc, ptOuter[2].x, ptOuter[2].y);

	MoveToEx(hdc, ptTop[0].x, ptTop[0].y, NULL);
	LineTo(hdc, iXY[0][0], iXY[0][1]);
	MoveToEx(hdc, ptTop[3].x, ptTop[3].y, NULL);
	LineTo(hdc, iXY[1][0], iXY[1][1]);

	MoveToEx(hdc, ptTop[1].x, ptTop[1].y, NULL);
	LineTo(hdc, iPT[0][0], iPT[0][1]);
	MoveToEx(hdc, ptTop[2].x, ptTop[2].y, NULL);
	LineTo(hdc, iPT[1][0], iPT[1][1]);

	if (iFlag == 1) {
		Ellipse(hdc, iXY[1][0] + iTilt, iXY[1][1] - iTilt, iXY[2][0] + iTilt * 2, iXY[2][1]);
		Ellipse(hdc, ptOuter[3].x - 1 - iTilt, ptOuter[3].y, ptOuter[3].x, ptOuter[3].y + 65 + iTilt);

	}
	else {
		Ellipse(hdc, iXY[1][0] - iTilt, iXY[1][1] - iTilt, iXY[2][0] - iTilt * 2, iXY[2][1]);
		Ellipse(hdc, ptOuter[3].x + 1 + iTilt, ptOuter[3].y, ptOuter[3].x, ptOuter[3].y + 65 + iTilt);
	}
	return(0);
}


int Grass(HDC hdc, int iX, int iY, int iFlag) {
	POINT pt[10];
	HPEN hP = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HBRUSH hB = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hdc, hP);
	SelectObject(hdc, hB);

	if (iFlag == 1) {
		pt[0].x = iX - 60;
		pt[0].y = iY;

		pt[1].x = iX - 50;
		pt[1].y = iY + 20;

		pt[2].x = iX - 60;
		pt[2].y = iY + 40;

		pt[3].x = iX - 90;
		pt[3].y = iY + 40;

		pt[4].x = iX - 100;
		pt[4].y = iY + 20;

		pt[5].x = iX - 90;
		pt[5].y = iY;

		pt[6].x = iX - 82;
		pt[6].y = iY + 20;

		pt[7].x = iX - 75;
		pt[7].y = iY;

		pt[8].x = iX - 66;
		pt[8].y = iY + 20;

		pt[9].x = iX - 60;
		pt[9].y = iY;


		Polygon(hdc, pt, 10);

	}
	else {
		pt[0].x = iX + 60;
		pt[0].y = iY;

		pt[1].x = iX + 50;
		pt[1].y = iY + 20;

		pt[2].x = iX + 60;
		pt[2].y = iY + 40;

		pt[3].x = iX + 90;
		pt[3].y = iY + 40;

		pt[4].x = iX + 100;
		pt[4].y = iY + 20;

		pt[5].x = iX + 90;
		pt[5].y = iY;

		pt[6].x = iX + 82;
		pt[6].y = iY + 20;

		pt[7].x = iX + 75;
		pt[7].y = iY;

		pt[8].x = iX + 66;
		pt[8].y = iY + 20;

		pt[9].x = iX + 60;
		pt[9].y = iY;


		Polygon(hdc, pt, 10);


	}
	hP = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
	hB = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hP);
	SelectObject(hdc, hB);
	return(0);
}

int iX1 = 450;
int iX2 = 342;
int iX3 = 172;

int iY1 = 0;
int iY2 = 175;
int iY3 = 438;

int iX4 = 910;
int iX5 = 1022;
int iX6 = 1193;



int Road(HDC hdc, int iCurve, int iFlag, int iGrass) {
	POINT pt1[4];
	POINT pt2[4];
	static int i = 0;
	static int j = 0;

	if (iFlag == 1) {
		pt1[0].x = 455 + iCurve + iCurve;
		pt1[0].y = 0;
		pt1[1].x = 342;
		pt1[1].y = 175;
		pt1[2].x = 172 - iCurve;
		pt1[2].y = 438;
		pt1[3].x = 0;
		pt1[3].y = 702;

		pt2[0].x = 910 + iCurve + iCurve;
		pt2[0].y = 0;
		pt2[1].x = 1022;
		pt2[1].y = 175;
		pt2[2].x = 1193 - (iCurve + iCurve);
		pt2[2].y = 438;
		pt2[3].x = 1365;
		pt2[3].y = 702;

		PolyBezier(hdc, pt1, 4);
		PolyBezier(hdc, pt2, 4);
		if (iCurve == 0) {
			if (i % 3 == 0) {
				Grass(hdc, pt1[0].x, pt1[0].y, 1);
				Grass(hdc, pt2[1].x, pt2[1].y, 2);
			}
			else if (i % 3 == 1) {
				Grass(hdc, pt1[1].x, pt1[1].y, 1);
				Grass(hdc, pt2[2].x, pt2[2].y, 2);
			}
			else if (i % 3 == 2) {
				Grass(hdc, pt1[2].x, pt1[2].y, 1);
				Grass(hdc, pt2[0].x, pt2[0].y, 2);
			}
			i++;
		}
		else {
			if (j % 3 == 0) {
				Grass(hdc, iX1 + iCurve, iY1, 1);
				Grass(hdc, iX5, iY2, 2);
			}
			else if (j % 3 == 1) {
				Grass(hdc, iX2 + iCurve / 2, iY2, 1);
				Grass(hdc, iX6 - iCurve, iY3, 2);
			}
			else if (j % 3 == 2) {
				Grass(hdc, iX3 - iCurve / 3, iY3, 1);
				Grass(hdc, iX4 + iCurve * 2, iY1, 2);
			}
			j++;
		}
	}
	else {
		pt1[0].x = 455 - (iCurve + iCurve);
		pt1[0].y = 0;
		pt1[1].x = 342;
		pt1[1].y = 175;
		pt1[2].x = 172 + iCurve + iCurve;
		pt1[2].y = 438;
		pt1[3].x = 0;
		pt1[3].y = 702;

		pt2[0].x = 910 - (iCurve + iCurve);
		pt2[0].y = 0;
		pt2[1].x = 1022;
		pt2[1].y = 175;
		pt2[2].x = 1193 + iCurve;
		pt2[2].y = 438;
		pt2[3].x = 1365;
		pt2[3].y = 702;

		PolyBezier(hdc, pt1, 4);
		PolyBezier(hdc, pt2, 4);

		if (j % 3 == 0) {
			Grass(hdc, iX1 - iCurve * 2, iY1, 1);
			Grass(hdc, iX5, iY2, 2);
		}
		else if (j % 3 == 1) {
			Grass(hdc, iX2, iY2, 1);
			Grass(hdc, iX6 + iCurve / 3, iY3, 2);
		}
		else if (j % 3 == 2) {
			Grass(hdc, iX3 + iCurve, iY3, 1);
			Grass(hdc, iX4 - iCurve, iY1, 2);
		}
		j++;
	}
	return(0);
}







DWORD WINAPI Group_Name(LPVOID lParam)
{
	HWND hwnd = (HWND)lParam;
	HDC hdc = GetDC(hwnd);
	HPEN hPen;
	POINT W[5], M[5], C[4], O1[4], O2[4], P[4], B1[4],
		B2[4], N1[4], N2[4], S1[4], S2[4], R[4], S3[4],
		S4[4], O3[4], O4[4], S5[4], S6[4], G[5], P1[4],
		R1[4], O5[4], O6[4], D[4], B3[4], B4[4], S7[4], S8[4], G1[5], P2[4];

	//W:

	W[0].x = 200;
	W[0].y = 370;
	W[1].x = 240;
	W[1].y = 480;
	W[2].x = 295;
	W[2].y = 370;
	W[3].x = 340;
	W[3].y = 480;
	W[4].x = 390;
	W[4].y = 370;

	//M

	M[0].x = 425;
	M[0].y = 480;
	M[1].x = 425;
	M[1].y = 370;
	M[2].x = 485;
	M[2].y = 480;
	M[3].x = 545;
	M[3].y = 370;
	M[4].x = 545;
	M[4].y = 480;

	//C:


	C[0].x = 735;
	C[0].y = 370;
	C[1].x = 625;
	C[1].y = 370;
	C[2].x = 625;
	C[2].y = 490;
	C[3].x = 735;
	C[3].y = 490;

	//O1:

	O1[0].x = 845;
	O1[0].y = 370;
	O1[1].x = 755;
	O1[1].y = 370;
	O1[2].x = 755;
	O1[2].y = 490;
	O1[3].x = 845;
	O1[3].y = 490;

	//O2:

	O2[0].x = 845;
	O2[0].y = 370;
	O2[1].x = 935;
	O2[1].y = 370;
	O2[2].x = 935;
	O2[2].y = 490;
	O2[3].x = 845;
	O2[3].y = 490;

	//P:

	P[0].x = 965;
	P[0].y = 370;
	P[1].x = 1055;
	P[1].y = 370;
	P[2].x = 1055;
	P[2].y = 440;
	P[3].x = 965;
	P[3].y = 440;

	//B1:

	B1[0].x = 955;
	B1[0].y = 160;
	B1[1].x = 1040;
	B1[1].y = 160;
	B1[2].x = 1040;
	B1[2].y = 220;
	B1[3].x = 950;
	B1[3].y = 220;

	//B2:

	B2[0].x = 950;
	B2[0].y = 220;
	B2[1].x = 1040;
	B2[1].y = 220;
	B2[2].x = 1040;
	B2[2].y = 280;
	B2[3].x = 950;
	B2[3].y = 280;

	//N1:

	N1[0].x = 70 + 30;
	N1[0].y = 190;
	N1[1].x = 70 + 30;
	N1[1].y = 90;
	N1[2].x = 140 + 30;
	N1[2].y = 190;
	N1[3].x = 140 + 30;
	N1[3].y = 90;

	//N2:

	N2[0].x = 470 + 30;
	N2[0].y = 190;
	N2[1].x = 470 + 30;
	N2[1].y = 90;
	N2[2].x = 540 + 30;
	N2[2].y = 190;
	N2[3].x = 540 + 30;
	N2[3].y = 90;

	//S1:

	S1[0].x = 675 + 30;
	S1[0].y = 90;
	S1[1].x = 615 + 30;
	S1[1].y = 90;
	S1[2].x = 615 + 30;
	S1[2].y = 140;
	S1[3].x = 665 + 30;
	S1[3].y = 140;

	//S2:

	S2[0].x = 660 + 30;
	S2[0].y = 140;
	S2[1].x = 695 + 30;
	S2[1].y = 140;
	S2[2].x = 695 + 30;
	S2[2].y = 190;
	S2[3].x = 625 + 30;
	S2[3].y = 190;

	//R:

	R[0].x = 1170 + 30;
	R[0].y = 90;
	R[1].x = 1250 + 30;
	R[1].y = 90;
	R[2].x = 1250 + 30;
	R[2].y = 150;
	R[3].x = 1170 + 30;
	R[3].y = 150;


	//S3:

	S3[0].x = 465 - 50;
	S3[0].y = 350;
	S3[1].x = 405 - 50;
	S3[1].y = 350;
	S3[2].x = 405 - 50;
	S3[2].y = 410;
	S3[3].x = 465 - 50;
	S3[3].y = 410;

	//S4:

	S4[0].x = 460 - 50;
	S4[0].y = 407;
	S4[1].x = 495 - 50;
	S4[1].y = 408;
	S4[2].x = 495 - 50;
	S4[2].y = 460;
	S4[3].x = 405 - 50;
	S4[3].y = 460;

	//O3:

	O3[0].x = 905 - 50;
	O3[0].y = 360;
	O3[1].x = 855 - 50;
	O3[1].y = 360;
	O3[2].x = 855 - 50;
	O3[2].y = 460;
	O3[3].x = 905 - 50;
	O3[3].y = 460;

	//O4:

	O4[0].x = 905 - 50;
	O4[0].y = 360;
	O4[1].x = 955 - 50;
	O4[1].y = 360;
	O4[2].x = 955 - 50;
	O4[2].y = 460;
	O4[3].x = 905 - 50;
	O4[3].y = 460;

	//S5:

	S5[0].x = 1025 - 50;
	S5[0].y = 360;
	S5[1].x = 965 - 50;
	S5[1].y = 360;
	S5[2].x = 965 - 50;
	S5[2].y = 410;
	S5[3].x = 1025 - 50;
	S5[3].y = 410;

	//S6:

	S6[0].x = 1008 - 50;
	S6[0].y = 407;
	S6[1].x = 1045 - 50;
	S6[1].y = 407;
	S6[2].x = 1045 - 50;
	S6[2].y = 460;
	S6[3].x = 985 - 50;
	S6[3].y = 460;

	//G

	G[0].x = 675 - 50;
	G[0].y = 540;
	G[1].x = 595 - 50;
	G[1].y = 540;
	G[2].x = 595 - 50;
	G[2].y = 640;
	G[3].x = 675 - 50;
	G[3].y = 640;
	G[4].x = 675 - 50;
	G[4].y = 600;

	//P1:

	P1[0].x = 975 - 50;
	P1[0].y = 540;
	P1[1].x = 1050 - 50;
	P1[1].y = 540;
	P1[2].x = 1050 - 50;
	P1[2].y = 600;
	P1[3].x = 975 - 50;
	P1[3].y = 600;

	//R1:

	R1[0].x = 300;
	R1[0].y = 480;
	R1[1].x = 360;
	R1[1].y = 480;
	R1[2].x = 360;
	R1[2].y = 520;
	R1[3].x = 300;
	R1[3].y = 520;

	//O5:

	O5[0].x = 405;
	O5[0].y = 480;
	O5[1].x = 365;
	O5[1].y = 480;
	O5[2].x = 365;
	O5[2].y = 550;
	O5[3].x = 405;
	O5[3].y = 550;

	//O6:

	O6[0].x = 405;
	O6[0].y = 480;
	O6[1].x = 445;
	O6[1].y = 480;
	O6[2].x = 445;
	O6[2].y = 550;
	O6[3].x = 405;
	O6[3].y = 550;


	//D:

	D[0].x = 990;
	D[0].y = 480;
	D[1].x = 1060;
	D[1].y = 480;
	D[2].x = 1060;
	D[2].y = 550;
	D[3].x = 990;
	D[3].y = 550;

	//B3:

	B3[0].x = 180;
	B3[0].y = 620;
	B3[1].x = 240;
	B3[1].y = 620;
	B3[2].x = 240;
	B3[2].y = 655;
	B3[3].x = 180;
	B3[3].y = 655;

	//B4:

	B4[0].x = 180;
	B4[0].y = 655;
	B4[1].x = 240;
	B4[1].y = 655;
	B4[2].x = 240;
	B4[2].y = 690;
	B4[3].x = 180;
	B4[3].y = 690;

	//S7:

	S7[0].x = 480;
	S7[0].y = 620;
	S7[1].x = 410;
	S7[1].y = 620;
	S7[2].x = 410;
	S7[2].y = 655;
	S7[3].x = 473;
	S7[3].y = 655;

	//S8:

	S8[0].x = 460;
	S8[0].y = 655;
	S8[1].x = 492;
	S8[1].y = 650;
	S8[2].x = 490;
	S8[2].y = 690;
	S8[3].x = 420;
	S8[3].y = 690;

	//G1

	G1[0].x = 1050;
	G1[0].y = 620;
	G1[1].x = 990;
	G1[1].y = 620;
	G1[2].x = 990;
	G1[2].y = 690;
	G1[3].x = 1050;
	G1[3].y = 690;
	G1[4].x = 1050;
	G1[4].y = 665;

	//P2:

	P2[0].x = 1255;
	P2[0].y = 620;
	P2[1].x = 1305;
	P2[1].y = 620;
	P2[2].x = 1305;
	P2[2].y = 655;
	P2[3].x = 1255;
	P2[3].y = 655;

	PlaySound(MAKEINTRESOURCE(SONG), hInst, SND_FILENAME | SND_ASYNC | SND_RESOURCE);
	Sleep(3000);

	for (int i = 0; i < 512; i += 2)
	{
		if (i < 256)
		{

			MoveToEx(hdc, 150, 160, NULL);
			LineTo(hdc, 190, 280);
			MoveToEx(hdc, 190, 280, NULL);
			LineTo(hdc, 240, 160);
			MoveToEx(hdc, 240, 160, NULL);
			LineTo(hdc, 290, 280);
			MoveToEx(hdc, 290, 280, NULL);
			LineTo(hdc, 330, 160);


			MoveToEx(hdc, 380, 160, NULL);
			LineTo(hdc, 380, 280);
			MoveToEx(hdc, 380, 160, NULL);
			LineTo(hdc, 430, 280);
			MoveToEx(hdc, 430, 280, NULL);
			LineTo(hdc, 480, 160);
			MoveToEx(hdc, 480, 160, NULL);
			LineTo(hdc, 480, 280);

			MoveToEx(hdc, 510, 280, NULL);
			LineTo(hdc, 560, 280);



			MoveToEx(hdc, 590, 160, NULL);
			LineTo(hdc, 590, 280);
			MoveToEx(hdc, 590, 160, NULL);
			LineTo(hdc, 670, 160);
			MoveToEx(hdc, 590, 280, NULL);
			LineTo(hdc, 670, 280);
			MoveToEx(hdc, 590, 220, NULL);
			LineTo(hdc, 640, 220);


			MoveToEx(hdc, 700, 160, NULL);
			LineTo(hdc, 700, 280);
			MoveToEx(hdc, 700, 160, NULL);
			LineTo(hdc, 790, 280);
			MoveToEx(hdc, 790, 280, NULL);
			LineTo(hdc, 790, 160);



			MoveToEx(hdc, 820, 280, NULL);
			LineTo(hdc, 870, 160);
			MoveToEx(hdc, 870, 160, NULL);
			LineTo(hdc, 920, 280);
			MoveToEx(hdc, 840, 230, NULL);
			LineTo(hdc, 900, 230);


			MoveToEx(hdc, 950, 160, NULL);
			LineTo(hdc, 950, 280);

			PolyBezier(hdc, B1, 4);
			PolyBezier(hdc, B2, 4);

			MoveToEx(hdc, 1070, 160, NULL);
			LineTo(hdc, 1070, 280);
			MoveToEx(hdc, 1070, 280, NULL);
			LineTo(hdc, 1150, 280);

			MoveToEx(hdc, 1180, 160, NULL);
			LineTo(hdc, 1180, 280);
			MoveToEx(hdc, 1180, 160, NULL);
			LineTo(hdc, 1260, 160);
			MoveToEx(hdc, 1180, 280, NULL);
			LineTo(hdc, 1260, 280);
			MoveToEx(hdc, 1180, 220, NULL);
			LineTo(hdc, 1230, 220);

			Polyline(hdc, W, 5);

			Polyline(hdc, M, 5);

			MoveToEx(hdc, 575, 480, NULL);
			LineTo(hdc, 625, 480);

			PolyBezier(hdc, C, 4);

			PolyBezier(hdc, O1, 4);
			PolyBezier(hdc, O2, 4);


			MoveToEx(hdc, 965, 370, NULL);
			LineTo(hdc, 965, 490);

			PolyBezier(hdc, P, 4);

			MoveToEx(hdc, 1070, 370, NULL);
			LineTo(hdc, 1120, 435);
			MoveToEx(hdc, 1120, 435, NULL);
			LineTo(hdc, 1170, 370);
			MoveToEx(hdc, 1120, 435, NULL);
			LineTo(hdc, 1120, 490);
			MoveToEx(hdc, 1120, 490, NULL);
			LineTo(hdc, 1135, 490);
			MoveToEx(hdc, 1120, 490, NULL);
			LineTo(hdc, 1105, 490);




			hPen = CreatePen(PS_SOLID, 6, RGB(0 + i, 0 + i, 0 + i));
			SelectObject(hdc, hPen);
			Sleep(70);
			InvalidateRect(hwnd, NULL, TRUE);
			Sleep(7);
		}
		else
		{


			MoveToEx(hdc, 150, 160, NULL);
			LineTo(hdc, 190, 280);
			MoveToEx(hdc, 190, 280, NULL);
			LineTo(hdc, 240, 160);
			MoveToEx(hdc, 240, 160, NULL);
			LineTo(hdc, 290, 280);
			MoveToEx(hdc, 290, 280, NULL);
			LineTo(hdc, 330, 160);


			MoveToEx(hdc, 380, 160, NULL);
			LineTo(hdc, 380, 280);
			MoveToEx(hdc, 380, 160, NULL);
			LineTo(hdc, 430, 280);
			MoveToEx(hdc, 430, 280, NULL);
			LineTo(hdc, 480, 160);
			MoveToEx(hdc, 480, 160, NULL);
			LineTo(hdc, 480, 280);

			MoveToEx(hdc, 510, 280, NULL);
			LineTo(hdc, 560, 280);



			MoveToEx(hdc, 590, 160, NULL);
			LineTo(hdc, 590, 280);
			MoveToEx(hdc, 590, 160, NULL);
			LineTo(hdc, 670, 160);
			MoveToEx(hdc, 590, 280, NULL);
			LineTo(hdc, 670, 280);
			MoveToEx(hdc, 590, 220, NULL);
			LineTo(hdc, 640, 220);


			MoveToEx(hdc, 700, 160, NULL);
			LineTo(hdc, 700, 280);
			MoveToEx(hdc, 700, 160, NULL);
			LineTo(hdc, 790, 280);
			MoveToEx(hdc, 790, 280, NULL);
			LineTo(hdc, 790, 160);

			MoveToEx(hdc, 820, 280, NULL);
			LineTo(hdc, 870, 160);
			MoveToEx(hdc, 870, 160, NULL);
			LineTo(hdc, 920, 280);
			MoveToEx(hdc, 840, 230, NULL);
			LineTo(hdc, 900, 230);

			MoveToEx(hdc, 950, 160, NULL);
			LineTo(hdc, 950, 280);

			PolyBezier(hdc, B1, 4);
			PolyBezier(hdc, B2, 4);


			MoveToEx(hdc, 1070, 160, NULL);
			LineTo(hdc, 1070, 280);
			MoveToEx(hdc, 1070, 280, NULL);
			LineTo(hdc, 1150, 280);


			MoveToEx(hdc, 1180, 160, NULL);
			LineTo(hdc, 1180, 280);
			MoveToEx(hdc, 1180, 160, NULL);
			LineTo(hdc, 1260, 160);
			MoveToEx(hdc, 1180, 280, NULL);
			LineTo(hdc, 1260, 280);
			MoveToEx(hdc, 1180, 220, NULL);
			LineTo(hdc, 1230, 220);

			Polyline(hdc, W, 5);

			Polyline(hdc, M, 5);

			MoveToEx(hdc, 575, 480, NULL);
			LineTo(hdc, 625, 480);

			PolyBezier(hdc, C, 4);

			PolyBezier(hdc, O1, 4);
			PolyBezier(hdc, O2, 4);

			MoveToEx(hdc, 965, 370, NULL);
			LineTo(hdc, 965, 490);


			PolyBezier(hdc, P, 4);


			MoveToEx(hdc, 1070, 370, NULL);
			LineTo(hdc, 1120, 435);
			MoveToEx(hdc, 1120, 435, NULL);
			LineTo(hdc, 1170, 370);
			MoveToEx(hdc, 1120, 435, NULL);
			LineTo(hdc, 1120, 490);
			MoveToEx(hdc, 1120, 490, NULL);
			LineTo(hdc, 1135, 490);
			MoveToEx(hdc, 1120, 490, NULL);
			LineTo(hdc, 1105, 490);




			hPen = CreatePen(PS_SOLID, 6, RGB(512 - i, 512 - i, 512 - i));
			SelectObject(hdc, hPen);
			Sleep(70);
			InvalidateRect(hwnd, NULL, TRUE);
			Sleep(7);

		}
	}

	Sleep(1000);
	//NITIN SHELAR AND AUSHITOSH JAGTAP

	for (int i = 0; i < 512; i+=2)
	{
		if (i < 256)
		{
			//NITIN SHELAR

			//N

			Polyline(hdc, N1, 4);

			//I

			MoveToEx(hdc, 170 + 30, 90, NULL);
			LineTo(hdc, 240 + 30, 90);
			MoveToEx(hdc, 170 + 30, 190, NULL);
			LineTo(hdc, 240 + 30, 190);
			MoveToEx(hdc, 205 + 30, 90, NULL);
			LineTo(hdc, 205 + 30, 190);

			//T

			MoveToEx(hdc, 270 + 30, 90, NULL);
			LineTo(hdc, 340 + 30, 90);
			MoveToEx(hdc, 305 + 30, 90, NULL);
			LineTo(hdc, 305 + 30, 190);

			//I

			MoveToEx(hdc, 370 + 30, 90, NULL);
			LineTo(hdc, 440 + 30, 90);
			MoveToEx(hdc, 370 + 30, 190, NULL);
			LineTo(hdc, 440 + 30, 190);
			MoveToEx(hdc, 405 + 30, 90, NULL);
			LineTo(hdc, 405 + 30, 190);

			//N

			Polyline(hdc, N2, 4);

			//S

			PolyBezier(hdc, S1, 4);
			PolyBezier(hdc, S2, 4);

			//H

			MoveToEx(hdc, 720 + 30, 90, NULL);
			LineTo(hdc, 720 + 30, 190);
			MoveToEx(hdc, 790 + 30, 90, NULL);
			LineTo(hdc, 790 + 30, 190);
			MoveToEx(hdc, 720 + 30, 140, NULL);
			LineTo(hdc, 790 + 30, 140);

			//E

			MoveToEx(hdc, 830 + 30, 90, NULL);
			LineTo(hdc, 830 + 30, 190);
			MoveToEx(hdc, 830 + 30, 90, NULL);
			LineTo(hdc, 900 + 30, 90);
			MoveToEx(hdc, 830 + 30, 190, NULL);
			LineTo(hdc, 900 + 30, 190);
			MoveToEx(hdc, 830 + 30, 140, NULL);
			LineTo(hdc, 875 + 30, 140);

			//L


			MoveToEx(hdc, 930 + 30, 90, NULL);
			LineTo(hdc, 930 + 30, 190);
			MoveToEx(hdc, 930 + 30, 190, NULL);
			LineTo(hdc, 1010 + 30, 190);

			//A


			MoveToEx(hdc, 1040 + 30, 190, NULL);
			LineTo(hdc, 1090 + 30, 90);
			MoveToEx(hdc, 1090 + 30, 90, NULL);
			LineTo(hdc, 1140 + 30, 190);
			MoveToEx(hdc, 1065 + 30, 150, NULL);
			LineTo(hdc, 1115 + 30, 150);

			//R


			MoveToEx(hdc, 1170 + 30, 90, NULL);
			LineTo(hdc, 1170 + 30, 190);
			PolyBezier(hdc, R, 4);
			MoveToEx(hdc, 1170 + 30, 150, NULL);
			LineTo(hdc, 1250 + 30, 190);

			//AUSHITOSH JAGTAP



			//A

			MoveToEx(hdc, 275 - 50, 460, NULL);
			LineTo(hdc, 325 - 50, 360);
			MoveToEx(hdc, 325 - 50, 360, NULL);
			LineTo(hdc, 375 - 50, 460);
			MoveToEx(hdc, 295 - 50, 420, NULL);
			LineTo(hdc, 350 - 50, 420);


			//S

			PolyBezier(hdc, S3, 4);
			PolyBezier(hdc, S4, 4);

			//H

			MoveToEx(hdc, 525 - 50, 360, NULL);
			LineTo(hdc, 525 - 50, 460);
			MoveToEx(hdc, 605 - 50, 360, NULL);
			LineTo(hdc, 605 - 50, 460);
			MoveToEx(hdc, 525 - 50, 410, NULL);
			LineTo(hdc, 605 - 50, 410);

			//U

			MoveToEx(hdc, 635 - 50, 360, NULL);
			LineTo(hdc, 635 - 50, 460);
			MoveToEx(hdc, 635 - 50, 460, NULL);
			LineTo(hdc, 715 - 50, 460);
			MoveToEx(hdc, 715 - 50, 460, NULL);
			LineTo(hdc, 715 - 50, 360);

			//T


			MoveToEx(hdc, 745 - 50, 360, NULL);
			LineTo(hdc, 825 - 50, 360);
			MoveToEx(hdc, 785 - 50, 360, NULL);
			LineTo(hdc, 785 - 50, 460);

			//O


			PolyBezier(hdc, O3, 4);
			PolyBezier(hdc, O4, 4);

			//S


			PolyBezier(hdc, S5, 4);
			PolyBezier(hdc, S6, 4);


			//H

			MoveToEx(hdc, 1075 - 50, 360, NULL);
			LineTo(hdc, 1075 - 50, 460);
			MoveToEx(hdc, 1145 - 50, 360, NULL);
			LineTo(hdc, 1145 - 50, 460);
			MoveToEx(hdc, 1075 - 50, 410, NULL);
			LineTo(hdc, 1145 - 50, 410);

			//J



			MoveToEx(hdc, 420 - 50, 540, NULL);
			LineTo(hdc, 420 - 50, 640);
			MoveToEx(hdc, 420 - 50, 640, NULL);
			LineTo(hdc, 350 - 50, 640);
			MoveToEx(hdc, 350 - 50, 640, NULL);
			LineTo(hdc, 350 - 50, 610);
			MoveToEx(hdc, 400 - 50, 540, NULL);
			LineTo(hdc, 440 - 50, 540);
			MoveToEx(hdc, 340 - 50, 610, NULL);
			LineTo(hdc, 360 - 50, 610);

			//A

			MoveToEx(hdc, 465 - 50, 640, NULL);
			LineTo(hdc, 515 - 50, 540);
			MoveToEx(hdc, 515 - 50, 540, NULL);
			LineTo(hdc, 565 - 50, 640);
			MoveToEx(hdc, 490 - 50, 600, NULL);
			LineTo(hdc, 540 - 50, 600);

			//G

			Polyline(hdc, G, 5);
			MoveToEx(hdc, 655 - 50, 600, NULL);
			LineTo(hdc, 695 - 50, 600);
			MoveToEx(hdc, 675 - 50, 540, NULL);
			LineTo(hdc, 675 - 50, 545);

			//T


			MoveToEx(hdc, 725 - 50, 540, NULL);
			LineTo(hdc, 815 - 50, 540);
			MoveToEx(hdc, 770 - 50, 540, NULL);
			LineTo(hdc, 770 - 50, 640);

			//A

			MoveToEx(hdc, 845 - 50, 640, NULL);
			LineTo(hdc, 895 - 50, 540);
			MoveToEx(hdc, 895 - 50, 540, NULL);
			LineTo(hdc, 945 - 50, 640);
			MoveToEx(hdc, 870 - 50, 600, NULL);
			LineTo(hdc, 920 - 50, 600);

			//p


			MoveToEx(hdc, 975 - 50, 540, NULL);
			LineTo(hdc, 975 - 50, 640);
			PolyBezier(hdc, P1, 4);



			hPen = CreatePen(PS_SOLID, 6, RGB(0 + i, 0 + i, 0 + i));
			SelectObject(hdc, hPen);
			Sleep(70);
			InvalidateRect(hwnd, NULL, TRUE);
			Sleep(7);
		}
		else
		{
			// NITIN SHELAR


			//N

			Polyline(hdc, N1, 4);

			//I

			MoveToEx(hdc, 170 + 30, 90, NULL);
			LineTo(hdc, 240 + 30, 90);
			MoveToEx(hdc, 170 + 30, 190, NULL);
			LineTo(hdc, 240 + 30, 190);
			MoveToEx(hdc, 205 + 30, 90, NULL);
			LineTo(hdc, 205 + 30, 190);

			//T

			MoveToEx(hdc, 270 + 30, 90, NULL);
			LineTo(hdc, 340 + 30, 90);
			MoveToEx(hdc, 305 + 30, 90, NULL);
			LineTo(hdc, 305 + 30, 190);

			//I

			MoveToEx(hdc, 370 + 30, 90, NULL);
			LineTo(hdc, 440 + 30, 90);
			MoveToEx(hdc, 370 + 30, 190, NULL);
			LineTo(hdc, 440 + 30, 190);
			MoveToEx(hdc, 405 + 30, 90, NULL);
			LineTo(hdc, 405 + 30, 190);

			//N

			Polyline(hdc, N2, 4);

			//S

			PolyBezier(hdc, S1, 4);
			PolyBezier(hdc, S2, 4);

			//H

			MoveToEx(hdc, 720 + 30, 90, NULL);
			LineTo(hdc, 720 + 30, 190);
			MoveToEx(hdc, 790 + 30, 90, NULL);
			LineTo(hdc, 790 + 30, 190);
			MoveToEx(hdc, 720 + 30, 140, NULL);
			LineTo(hdc, 790 + 30, 140);

			//E

			MoveToEx(hdc, 830 + 30, 90, NULL);
			LineTo(hdc, 830 + 30, 190);
			MoveToEx(hdc, 830 + 30, 90, NULL);
			LineTo(hdc, 900 + 30, 90);
			MoveToEx(hdc, 830 + 30, 190, NULL);
			LineTo(hdc, 900 + 30, 190);
			MoveToEx(hdc, 830 + 30, 140, NULL);
			LineTo(hdc, 875 + 30, 140);

			//L


			MoveToEx(hdc, 930 + 30, 90, NULL);
			LineTo(hdc, 930 + 30, 190);
			MoveToEx(hdc, 930 + 30, 190, NULL);
			LineTo(hdc, 1010 + 30, 190);

			//A


			MoveToEx(hdc, 1040 + 30, 190, NULL);
			LineTo(hdc, 1090 + 30, 90);
			MoveToEx(hdc, 1090 + 30, 90, NULL);
			LineTo(hdc, 1140 + 30, 190);
			MoveToEx(hdc, 1065 + 30, 150, NULL);
			LineTo(hdc, 1115 + 30, 150);



			//R


			MoveToEx(hdc, 1170 + 30, 90, NULL);
			LineTo(hdc, 1170 + 30, 190);
			PolyBezier(hdc, R, 4);
			MoveToEx(hdc, 1170 + 30, 150, NULL);
			LineTo(hdc, 1250 + 30, 190);

			//AUSHITOSH JAGTAP

			//A

			MoveToEx(hdc, 275 - 50, 460, NULL);
			LineTo(hdc, 325 - 50, 360);
			MoveToEx(hdc, 325 - 50, 360, NULL);
			LineTo(hdc, 375 - 50, 460);
			MoveToEx(hdc, 295 - 50, 420, NULL);
			LineTo(hdc, 350 - 50, 420);


			//S

			PolyBezier(hdc, S3, 4);
			PolyBezier(hdc, S4, 4);

			//H

			MoveToEx(hdc, 525 - 50, 360, NULL);
			LineTo(hdc, 525 - 50, 460);
			MoveToEx(hdc, 605 - 50, 360, NULL);
			LineTo(hdc, 605 - 50, 460);
			MoveToEx(hdc, 525 - 50, 410, NULL);
			LineTo(hdc, 605 - 50, 410);

			//U

			MoveToEx(hdc, 635 - 50, 360, NULL);
			LineTo(hdc, 635 - 50, 460);
			MoveToEx(hdc, 635 - 50, 460, NULL);
			LineTo(hdc, 715 - 50, 460);
			MoveToEx(hdc, 715 - 50, 460, NULL);
			LineTo(hdc, 715 - 50, 360);

			//T


			MoveToEx(hdc, 745 - 50, 360, NULL);
			LineTo(hdc, 825 - 50, 360);
			MoveToEx(hdc, 785 - 50, 360, NULL);
			LineTo(hdc, 785 - 50, 460);

			//O


			PolyBezier(hdc, O3, 4);
			PolyBezier(hdc, O4, 4);

			//S


			PolyBezier(hdc, S5, 4);
			PolyBezier(hdc, S6, 4);


			//H

			MoveToEx(hdc, 1075 - 50, 360, NULL);
			LineTo(hdc, 1075 - 50, 460);
			MoveToEx(hdc, 1145 - 50, 360, NULL);
			LineTo(hdc, 1145 - 50, 460);
			MoveToEx(hdc, 1075 - 50, 410, NULL);
			LineTo(hdc, 1145 - 50, 410);

			//J



			MoveToEx(hdc, 420 - 50, 540, NULL);
			LineTo(hdc, 420 - 50, 640);
			MoveToEx(hdc, 420 - 50, 640, NULL);
			LineTo(hdc, 350 - 50, 640);
			MoveToEx(hdc, 350 - 50, 640, NULL);
			LineTo(hdc, 350 - 50, 610);
			MoveToEx(hdc, 400 - 50, 540, NULL);
			LineTo(hdc, 440 - 50, 540);
			MoveToEx(hdc, 340 - 50, 610, NULL);
			LineTo(hdc, 360 - 50, 610);

			//A

			MoveToEx(hdc, 465 - 50, 640, NULL);
			LineTo(hdc, 515 - 50, 540);
			MoveToEx(hdc, 515 - 50, 540, NULL);
			LineTo(hdc, 565 - 50, 640);
			MoveToEx(hdc, 490 - 50, 600, NULL);
			LineTo(hdc, 540 - 50, 600);

			//G

			Polyline(hdc, G, 5);
			MoveToEx(hdc, 655 - 50, 600, NULL);
			LineTo(hdc, 695 - 50, 600);
			MoveToEx(hdc, 675 - 50, 540, NULL);
			LineTo(hdc, 675 - 50, 545);

			//T


			MoveToEx(hdc, 725 - 50, 540, NULL);
			LineTo(hdc, 815 - 50, 540);
			MoveToEx(hdc, 770 - 50, 540, NULL);
			LineTo(hdc, 770 - 50, 640);

			//A

			MoveToEx(hdc, 845 - 50, 640, NULL);
			LineTo(hdc, 895 - 50, 540);
			MoveToEx(hdc, 895 - 50, 540, NULL);
			LineTo(hdc, 945 - 50, 640);
			MoveToEx(hdc, 870 - 50, 600, NULL);
			LineTo(hdc, 920 - 50, 600);

			//p


			MoveToEx(hdc, 975 - 50, 540, NULL);
			LineTo(hdc, 975 - 50, 640);
			PolyBezier(hdc, P1, 4);

			hPen = CreatePen(PS_SOLID, 6, RGB(512 - i, 512 - i, 512 - i));
			SelectObject(hdc, hPen);
			Sleep(70);
			InvalidateRect(hwnd, NULL, TRUE);
			Sleep(7);

		}
	}

	DeleteObject(hPen);
	ReleaseDC(hwnd, hdc);
	ResumeThread(hThreadR);
	return 0;
}



int iAngle;

DWORD WINAPI Race(LPVOID lpParam) {
	HWND hwnd = (HWND)lpParam;
	HDC hdc = GetDC(hwnd);
	RECT rc;
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	HPEN hPen = CreatePen(PS_SOLID, 5, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);

	double dD2R = 3.1415926536 / 180;


	int FAngle = 90;
	int AngleR;

	double dRad = iAngle * dD2R;

	rc.left = 500 + 30;
	rc.top = 625 - 20;
	rc.right = 800 + 30;
	rc.bottom = 700 - 20;


	Sleep(6000);
	//MessageBox(hwnd, TEXT("In Racing Thread"), TEXT("Race"), MB_OK);
	Block(hdc, 0);
	Road(hdc, 0, 1, 1);

	Sleep(5000);
	// Start 
	for (int i = 0; i < 150; i++)
	{
		Block(hdc, 0);
		Road(hdc, 0, 1, 1);
		Sleep(60);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(6);
	}

	// Center->Righlt
	for (int i = 0; i < 30; i++) {
		iAngle = FAngle - i;
		dRad = iAngle * dD2R;
		BlockMove(hdc, &rc, 325, dRad, 1, i);
		Road(hdc, i * 4, 1, 1);
		Sleep(60);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(6);
	}

	// Only Right
	for (int i = 0; i < 50; i++) {
		BlockMove(hdc, &rc, 325, dRad, 1, 29);
		Road(hdc, 29 * 4, 1, 1);
		Sleep(100);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(5);
	}

	AngleR = iAngle;
	//Right<-Center
	for (int i = 0; i < 30; i++) {
		iAngle = AngleR + i;
		dRad = iAngle * dD2R;
		BlockMove(hdc, &rc, 325, dRad, 1, 30 - i);
		Road(hdc, (29 - i) * 4, 1, 1);
		Sleep(60);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(6);
	}

	/* Stright Road
	for (int i = 0; i < 100; i++) {
		Block(hdc, 0);
		Road(hdc, 0, 1, 1);
		Sleep(60);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(5);
	}*/

	//Letf<-Center
	for (int i = 0; i < 30; i++) {
		iAngle = FAngle + i;
		dRad = iAngle * dD2R;
		BlockMove(hdc, &rc, 325, dRad, 2, i);
		Road(hdc, i * 4, 2, 1);
		Sleep(60);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(6);
	}

	//Only Left
	for (int i = 0; i < 50; i++) {
		BlockMove(hdc, &rc, 325, dRad, 2, 29);
		Road(hdc, 29 * 4, 2, 1);
		Sleep(100);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(5);
	}

	AngleR = iAngle;
	//Left->Center
	for (int i = 0; i < 30; i++) {
		iAngle = AngleR - i;
		dRad = iAngle * dD2R;
		BlockMove(hdc, &rc, 325, dRad, 2, 30 - i);
		Road(hdc, (29 - i) * 4, 2, 1);
		Sleep(60);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(6);
	}

	//Stright Road
	for (int i = 0; i < 100; i++)
	{
		Block(hdc, 0);
		Road(hdc, 0, 1, 1);
		Sleep(60);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(6);
	}
	
	//Going Forward
	for (int i = 0; i < 230; i++) {
		
		Block(hdc, i);
		Road(hdc, 0, 1, 1);
		Sleep(60);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(6);
	}

	// Slow Down
	for (int i = 0; i < 50; i++) {
		Road(hdc, 0, 1, 1);
		Sleep(50 + i * 4);
		InvalidateRect(hwnd, NULL, TRUE);
		Sleep(6);
	}
	Road(hdc, 0, 1, 1);
	Sleep(2000);
	InvalidateRect(hwnd, NULL, TRUE);

	DeleteObject(hPen);
	DeleteObject(hBrush);
	ReleaseDC(hwnd, hdc);
	ResumeThread(hThreadE);
	return(0);
}




DWORD WINAPI Our_Name(LPVOID lParam)
{
	HWND hwnd = (HWND)lParam;
	HDC hdc = GetDC(hwnd);
	HPEN hPen;
	POINT R1[4], O5[4], O6[4], D[4], B3[4], B4[4], S7[4], S8[4], G1[5], P2[4];
	//R1:

	R1[0].x = 190;
	R1[0].y = 480 - 260;
	R1[1].x = 250;
	R1[1].y = 480 - 260;
	R1[2].x = 250;
	R1[2].y = 520 - 260;
	R1[3].x = 190;
	R1[3].y = 520 - 260;

	//O5:

	O5[0].x = 295;
	O5[0].y = 480 - 260;
	O5[1].x = 255;
	O5[1].y = 480 - 260;
	O5[2].x = 255;
	O5[2].y = 550 - 260;
	O5[3].x = 295;
	O5[3].y = 550 - 260;

	//O6:

	O6[0].x = 295;
	O6[0].y = 480 - 260;
	O6[1].x = 335;
	O6[1].y = 480 - 260;
	O6[2].x = 335;
	O6[2].y = 550 - 260;
	O6[3].x = 295;
	O6[3].y = 550 - 260;


	//D:

	D[0].x = 880;
	D[0].y = 480 - 260;
	D[1].x = 950;
	D[1].y = 480 - 260;
	D[2].x = 950;
	D[2].y = 550 - 260;
	D[3].x = 880;
	D[3].y = 550 - 260;

	//B3:

	B3[0].x = 180;
	B3[0].y = 620 - 250;
	B3[1].x = 240;
	B3[1].y = 620 - 250;
	B3[2].x = 240;
	B3[2].y = 655 - 250;
	B3[3].x = 180;
	B3[3].y = 655 - 250;

	//B4:

	B4[0].x = 180;
	B4[0].y = 655 - 250;
	B4[1].x = 240;
	B4[1].y = 655 - 250;
	B4[2].x = 240;
	B4[2].y = 690 - 250;
	B4[3].x = 180;
	B4[3].y = 690 - 250;

	//S7:

	S7[0].x = 480;
	S7[0].y = 620 - 250;
	S7[1].x = 410;
	S7[1].y = 620 - 250;
	S7[2].x = 410;
	S7[2].y = 655 - 250;
	S7[3].x = 473;
	S7[3].y = 655 - 250;

	//S8:

	S8[0].x = 460;
	S8[0].y = 655 - 250;
	S8[1].x = 492;
	S8[1].y = 650 - 250;
	S8[2].x = 490;
	S8[2].y = 690 - 250;
	S8[3].x = 420;
	S8[3].y = 690 - 250;

	//G1

	G1[0].x = 1050;
	G1[0].y = 620 - 250;
	G1[1].x = 990;
	G1[1].y = 620 - 250;
	G1[2].x = 990;
	G1[2].y = 690 - 250;
	G1[3].x = 1050;
	G1[3].y = 690 - 250;
	G1[4].x = 1050;
	G1[4].y = 665 - 250;

	//P2:

	P2[0].x = 1255;
	P2[0].y = 620 - 250;
	P2[1].x = 1305;
	P2[1].y = 620 - 250;
	P2[2].x = 1305;
	P2[2].y = 655 - 250;
	P2[3].x = 1255;
	P2[3].y = 655 - 250;


	Sleep(2500);
	//ROHIT JADHAV AND ABHISHEK JAGTAP

	for (int i = 0; i < 128; i++)
	{
			//ROHIT JADHAV


			//R


			MoveToEx(hdc, 190, 480 - 260, NULL);
			LineTo(hdc, 190, 550 - 260);
			PolyBezier(hdc, R1, 4);
			MoveToEx(hdc, 190, 520 - 260, NULL);
			LineTo(hdc, 240, 550 - 260);

			//O

			PolyBezier(hdc, O5, 4);
			PolyBezier(hdc, O6, 4);


			//H


			MoveToEx(hdc, 360, 480 - 260, NULL);
			LineTo(hdc, 360, 550 - 260);
			MoveToEx(hdc, 420, 480 - 260, NULL);
			LineTo(hdc, 420, 550 - 260);
			MoveToEx(hdc, 360, 515 - 260, NULL);
			LineTo(hdc, 420, 515 - 260);

			//I

			MoveToEx(hdc, 450, 480 - 260, NULL);
			LineTo(hdc, 510, 480 - 260);
			MoveToEx(hdc, 450, 550 - 260, NULL);
			LineTo(hdc, 510, 550 - 260);
			MoveToEx(hdc, 480, 480 - 260, NULL);
			LineTo(hdc, 480, 550 - 260);

			//T


			MoveToEx(hdc, 540, 480 - 260, NULL);
			LineTo(hdc, 610, 480 - 260);
			MoveToEx(hdc, 575, 480 - 260, NULL);
			LineTo(hdc, 575, 550 - 260);

			//J

			MoveToEx(hdc, 740, 480 - 260, NULL);
			LineTo(hdc, 740, 550 - 260);
			MoveToEx(hdc, 740, 550 - 260, NULL);
			LineTo(hdc, 690, 550 - 260);
			MoveToEx(hdc, 690, 550 - 260, NULL);
			LineTo(hdc, 690, 525 - 260);
			MoveToEx(hdc, 680, 525 - 260, NULL);
			LineTo(hdc, 700, 525 - 260);
			MoveToEx(hdc, 727, 480 - 260, NULL);
			LineTo(hdc, 753, 480 - 260);

			//A

			MoveToEx(hdc, 775, 550 - 260, NULL);
			LineTo(hdc, 810, 480 - 260);
			MoveToEx(hdc, 810, 480 - 260, NULL);
			LineTo(hdc, 855, 550 - 260);
			MoveToEx(hdc, 790, 520 - 260, NULL);
			LineTo(hdc, 835, 520 - 260);

			//D

			MoveToEx(hdc, 880, 480 - 260, NULL);
			LineTo(hdc, 880, 550 - 260);
			PolyBezier(hdc, D, 4);

			//H


			MoveToEx(hdc, 965, 480 - 260, NULL);
			LineTo(hdc, 965, 550 - 260);
			MoveToEx(hdc, 1025, 480 - 260, NULL);
			LineTo(hdc, 1025, 550 - 260);
			MoveToEx(hdc, 965, 515 - 260, NULL);
			LineTo(hdc, 1025, 515 - 260);

			//A

			MoveToEx(hdc, 1050, 550 - 260, NULL);
			LineTo(hdc, 1090, 480 - 260);
			MoveToEx(hdc, 1090, 480 - 260, NULL);
			LineTo(hdc, 1130, 550 - 260);
			MoveToEx(hdc, 1070, 520 - 260, NULL);
			LineTo(hdc, 1112, 520 - 260);

			//V

			MoveToEx(hdc, 1130, 480 - 260, NULL);
			LineTo(hdc, 1170, 550 - 260);
			MoveToEx(hdc, 1170, 550 - 260, NULL);
			LineTo(hdc, 1210, 480 - 260);


			//ABHISHEK JAGTAP

			MoveToEx(hdc, 80, 690 - 250, NULL);
			LineTo(hdc, 120, 620 - 250);
			MoveToEx(hdc, 120, 620 - 250, NULL);
			LineTo(hdc, 160, 690 - 250);
			MoveToEx(hdc, 100, 660 - 250, NULL);
			LineTo(hdc, 140, 660 - 250);

			//B

			MoveToEx(hdc, 180, 620 - 250, NULL);
			LineTo(hdc, 180, 690 - 250);
			PolyBezier(hdc, B3, 4);
			PolyBezier(hdc, B4, 4);

			//H


			MoveToEx(hdc, 255, 620 - 250, NULL);
			LineTo(hdc, 255, 690 - 250);
			MoveToEx(hdc, 315, 620 - 250, NULL);
			LineTo(hdc, 315, 690 - 250);
			MoveToEx(hdc, 255, 655 - 250, NULL);
			LineTo(hdc, 315, 655 - 250);

			//I

			MoveToEx(hdc, 340, 620 - 250, NULL);
			LineTo(hdc, 400, 620 - 250);
			MoveToEx(hdc, 340, 690 - 250, NULL);
			LineTo(hdc, 400, 690 - 250);
			MoveToEx(hdc, 370, 620 - 250, NULL);
			LineTo(hdc, 370, 690 - 250);

			//S

			PolyBezier(hdc, S7, 4);
			PolyBezier(hdc, S8, 4);

			//H


			MoveToEx(hdc, 510, 620 - 250, NULL);
			LineTo(hdc, 510, 690 - 250);
			MoveToEx(hdc, 570, 620 - 250, NULL);
			LineTo(hdc, 570, 690 - 250);
			MoveToEx(hdc, 510, 655 - 250, NULL);
			LineTo(hdc, 570, 655 - 250);

			//E

			MoveToEx(hdc, 600, 620 - 250, NULL);
			LineTo(hdc, 660, 620 - 250);
			MoveToEx(hdc, 600, 620 - 250, NULL);
			LineTo(hdc, 600, 690 - 250);
			MoveToEx(hdc, 600, 690 - 250, NULL);
			LineTo(hdc, 660, 690 - 250);
			MoveToEx(hdc, 600, 655 - 250, NULL);
			LineTo(hdc, 640, 655 - 250);

			//K


			MoveToEx(hdc, 690, 620 - 250, NULL);
			LineTo(hdc, 690, 690 - 250);
			MoveToEx(hdc, 690, 655 - 250, NULL);
			LineTo(hdc, 740, 690 - 250);
			MoveToEx(hdc, 690, 655 - 250, NULL);
			LineTo(hdc, 740, 620 - 250);


			//J

			MoveToEx(hdc, 850, 620 - 250, NULL);
			LineTo(hdc, 850, 690 - 250);
			MoveToEx(hdc, 850, 690 - 250, NULL);
			LineTo(hdc, 800, 690 - 250);
			MoveToEx(hdc, 800, 690 - 250, NULL);
			LineTo(hdc, 800, 665 - 250);
			MoveToEx(hdc, 790, 665 - 250, NULL);
			LineTo(hdc, 810, 665 - 250);
			MoveToEx(hdc, 837, 620 - 250, NULL);
			LineTo(hdc, 863, 620 - 250);


			//A

			MoveToEx(hdc, 885, 690 - 250, NULL);
			LineTo(hdc, 920, 620 - 250);
			MoveToEx(hdc, 920, 620 - 250, NULL);
			LineTo(hdc, 965, 690 - 250);
			MoveToEx(hdc, 900, 660 - 250, NULL);
			LineTo(hdc, 945, 660 - 250);

			//G

			Polyline(hdc, G1, 5);
			MoveToEx(hdc, 1040, 665 - 250, NULL);
			LineTo(hdc, 1060, 665 - 250);

			//T


			MoveToEx(hdc, 1075, 620 - 250, NULL);
			LineTo(hdc, 1145, 620 - 250);
			MoveToEx(hdc, 1110, 620 - 250, NULL);
			LineTo(hdc, 1110, 690 - 250);

			//A

			MoveToEx(hdc, 1155, 690 - 250, NULL);
			LineTo(hdc, 1195, 620 - 250);
			MoveToEx(hdc, 1195, 620 - 250, NULL);
			LineTo(hdc, 1235, 690 - 250);
			MoveToEx(hdc, 1175, 660 - 250, NULL);
			LineTo(hdc, 1215, 660 - 250);


			//P

			MoveToEx(hdc, 1255, 620 - 250, NULL);
			LineTo(hdc, 1255, 690 - 250);
			PolyBezier(hdc, P2, 4);

			hPen = CreatePen(PS_SOLID, 6, RGB(0 + i*2, 0 + i*2, 0 + i*2));
			SelectObject(hdc, hPen);
			Sleep(60);
			InvalidateRect(hwnd, NULL, TRUE);
			Sleep(6);
	}
	
	Sleep(5000);
	DeleteObject(hPen);
	ReleaseDC(hwnd, hdc);
	SendMessage(hwnd, WM_DESTROY, 0, 0);
	return 0;
}

