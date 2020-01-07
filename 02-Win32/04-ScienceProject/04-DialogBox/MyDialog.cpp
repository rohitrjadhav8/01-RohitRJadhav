#define UNICODE

#include<Windows.h>
#include<stdio.h>
#include<fcntl.h>
#include<math.h>
#include<stdlib.h>
#include"MyDialog.h"
#include"Chemistry.h"

#import "Debug/MathsDotNetDll.tlb" no_namespace,raw_interfaces_only
#include"Debug/MathsDotNetDll.tlh"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int iAdd = 0;
int iSub = 0;
int iMul = 0;
int iDiv = 0;

HINSTANCE hInst=NULL;
static HWND hDlg=NULL;
static IChemistry *pIChemistry = NULL;
IMath *iMath = NULL;



struct UserEntry {
	long double fMass1;
	long double fMass2;
	long double fDis;
	long double fChoice;
	long double pfAnswer;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szName[] = TEXT("Project");
	hInst = hInstance;

	wndclass.lpszClassName = szName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszMenuName = NULL;

	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;

	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(MyIcon));
	wndclass.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(MyIcon));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szName,
		TEXT("Project"),
		WS_POPUP,//WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME,
		0,
		0,
		1366,
		786,
		HWND_DESKTOP,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}
	return((int)msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	static HBITMAP hBit;
	HDC hdc;
	HDC hdcBitmap;
	PAINTSTRUCT ps;
	BITMAP x;


	switch (iMsg) {
	case WM_CREATE:
		hBit = LoadBitmap(hInst, MAKEINTRESOURCE(MyBitmap));
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hdcBitmap = CreateCompatibleDC(hdc);
		SelectObject(hdcBitmap, hBit);
		GetObject(hBit, sizeof(BITMAP), &x);
		BitBlt(hdc, 0, 0, x.bmWidth, x.bmHeight, hdcBitmap, 0, 0, SRCCOPY);
		DeleteDC(hdcBitmap);
		EndPaint(hwnd, &ps);
		break;


	case WM_KEYDOWN:
		switch (wParam) {
		case 'S':
			DialogBox(hInst, MAKEINTRESOURCE(MyDialog), hwnd, DlgProc);
			DestroyWindow(hwnd);
			break;
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
	


BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	
	void Clear(HWND);
	void PrintInformation(TCHAR*, TCHAR*, TCHAR*);
	void GetMathsInfo(TCHAR*, TCHAR*, TCHAR*, TCHAR*);
	//Information Buffers
	
	

	//Physics
	struct UserEntry Entry;
	TCHAR strMass1[50];
	TCHAR strMass2[50];
	TCHAR strDis[50];
	TCHAR szStr[256];
	void DisablePhysics(HWND);
	void EnablePhysics(HWND);

	//Chemistry
	struct ChemEntry cEntry;
	TCHAR strTempHeat1[50];
	TCHAR strTempHeat2[50];
	TCHAR strWorkDone[50];
	static TCHAR szTemp[512];
	static TCHAR szHeat[512];
	static TCHAR szWD[512];
	void EnableChemistry(HWND);
	void DisableChemistry(HWND);



	TCHAR szANS[512];

	//Maths
	TCHAR strAdd[512];
	TCHAR strSub[512];
	TCHAR strMul[512];
	TCHAR strDiv[512];
	CLSID clsid;
	static TCHAR Real1[128];
	static TCHAR Real2[128];
	static TCHAR Img1[128];
	static TCHAR Img2[128];
	double ansReal;
	double ansImg;
	TCHAR Equation[256];
	void EnableMath(HWND);
	void DisableMath(HWND);


	HDC hdc;
	HBRUSH hbrBkgnd = NULL;

	HMODULE hDll;
	static TCHAR strChoice[50];
	static int iChoice = 0;
	static int iChemChoice = 0;
	HRESULT hr;

	typedef void(*lpLawOfAttraction)(struct UserEntry*, int);
	static lpLawOfAttraction pfn=NULL;


	switch (iMsg) {
	case WM_INITDIALOG:

		hr = CoInitialize(NULL);
		if (FAILED(hr)) {
			MessageBox(hDlg, TEXT("Failed To Initilize COM!!"), TEXT("ERROR"), MB_OK);
			EndDialog(hDlg, 1);
		}

		//Physics Load
		hDll = LoadLibrary(TEXT("PhyLawOfAttraction.dll"));
		pfn = (lpLawOfAttraction)GetProcAddress(hDll, "LawOfAttraction");

		//Physics End

		SendMessage(hDlg, WM_COMMAND, ID_PHY, 0);
		SendDlgItemMessage(hDlg, ID_PHY, BM_SETCHECK, BST_CHECKED, 0);
		swprintf_s(szStr, TEXT("     M1*M2*Gravity\nF =------------------------\n        Distance^2"));
		SetDlgItemText(hDlg, ID_FORCEFormula, szStr);

		SetFocus(GetDlgItem(hDlg, ID_ETMASS1));
		iChoice = 1;

		//Chem Start
		CoCreateInstance(CLSID_Chemistry, NULL, CLSCTX_INPROC_SERVER, IID_IChemistry, (void**)&pIChemistry);
		if (pIChemistry == NULL) {
			MessageBox(hDlg, TEXT("IChemistry Cannot Obtain!!"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, 0);
		}
		PrintInformation(szTemp, szHeat, szWD);

		//Chem Ends



		//Maths Start
		GetMathsInfo(strAdd, strSub, strMul, strDiv);
		hr = CLSIDFromProgID(L"DotNetServerMaths.CMath", &clsid);
		if (FAILED(hr)) {
			MessageBox(hDlg, TEXT("Failed To get CLSID!!"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, 0);
		}

		hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, __uuidof(IMath), (void**)&iMath);
		if (FAILED(hr))
		{
			MessageBox(hDlg, TEXT("Error in CoCreateInstance"), TEXT("Error"), MB_OK);
			EndDialog(hDlg, 0);
		}


		//Maths Ends


		SetDlgItemText(hDlg, ID_Add, strAdd);
		SetDlgItemText(hDlg, ID_Sub, strSub);
		SetDlgItemText(hDlg, ID_Mul, strMul);
		SetDlgItemText(hDlg, ID_Div, strDiv);

		/*SendDlgItemMessage(hDlg, ID_RBFORCE, BM_SETCHECK, BST_CHECKED, 0);
		EnableWindow(GetDlgItem(hDlg, ID_ETFORCE), FALSE);
		EnableWindow(GetDlgItem(hDlg, ID_ETGRAVITY), TRUE);*/
		return(TRUE);
		break;

	case WM_CTLCOLORDLG:
		hdc = HDC(wParam);
		if (hbrBkgnd == NULL)
		{
			hbrBkgnd = CreateSolidBrush(RGB(255, 148, 77));
		}
		return (INT_PTR)hbrBkgnd;
		break;

	case WM_CTLCOLORSTATIC:
		hdc = HDC(wParam);
		SetTextColor(hdc, RGB(0, 0,0 ));
		SetBkColor(hdc, RGB(255, 148, 77));
		if (hbrBkgnd == NULL)
		{
			hbrBkgnd = CreateSolidBrush(RGB(255, 148, 77));
		}
		return (INT_PTR)hbrBkgnd;
		break;

	case WM_CTLCOLOREDIT:
		hdc = HDC(wParam);
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(240, 100, 30));
		if (hbrBkgnd == NULL)
		{
			hbrBkgnd = CreateSolidBrush(RGB(240, 100, 30));
		}
		return (INT_PTR)hbrBkgnd;
		break;
	

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		//PHYSICS
		case ID_PHY:
			EnablePhysics(hDlg);
			SendMessage(hDlg, WM_COMMAND, ID_PBRESET, 0);
			SendMessage(hDlg, WM_COMMAND, ID_PBResetMath, 0);
			Clear(hDlg);
			SendDlgItemMessage(hDlg, ID_RBTEMP, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBHEAT, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBWD, BM_SETCHECK, BST_UNCHECKED, 0);
			DisableChemistry(hDlg);
			DisableMath(hDlg);


			break;

		case ID_PBRESET:
			SetDlgItemText(hDlg, ID_ETMASS1,TEXT(""));
			SetDlgItemText(hDlg, ID_ETMASS2, TEXT(""));
			SetDlgItemText(hDlg, ID_ETDIS, TEXT(""));
			SetDlgItemText(hDlg, ID_ETFORCE,TEXT(""));
			SetDlgItemText(hDlg, ID_ETGRAVITY, TEXT(""));
			//SetDlgItemText(hDlg, ID_STTEST, TEXT("HELLO WORLD!!"));
			SendDlgItemMessage(hDlg, ID_RBFORCE, BM_SETCHECK, BST_CHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBGRAVITY, BM_SETCHECK, BST_UNCHECKED, 0);
			SetDlgItemText(hDlg, ID_STFORCE, TEXT(""));
			swprintf_s(szStr, TEXT("     M1*M2*Gravity\nF =------------------------\n        Distance^2"));
			SetDlgItemText(hDlg, ID_FORCEFormula, szStr);
			SetDlgItemText(hDlg, ID_GravityFormula, TEXT(""));
			SetDlgItemText(hDlg, ID_STGRAVITY, TEXT("Enter Gravity B/w Them"));
			iChoice = 1;
			EnableWindow(GetDlgItem(hDlg, ID_ETFORCE), FALSE);
			EnableWindow(GetDlgItem(hDlg, ID_ETGRAVITY), TRUE);
			break;
			
		case ID_RBFORCE:
			iChoice = 1;
			SetDlgItemText(hDlg, ID_STGRAVITY, TEXT("Enter Gravity B/w Them"));
			SetDlgItemText(hDlg, ID_STFORCE, TEXT(""));
			SetDlgItemText(hDlg, ID_ETFORCE, TEXT(""));
			SetDlgItemText(hDlg, ID_ETGRAVITY, TEXT(""));
			EnableWindow(GetDlgItem(hDlg, ID_ETFORCE), FALSE);
			EnableWindow(GetDlgItem(hDlg, ID_ETGRAVITY),TRUE);
			swprintf_s(szStr, TEXT("     M1*M2*Gravity\nF =------------------------\n        Distance^2"));
			SetDlgItemText(hDlg, ID_FORCEFormula, szStr);
			SetDlgItemText(hDlg, ID_GravityFormula, TEXT(""));
			//MessageBox(hDlg, TEXT("Force Radio Button"), TEXT("Force Radio"), MB_OK);
			break;

		case ID_RBGRAVITY:
			iChoice = 2;
			SetDlgItemText(hDlg, ID_STFORCE, TEXT("Enter Force B/w Them"));
			SetDlgItemText(hDlg, ID_STGRAVITY, TEXT(""));
			SetDlgItemText(hDlg, ID_ETFORCE, TEXT(""));
			SetDlgItemText(hDlg, ID_ETGRAVITY, TEXT(""));
			EnableWindow(GetDlgItem(hDlg, ID_ETFORCE), TRUE);
			EnableWindow(GetDlgItem(hDlg, ID_ETGRAVITY), FALSE);
			swprintf_s(szStr, TEXT("       F*Distance^2\nG = -----------------------\n         M1*M2"));
			SetDlgItemText(hDlg, ID_GravityFormula, szStr);
			SetDlgItemText(hDlg, ID_FORCEFormula, TEXT(""));
			//MessageBox(hDlg, TEXT("Gravity Radio Button"), TEXT("Gravity"), MB_OK);
			break;
	
		case ID_PBFIND:
		
			GetDlgItemText(hDlg, ID_ETMASS1, strMass1, 50);
			GetDlgItemText(hDlg, ID_ETMASS2, strMass2, 50);
			GetDlgItemText(hDlg, ID_ETDIS, strDis, 50);

			Entry.fMass1 = _wtof(strMass1);
			Entry.fMass2 = _wtof(strMass2);
			Entry.fDis = _wtof(strDis);
			Entry.pfAnswer = 0;

			
			if (iChoice == 1) {
				GetDlgItemText(hDlg, ID_ETGRAVITY, strChoice, 50);
				//MessageBox(hDlg, TEXT("In Choice1"), TEXT("Info"), MB_OK);
				Entry.fChoice = _wtof(strChoice);
				pfn(&Entry, iChoice);
				SetDlgItemText(hDlg, ID_STFORCE, TEXT("Your Force B/w Them"));
				swprintf_s(szStr, TEXT("%lf"), Entry.pfAnswer);
				EnableWindow(GetDlgItem(hDlg, ID_ETFORCE), TRUE);
				SetDlgItemText(hDlg, ID_ETFORCE, szStr);
			}
			else if (iChoice == 2) {
				GetDlgItemText(hDlg, ID_ETFORCE, strChoice, 50);
				//MessageBox(hDlg, TEXT("In Choice2"), TEXT("Info"), MB_OK);

				Entry.fChoice = _wtof(strChoice);
				pfn(&Entry, iChoice);
				SetDlgItemText(hDlg, ID_STGRAVITY, TEXT("Your Gravity B/w Them"));
				EnableWindow(GetDlgItem(hDlg, ID_ETGRAVITY), TRUE);
				swprintf_s(szStr, TEXT("%lf"), Entry.pfAnswer);
				SetDlgItemText(hDlg, ID_ETGRAVITY, szStr);
			}
			break;

		case ID_PBCANCLE:
			//MessageBox(hDlg, TEXT("Now Dialog Box Will Close"), TEXT("Info"), MB_OK);
			EndDialog(hDlg, 0);
			break;


		//Chemistry
		case ID_CHEM:
			EnableChemistry(hDlg);
			//SendDlgItemMessage(hDlg, ID_RBTEMP, BM_SETCHECK, BST_CHECKED, 0);
			SendMessage(hDlg, WM_COMMAND, ID_PBRESETChem,0);
			SendMessage(hDlg, WM_COMMAND, ID_PBRESET, 0);
			SendMessage(hDlg, WM_COMMAND, ID_PBResetMath, 0);
			SendDlgItemMessage(hDlg, ID_RBFORCE, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBGRAVITY, BM_SETCHECK, BST_UNCHECKED, 0);
			SetDlgItemText(hDlg, ID_FORCEFormula, TEXT(""));
			SetDlgItemText(hDlg, ID_GravityFormula, TEXT(""));
			SetDlgItemText(hDlg, ID_STGRAVITY, TEXT(""));
			DisableMath(hDlg);
			DisablePhysics(hDlg);

		case ID_RBTEMP:
			iChemChoice = 1;
			EnableWindow(GetDlgItem(hDlg, ID_ETWD), FALSE);
			EnableWindow(GetDlgItem(hDlg, ID_ETq2), FALSE);
			EnableWindow(GetDlgItem(hDlg, ID_ETt1q1), TRUE);
			EnableWindow(GetDlgItem(hDlg, ID_ETDt2q2), TRUE);
			EnableWindow(GetDlgItem(hDlg, ID_ETt2q2), TRUE);
			Clear(hDlg);
			SetDlgItemText(hDlg, ID_STINFO, szTemp);

			break;

		case ID_RBHEAT:
			iChemChoice = 2;
			EnableWindow(GetDlgItem(hDlg, ID_ETWD), FALSE);
			EnableWindow(GetDlgItem(hDlg, ID_ETq2), FALSE);
			EnableWindow(GetDlgItem(hDlg, ID_ETt1q1), TRUE);
			EnableWindow(GetDlgItem(hDlg, ID_ETDt2q2), TRUE);
			EnableWindow(GetDlgItem(hDlg, ID_ETt2q2), TRUE);
			Clear(hDlg);
			SetDlgItemText(hDlg, ID_STINFO, szHeat);


			break;

		case ID_RBWD:
			iChemChoice = 3;
			EnableWindow(GetDlgItem(hDlg, ID_ETt1q1), FALSE);
			EnableWindow(GetDlgItem(hDlg, ID_ETDt2q2), FALSE);
			EnableWindow(GetDlgItem(hDlg, ID_ETt2q2), FALSE);
			EnableWindow(GetDlgItem(hDlg, ID_ETWD), TRUE);
			EnableWindow(GetDlgItem(hDlg, ID_ETq2), TRUE);
			Clear(hDlg);
			SetDlgItemText(hDlg, ID_STINFO, szWD);



			break;

		case ID_PBFINDChem:
		
			if (iChemChoice == 1) {
				//Temperture
				cEntry.iChoice = 1;
				GetDlgItemText(hDlg, ID_ETt2q2, strTempHeat2, 50);
				cEntry.dTemp2 = _wtof(strTempHeat2);
				GetDlgItemText(hDlg, ID_ETt1q1, strTempHeat1, 50);
				cEntry.dTemp1 = _wtof(strTempHeat1);
				
				GetDlgItemText(hDlg, ID_ETDt2q2, strTempHeat2, 50);
				
				if (cEntry.dTemp2 != (_wtof(strTempHeat2)))
					MessageBox(hDlg, TEXT("Invalid Input For Temperture 2"), TEXT("Warnnig"), MB_OK);
				else {
					
					pIChemistry->Efficiency(&cEntry);
					swprintf_s(szANS, TEXT("      (%.4f)   ---   (%.4f)\nn = --------------------------------------------------------\n                       (%.2f)\n"), cEntry.dTemp2, cEntry.dTemp1, cEntry.dTemp2);
					SetDlgItemText(hDlg, ID_STEQU, szANS);
					swprintf_s(szANS, TEXT("Efficiency Of Carnot Engine is\n\n            n = %f"), cEntry.dEffieciency);
					SetDlgItemText(hDlg, ID_STANS, szANS);
				}


			}
			else if (iChemChoice == 2) {
				//Heat
				cEntry.iChoice = 2;
				GetDlgItemText(hDlg, ID_ETt2q2, strTempHeat2, 50);
				cEntry.dHeat2 = _wtof(strTempHeat2);
				GetDlgItemText(hDlg, ID_ETt1q1, strTempHeat1, 50);
				cEntry.dHeat1 = _wtof(strTempHeat1);

				GetDlgItemText(hDlg, ID_ETDt2q2, strTempHeat2, 50);
				if (cEntry.dHeat2 != (_wtof(strTempHeat2)))
					MessageBox(hDlg, TEXT("Invalid Input For Heat 2"), TEXT("Warning"), MB_OK);
				else {
					pIChemistry->Efficiency(&cEntry);
					swprintf_s(szANS, TEXT("      (%.4f)   ---   (%.4f)\nn = --------------------------------------------------------\n                       (%.2f)\n"), cEntry.dHeat2, cEntry.dHeat1, cEntry.dHeat2);
					SetDlgItemText(hDlg, ID_STEQU, szANS);
					swprintf_s(szANS, TEXT("Efficiency Of Carnot Engine is\n\n            n = %f"), cEntry.dEffieciency);
					SetDlgItemText(hDlg, ID_STANS, szANS);

				}

			}
			else if (iChemChoice == 3) {
				//WorkDone
				cEntry.iChoice = 3;
				GetDlgItemText(hDlg, ID_ETWD, strWorkDone, 50);
				cEntry.dWorkDone = _wtof(strWorkDone);
				GetDlgItemText(hDlg, ID_ETq2,strTempHeat2,50);
				cEntry.dHeat2 = _wtof(strTempHeat2);
				if (pIChemistry == NULL) {
					MessageBox(hDlg, TEXT("IChemistry Cannot Obtain!!"), TEXT("Error"), MB_OK);
					EndDialog(hDlg, 0);
				}
				pIChemistry->Efficiency(&cEntry);
				swprintf_s(szANS, TEXT("              %.4f\n      n = ------------------------------------\n              %.4f\n"), cEntry.dWorkDone,cEntry.dHeat2);
				SetDlgItemText(hDlg, ID_STEQU, szANS);
				swprintf_s(szANS, TEXT("Efficiency Of Carnot Engine is\n\n            n = %f"), cEntry.dEffieciency);
				SetDlgItemText(hDlg, ID_STANS, szANS);

			}
			break;

		case ID_PBRESETChem:
			SendDlgItemMessage(hDlg, ID_RBTEMP, BM_SETCHECK, BST_CHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBHEAT, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBWD, BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(hDlg, WM_COMMAND, ID_RBTEMP, 0);
			break;

		case ID_PBEXITChem:
			EndDialog(hDlg, 0);
			break;


		//Maths
		case ID_MATH:
			EnableMath(hDlg);
			//SendMessage(hDlg, WM_COMMAND, ID_PBRESETChem, 0);
			SendMessage(hDlg, WM_COMMAND, ID_PBRESET, 0);
			SendMessage(hDlg, WM_COMMAND, ID_PBResetMath, 0);
			Clear(hDlg);
			SendDlgItemMessage(hDlg, ID_RBTEMP, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBHEAT, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBWD, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBFORCE, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_RBGRAVITY, BM_SETCHECK, BST_UNCHECKED, 0);
			SetDlgItemText(hDlg, ID_FORCEFormula, TEXT(""));
			SetDlgItemText(hDlg, ID_GravityFormula, TEXT(""));
			SetDlgItemText(hDlg, ID_STGRAVITY, TEXT(""));


			DisableChemistry(hDlg);
			DisablePhysics(hDlg);
			break;

		case ID_PBExitMath:
			EndDialog(hDlg, 0);
			break;

		case ID_PBGET:
			GetDlgItemText(hDlg, ID_ETReal1, Real1, 128);
			GetDlgItemText(hDlg, ID_ETImg1, Img1, 128);
			GetDlgItemText(hDlg, ID_ETReal2, Real2, 128);
			GetDlgItemText(hDlg, ID_ETImg2, Img2, 128);

			swprintf_s(Equation, TEXT("          z1 = (%s) + (%s) i"), Real1, Img1);
			SetDlgItemText(hDlg, ID_Eq1, Equation);
			swprintf_s(Equation, TEXT("          z2 = (%s) + (%s) i"), Real2, Img2);
			SetDlgItemText(hDlg, ID_Eq2, Equation);
			break;


		case ID_CBADD:
			if (iAdd == 0)
				iAdd = 1;
			else if (iAdd == 1)
				iAdd = 0;


			if (iAdd == 1) {
				SendMessage(hDlg, WM_COMMAND,ID_PBGET, 0);
				iMath->Addition(_wtof(Real1), _wtof(Img1), _wtof(Real2), _wtof(Img2), &ansReal, &ansImg);
	
				swprintf_s(szANS, TEXT("  zAns = (%g) + (%g)i\n\n    Real Part = %.4f\n  Imaginary Part = %.4f"), ansReal, ansImg,ansReal,ansImg);
				SetDlgItemText(hDlg, ID_ansADD, szANS);
			}
			else {
				SetDlgItemText(hDlg, ID_ansADD, TEXT(""));
			}
			break;

		case ID_CBSUB:
			if (iSub == 0)
				iSub = 1;
			else if (iSub == 1)
				iSub = 0;


			if (iSub == 1) {
				SendMessage(hDlg, WM_COMMAND, ID_PBGET, 0);
				iMath->Subtraction(_wtof(Real1), _wtof(Img1), _wtof(Real2), _wtof(Img2), &ansReal, &ansImg);

				swprintf_s(szANS, TEXT("  zAns = (%g) + (%g)i\n\n    Real Part = %.4f\n  Imaginary Part = %.4f"), ansReal, ansImg, ansReal, ansImg);
				SetDlgItemText(hDlg, ID_ansSUB, szANS);
			}
			else {
				SetDlgItemText(hDlg, ID_ansSUB, TEXT(""));
			}
			break;

		case ID_CBMUL:
			if (iMul == 0)
				iMul = 1;
			else if (iMul == 1)
				iMul = 0;


			if (iMul == 1) {
				SendMessage(hDlg, WM_COMMAND, ID_PBGET, 0);
				iMath->Multiplication(_wtof(Real1), _wtof(Img1), _wtof(Real2), _wtof(Img2), &ansReal, &ansImg);

				swprintf_s(szANS, TEXT("  zAns = (%g) + (%g)i\n\n    Real Part = %.4f\n  Imaginary Part = %.4f"), ansReal, ansImg, ansReal, ansImg);
				SetDlgItemText(hDlg, ID_ansMUL, szANS);
			}
			else {
				SetDlgItemText(hDlg, ID_ansMUL, TEXT(""));
			}
			break;

		case ID_CBDIV:
			if (iDiv == 0)
				iDiv = 1;
			else if (iDiv == 1)
				iDiv = 0;


			if (iDiv == 1) {
				SendMessage(hDlg, WM_COMMAND, ID_PBGET, 0);
				iMath->Division(_wtof(Real1), _wtof(Img1), _wtof(Real2), _wtof(Img2), &ansReal, &ansImg);

				swprintf_s(szANS, TEXT("  zAns = (%g) + (%g)i\n\n    Real Part = %.4f\n  Imaginary Part = %.4f"), ansReal, ansImg, ansReal, ansImg);
				SetDlgItemText(hDlg, ID_ansDIV, szANS);
			}
			else {
				SetDlgItemText(hDlg, ID_ansDIV, TEXT(""));
			}
			break;


		case ID_PBResetMath:
			SendDlgItemMessage(hDlg, ID_CBADD, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_CBSUB, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_CBMUL, BM_SETCHECK, BST_UNCHECKED, 0);
			SendDlgItemMessage(hDlg, ID_CBDIV, BM_SETCHECK, BST_UNCHECKED, 0);
			iAdd = 0;
			iSub = 0;
			iDiv = 0;
			iMul = 0;
			SetDlgItemText(hDlg, ID_ansADD, TEXT(""));
			SetDlgItemText(hDlg, ID_ansSUB, TEXT(""));
			SetDlgItemText(hDlg, ID_ansMUL, TEXT(""));
			SetDlgItemText(hDlg, ID_ansDIV, TEXT(""));
			SetDlgItemText(hDlg, ID_Eq1, TEXT(""));
			SetDlgItemText(hDlg, ID_Eq2, TEXT(""));
			SetDlgItemText(hDlg, ID_ETReal1, TEXT(""));
			SetDlgItemText(hDlg, ID_ETImg1, TEXT(""));
			SetDlgItemText(hDlg, ID_ETReal2, TEXT(""));
			SetDlgItemText(hDlg, ID_ETImg2, TEXT(""));

			break;
		}
		return(TRUE);
		break;
	}
	return(FALSE);
}


void Clear(HWND hDlg) {
	SetDlgItemText(hDlg, ID_ETt1q1, TEXT(""));
	SetDlgItemText(hDlg, ID_ETt2q2, TEXT(""));
	SetDlgItemText(hDlg, ID_ETDt2q2, TEXT(""));
	SetDlgItemText(hDlg, ID_ETWD, TEXT(""));
	SetDlgItemText(hDlg, ID_ETq2, TEXT(""));
	SetDlgItemText(hDlg, ID_STEQU, TEXT(""));
	SetDlgItemText(hDlg, ID_STANS, TEXT(""));
	SetDlgItemText(hDlg, ID_STINFO, TEXT(""));
}

void PrintInformation(TCHAR *Temp, TCHAR *Heat, TCHAR *WD) {
	int i = 0;
	int c;

	FILE *file;

	_wfopen_s(&file,TEXT("MyInfo.txt"), TEXT("r"));

	while ((c = fgetwc(file)) != '$')
		Temp[i++] = c;
	Temp[i] = '\0';

	i = 0;
	while ((c = fgetwc(file)) != '#')
		Heat[i++] = c;
	Heat[i] = '\0';

	i = 0;
	while ((c = fgetwc(file)) != '@')
		WD[i++] = c;
	WD[i] = '\0';
}

void GetMathsInfo(TCHAR *Add, TCHAR *Sub, TCHAR *Mul, TCHAR *Div) {
	int i = 0;
	int c;
	FILE *file;

	_wfopen_s(&file, TEXT("Formulas.txt"), TEXT("r"));

	while ((c = fgetwc(file)) != '!')
		Add[i++] = c;
	Add[i] = '\0';

	i = 0;
	while ((c = fgetwc(file)) != '@')
		Sub[i++] = c;
	Sub[i] = '\0';
	
	i = 0;
	while ((c = fgetwc(file)) != '#')
		Mul[i++] = c;
	Mul[i] = '\0';

	i = 0;
	while ((c = fgetwc(file)) != '$')
		Div[i++] = c;
	Div[i] = '\0';
}
