#include<Windows.h>
#include<windowsx.h>
#include"PaintWindow.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szName[] = TEXT("Paint");

	wndclass.lpszClassName = szName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszMenuName = NULL;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;

	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szName,
		TEXT("My Paint"),
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
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
	return ((int)msg.wParam);

}

HBRUSH hBrush;
HPEN hPen;


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	static RECT rc;
	PAINTSTRUCT ps;

	int iX;
	int iY;
	static int iSize = 1;
	static int iType;
	static COLORREF cRef=RGB(0,0,0);
	int iFlag = 0;
	TCHAR szSize[128];
	static int iPrevSize=1;

	//TCHAR szStr[256];

	static int iMouseMove = 0;
	switch (iMsg) {

	case WM_CREATE:
		iType = 1;
		break;

	case WM_SIZE:
		GetClientRect(hwnd, &rc);
		break;
	
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		//wsprintf(szStr, TEXT("%d %d %d %d"), rc.left, rc.top, rc.right, rc.bottom);
		//MessageBox(hwnd, szStr, TEXT("Coordinate"), MB_OK);

		if (iMouseMove == 1&&iType==1) {
			hBrush = CreateSolidBrush(cRef);
			hPen = CreatePen(PS_SOLID, 0, cRef);
			iMouseMove = 0;
			iFlag = 1;
		}
		else if (iMouseMove == 1 && (iType == 2 || iType==3)) {
			hPen = CreatePen(PS_SOLID,2,cRef);
			SelectObject(hdc, hPen);
			MoveToEx(hdc, rc.left, rc.top,NULL);
			LineTo(hdc, rc.right, rc.bottom);
			DeleteObject(hPen);
			iMouseMove = 0;
			iFlag = 0;
		}
		else {
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			hPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
			iFlag = 1;
		}
	

		if (iFlag == 1) {
			SelectObject(hdc, hBrush);
			SelectObject(hdc, hPen);
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
			DeleteObject(hBrush);
			DeleteObject(hPen);
			iFlag = 0;
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_MOUSEMOVE:
		switch (wParam) {
		case MK_LBUTTON:
			iMouseMove = 1;
			iX = GET_X_LPARAM(lParam);
			iY = GET_Y_LPARAM(lParam);
			if (iType == 1 || iType==3) {
				rc.left = iX - iSize;
				rc.top = iY - iSize;
				rc.right = iX + iSize;
				rc.bottom = iY + iSize;
				InvalidateRect(hwnd, &rc, FALSE);
			}
			else if (iType == 2) {
				rc.left = iX - iSize;
				rc.top = iY + iSize;
				rc.right = iX + iSize;
				rc.bottom = iY - iSize;
				InvalidateRect(hwnd, &rc, FALSE);
			}
						
			break;
		} 
		break;

	case WM_LBUTTONDOWN:
		PostMessage(hwnd,WM_MOUSEMOVE,MK_LBUTTON,lParam);
		break;

	case WM_RBUTTONDOWN:
		GetClientRect(hwnd, &rc);
		iMouseMove = 0;
		InvalidateRect(hwnd, &rc, TRUE);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
			iType = 1;
			iSize = iPrevSize;
			MessageBox(hwnd, TEXT("Type 1 Brush Selected"), TEXT("Info"), MB_OK|MB_ICONINFORMATION);
			break;
		case '2':
			iType = 2;
			iPrevSize = iSize;
			iSize = 3;
			MessageBox(hwnd, TEXT("Type 2 Brush Selected\nWith Size: 3"), TEXT("Info"), MB_OK|MB_ICONINFORMATION);
			break;

		case '3':
			iType = 3;
			iPrevSize = iSize;
			iSize = 3;
			MessageBox(hwnd, TEXT("Type 3 Brush Selected\nWith Size: 3"), TEXT("Info"), MB_OK|MB_ICONINFORMATION);
			break;

		case 'R':
			cRef = RGB(255, 0, 0);
			MessageBox(hwnd, TEXT("Red Color Selected"), TEXT("Color"), MB_OK|MB_ICONINFORMATION);
			break;
		case 'G':
			cRef=RGB(0, 255, 0);
			MessageBox(hwnd, TEXT("Green Color Selected"), TEXT("Color"), MB_OK|MB_ICONINFORMATION);
			break;
		case 'B':
			cRef = RGB(0, 0, 255);
			MessageBox(hwnd, TEXT("Blue Color Selected"), TEXT("Color"), MB_OK|MB_ICONINFORMATION);
			break;
		case 'Y':
			cRef = RGB(255, 255, 0);
			MessageBox(hwnd, TEXT("Yellow Color Selected"), TEXT("Color"), MB_OK|MB_ICONINFORMATION);
			break;
		case 'M':
			cRef = RGB(255, 0, 255);
			MessageBox(hwnd, TEXT("Magenta Color Selected"), TEXT("Color"), MB_OK|MB_ICONINFORMATION);
			break;
		case 'C':
			cRef = RGB(0, 255, 255);
			MessageBox(hwnd, TEXT("Cyan Color Selected"), TEXT("Color"), MB_OK|MB_ICONINFORMATION);
			break;
		case 'K':
			cRef = RGB(0, 0, 0);
			MessageBox(hwnd, TEXT("Black Color Selected"), TEXT("Color"), MB_OK|MB_ICONINFORMATION);
			break;
		case 'W':
			cRef = RGB(255, 255, 255);
			MessageBox(hwnd, TEXT("Eraser Selected of Type 1"), TEXT("Eraser"), MB_OK|MB_ICONINFORMATION);
			iType = 1;

			break;
		case 'A':
			iSize++;
			wsprintf(szSize, TEXT("Size : %d"), iSize);
			MessageBox(hwnd, szSize, TEXT("Size"), MB_OK|MB_ICONINFORMATION);
			break;

		case 'S':
			iSize--;
			if (iSize <= 0)
				iSize = 1;
			wsprintf(szSize, TEXT("Size : %d"), iSize);
			MessageBox(hwnd, szSize, TEXT("Size"), MB_OK|MB_ICONINFORMATION);
			break;
		}
		break;
	
	case WM_DESTROY:
		CloseHandle(hBrush);
		CloseHandle(hPen);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
