#include<Windows.h>
#include"MyDialog.h"


void EnablePhysics(HWND hDlg) {
	EnableWindow(GetDlgItem(hDlg, ID_ETMASS1), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETMASS2), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETDIS), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_RBFORCE), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_RBGRAVITY), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETFORCE), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETGRAVITY), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_PBCANCLE), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_PBFIND), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_PBRESET), TRUE);
}

void DisablePhysics(HWND hDlg) {
	EnableWindow(GetDlgItem(hDlg, ID_ETMASS1), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETMASS2), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETDIS), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_RBFORCE), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_RBGRAVITY), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETFORCE), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETGRAVITY), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_PBCANCLE), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_PBFIND), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_PBRESET), FALSE);
}



void EnableChemistry(HWND hDlg) {
	EnableWindow(GetDlgItem(hDlg, ID_RBTEMP), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_RBHEAT), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_RBWD), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETt1q1), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETt2q2), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETDt2q2), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETWD), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETq2), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_PBFINDChem), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_PBRESETChem), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_PBEXITChem), TRUE);
}

void DisableChemistry(HWND hDlg) {
	EnableWindow(GetDlgItem(hDlg, ID_RBTEMP),FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_RBHEAT), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_RBWD), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETt1q1), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETt2q2), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETDt2q2), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETWD), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETq2), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_PBFINDChem),FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_PBRESETChem), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_PBEXITChem), FALSE);
}


void EnableMath(HWND hDlg) {

	EnableWindow(GetDlgItem(hDlg, ID_ETReal1), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETImg1), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETReal2), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_ETImg2), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_CBADD), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_CBSUB), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_CBMUL), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_CBDIV), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_PBGET), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_PBResetMath), TRUE);
	EnableWindow(GetDlgItem(hDlg, ID_PBExitMath), TRUE);
}

void DisableMath(HWND hDlg) {
	EnableWindow(GetDlgItem(hDlg, ID_PBGET), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETReal1), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETImg1), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETReal2),FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_ETImg2), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_CBADD), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_CBSUB), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_CBMUL), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_CBDIV), FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_PBResetMath),FALSE);
	EnableWindow(GetDlgItem(hDlg, ID_PBExitMath), FALSE);
}
