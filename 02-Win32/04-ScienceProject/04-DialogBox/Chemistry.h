struct ChemEntry {
	double dTemp1;
	double dTemp2;
	double dHeat1;
	double dHeat2;
	double dWorkDone;
	int iChoice;
	double dEffieciency;
};

class IChemistry :public IUnknown {
public:
	virtual HRESULT __stdcall Efficiency(ChemEntry*) = 0;
};

const IID IID_IChemistry = { 0xe221ff1c, 0x5b20, 0x4325, 0xbe, 0x42, 0x5e, 0x15, 0x7e, 0xe9, 0x4e, 0x3c };

const CLSID CLSID_Chemistry = { 0xc9fee2c2, 0x8ea, 0x49e5, 0xa5, 0xd5, 0x21, 0xbc, 0x5a, 0x39, 0xd, 0xea };
