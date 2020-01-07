#include<Windows.h>
#include"India.h"
#include<math.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI Orange(LPVOID);
DWORD WINAPI White(LPVOID);
DWORD WINAPI Green(LPVOID);
DWORD WINAPI AshokChakara(LPVOID);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szName[] = TEXT("My Project");

	wndclass.lpszClassName = szName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszMenuName = NULL;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;

	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_HAND);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szName,
		TEXT("Independence Day Gift!!"),
		WS_OVERLAPPED | WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX,
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

static int iFlag;
static HANDLE hThreadO = NULL;
static HANDLE hThreadW = NULL;
static HANDLE hThreadG = NULL;
static HANDLE hThreadAC = NULL;
int iThreadOFlag = 0;
int iThreadWFlag = 0;
int iThreadGFlag = 0;
RECT rcThread;
int iMove = 0;
int iSize = 0;
int iChangedY = 0;
int iMoveSize = 0;
int iSize2Move = 0;


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
	static HPEN hPen;

	HBRUSH hBrush;
	//TCHAR szStr[256];


	switch (iMsg) {
	
	case WM_CREATE:
		iFlag = 1;
		iMove = 0;
		iSize = 0;

		hThreadO = CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE)Orange,
				(LPVOID)hwnd,
				CREATE_SUSPENDED,
				NULL);
		if (hThreadO == NULL) {
			MessageBox(hwnd, "Thread Creation For Orange Color is Failed!!", "ERROR", MB_OK);
			DestroyWindow(hwnd);
		}

		hThreadW = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)White,
			(LPVOID)hwnd,
			CREATE_SUSPENDED,
			NULL);

		if (hThreadW == NULL) {
			MessageBox(hwnd, "Thread Creation For White Color is Failed!!", "Error", MB_OK);
			DestroyWindow(hwnd);
		}

		hThreadG = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Green,
			(LPVOID)hwnd,
			CREATE_SUSPENDED,
			NULL);

		if (hThreadG == NULL) {
			MessageBox(hwnd, "Thread Creation For Green Color is Failed!!", "Error", MB_OK);
			DestroyWindow(hwnd);
		}

		hThreadAC = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)AshokChakara,
			(LPVOID)hwnd,
			CREATE_SUSPENDED,
			NULL);
		
		if (hThreadAC == NULL) {
			MessageBox(hwnd, "Thread Creation For AshokChakara Failed!!", "Error", MB_OK);
			DestroyWindow(hwnd);
		}

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		if (iFlag == 1) {
			SetBkColor(hdc,RGB(0,0,0));
			SetTextColor(hdc, RGB(0, 255, 0));
			DrawText(hdc, "Press S For Continue!!", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else {
			hBrush = CreateSolidBrush(RGB(128, 128,128));
			SelectObject(hdc, hBrush);
			Rectangle(hdc,rc.left,rc.top,rc.right,rc.bottom);
			DeleteObject(hBrush);
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case 'S':
			iFlag = 0;
			InvalidateRect(hwnd, NULL, TRUE);
			GetClientRect(hwnd, &rcThread);
			iChangedY = rcThread.bottom/3;
			ResumeThread(hThreadO);
			ResumeThread(hThreadW);
			ResumeThread(hThreadG);
			break;
		}
		break;

	case WM_MOVE:
		if (iMove!=0) {
			iThreadOFlag = 1;
			iThreadWFlag = 1;
			iThreadGFlag = 1;

			iMoveSize = 1;
			InvalidateRect(hwnd, NULL, TRUE);
			//MessageBox(hwnd, "You Just Move Window", "WM_MOVE", MB_OK);
		}
		iMove=1;
		break;

	case WM_SIZE:
		if (iSize!=0) {
			iThreadOFlag = 1;
			iThreadWFlag = 1;
			iThreadGFlag = 1;
			
			
			GetClientRect(hwnd, &rcThread);

			/*wsprintf(szStr, "Left: %ld Top: %ld Right: %ld Bottom: %d", rcThread.left, rcThread.top, rcThread.right, rcThread.bottom);
			MessageBox(hwnd, szStr, TEXT("Coordinates"), MB_OK);*/
			
			if (iMoveSize == 1 && rcThread.left == 0 && rcThread.top == 0 && rcThread.right == 0 && rcThread.bottom == 0) {
				SuspendThread(hThreadO);
				SuspendThread(hThreadW);
				SuspendThread(hThreadG);
				iSize2Move = 1;
			}
			else if (iMoveSize == 1 && iSize2Move==1) {
				iChangedY = rcThread.bottom / 3;
				InvalidateRect(hwnd, NULL, TRUE);
				ResumeThread(hThreadO);
				ResumeThread(hThreadW);
				ResumeThread(hThreadG);
				iMoveSize = 1;
				iSize2Move = 0;
			}
			iChangedY = rcThread.bottom / 3;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		iSize=1;
		break;

	case WM_DESTROY:
		CloseHandle(hThreadO);
		CloseHandle(hThreadW);
		CloseHandle(hThreadG);
		CloseHandle(hThreadAC);
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

DWORD WINAPI Orange(LPVOID lpParam) {
	HDC hdc;
	HPEN hPen;
	HWND hwnd;
	int i;

	hwnd = (HWND)lpParam;
	hdc = GetDC(hwnd);
	hPen = CreatePen(PS_SOLID, 0, RGB(255,128,0));
	SelectObject(hdc, hPen);

Loop:
	i = 0;
	for (i = 0; i <= rcThread.right; i++) {
		if (iThreadOFlag == 0) {
			MoveToEx(hdc, 0 + i, 0, NULL);
			LineTo(hdc, 0 + i, iChangedY);
			Sleep(25);
		}
		else {
			iThreadOFlag = 0;
			goto Loop;
		}
	}
	
	DeleteObject(hPen);
	ReleaseDC(hwnd,hdc);
	return (0);
}

DWORD WINAPI White(LPVOID lpParam) {
	HDC hdc;
	HPEN hPen;
	HWND hwnd;
	int i;
	hwnd = (HWND)lpParam;
	hdc = GetDC(hwnd);
	hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	SelectObject(hdc, hPen);

Loop:
	i = 0;
	for (i = 0; i <= rcThread.right; i++) {
		if (iThreadWFlag == 0) {
			MoveToEx(hdc, 0 + i, iChangedY, NULL);
			LineTo(hdc, 0 + i, iChangedY * 2);
			Sleep(25);
		}
		else {
			iThreadWFlag = 0;
			goto Loop;
		}
	}

	DeleteObject(hPen);
	ReleaseDC(hwnd, hdc);
	return 0;
}


DWORD WINAPI Green(LPVOID lpParam) {
	HDC hdc;
	HPEN hPen;
	HWND hwnd;
	int i;
	hwnd = (HWND)lpParam;
	hdc = GetDC(hwnd);
	hPen = CreatePen(PS_SOLID, 0, RGB(0, 255, 0));
	SelectObject(hdc, hPen);

Loop:
	i = 0;
	for (i = 0; i <= rcThread.right; i++) {
		if (iThreadGFlag == 0) {
			MoveToEx(hdc, 0 + i, iChangedY * 2, NULL);
			LineTo(hdc, 0 + i, iChangedY * 3);
			Sleep(25);
		}
		else {
			iThreadGFlag = 0;
			goto Loop;
		}
	}
	DeleteObject(hPen);
	ReleaseDC(hwnd, hdc);
	ResumeThread(hThreadAC);
	return 0;
}

DWORD WINAPI AshokChakara(LPVOID lpParam) {
	HDC hdc;
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	HWND hwnd = (HWND)lpParam;
	hdc = GetDC(hwnd);
	int iX = rcThread.right / 2;
	int iY = rcThread.bottom/2;
	int iRadius = iChangedY / 2;

	double x;
	double y;

	int iAngle = 0;
	double pi = 3.14159;
	double dDegToRad = pi / 180.0;
	double Rad = iAngle * dDegToRad;

	SelectObject(hdc, hPen);
	Ellipse(hdc,iX-iRadius, iY-iRadius,iX+iRadius, iY+iRadius);
	
	for (int i = 0; i < 24; i++) {
		MoveToEx(hdc, iX, iY, NULL);
		x = iRadius * cos(Rad);
		x = iX + x;
		y = iRadius* sin(Rad);
		y = iY - y;
		LineTo(hdc, x, y);
		Sleep(500);
		iAngle = iAngle+18;
		Rad = iAngle * dDegToRad;
	}


	DeleteObject(hPen);
	ReleaseDC(hwnd, hdc);
	MessageBox(hwnd, "Happy Independence Day!!\n\nFrom:\n\tRohit Ramesh Jadhav", "Happy Independence Day!!", MB_OK | MB_ICONINFORMATION);
	PostMessage(hwnd, WM_DESTROY, 0, 0);
	return 0;
}
