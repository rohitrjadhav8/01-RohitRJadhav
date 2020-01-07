#include<Windows.h>
#include"PhyLawOfAttraction.h"

struct UserEntry {
	long double fMass1;
	long double fMass2;
	long double fDis;
	long double fChoice;
	long double pfAnswer;
};

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpResarve) {

	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return(TRUE);
}

void LawOfAttraction(struct UserEntry *Entry, int iChoice) {
	if (iChoice == 1){
		//Calculate Force
		Entry->pfAnswer = (Entry->fMass1*Entry->fMass2*Entry->fChoice) / (Entry->fDis*Entry->fDis);
	}
	else if (iChoice == 2) {
		//Calculate Gravity;
		Entry->pfAnswer = (Entry->fChoice*Entry->fDis*Entry->fDis) / (Entry->fMass1*Entry->fMass2);
	}
	
}

