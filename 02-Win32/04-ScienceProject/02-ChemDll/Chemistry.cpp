#include<windows.h>
#include"Chemistry.h"

class CChemistry :public IChemistry {
private:
	long m_cRef;
public:

	CChemistry();
	~CChemistry();

	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	HRESULT __stdcall Efficiency(ChemEntry*);
};

class CChemistryClassFactory :public IClassFactory {
private:
	long m_cRef;
public:
	CChemistryClassFactory();
	~CChemistryClassFactory();

	HRESULT __stdcall QueryInterface(REFIID, void**);
	ULONG __stdcall AddRef(void);
	ULONG __stdcall Release(void);

	HRESULT __stdcall CreateInstance(IUnknown*, REFIID, void**);
	HRESULT __stdcall LockServer(BOOL);
};


long glNumberOfActiveComponent = 0;
long glNumberOfServerLock = 0;

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReasone, LPVOID lpReserve) {
	switch (dwReasone) {
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


CChemistry::CChemistry(void) {
	m_cRef = 1;
	InterlockedIncrement(&glNumberOfActiveComponent);
}

CChemistry::~CChemistry(void) {
	InterlockedDecrement(&glNumberOfActiveComponent);
}

HRESULT CChemistry::QueryInterface(REFIID riid, void **ppv) {
	
	if (riid == IID_IUnknown)
		*ppv = static_cast<IChemistry*>(this);
	else if (riid == IID_IChemistry)
		*ppv = static_cast<IChemistry*>(this);
	else {
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return(S_OK);
}


ULONG CChemistry::AddRef(void) {
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CChemistry::Release(void) {
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0) {
		delete(this);
		return(0);
	}
	return(m_cRef);
}

HRESULT CChemistry::Efficiency(ChemEntry *Entry) {
	if (Entry->iChoice == 1)
		Entry->dEffieciency = (Entry->dTemp2 - Entry->dTemp1) / (Entry->dTemp2);
	else if (Entry->iChoice == 2)
		Entry->dEffieciency = (Entry->dHeat2 - Entry->dHeat1) / (Entry->dHeat2);
	else if (Entry->iChoice == 3)
		Entry->dEffieciency = (Entry->dWorkDone) / (Entry->dHeat2);
	return(S_OK);
}


//Class Factrory che Functions

CChemistryClassFactory::CChemistryClassFactory(void) {
	m_cRef = 1;
}

CChemistryClassFactory::~CChemistryClassFactory(void) {

}

HRESULT CChemistryClassFactory::QueryInterface(REFIID riid, void **ppv) {
	if (riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory*>(this);
	else if (riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else {
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return(S_OK);
}

ULONG CChemistryClassFactory::AddRef(void) {
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
}

ULONG CChemistryClassFactory::Release(void) {
	InterlockedDecrement(&m_cRef);
	if (m_cRef == 0) {
		delete(this);
		return(0);
	}
	return(m_cRef);
}

HRESULT CChemistryClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv) {
	CChemistry *pCChemistry = NULL;
	HRESULT hr;
	if (pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);
	
	pCChemistry = new CChemistry;
	if (pCChemistry == NULL)
		return(E_OUTOFMEMORY);
	hr=pCChemistry->QueryInterface(riid, ppv);
	pCChemistry->Release();
	return(hr);
}

HRESULT CChemistryClassFactory::LockServer(BOOL fLock) {
	if (fLock)
		InterlockedIncrement(&glNumberOfServerLock);
	else
		InterlockedDecrement(&glNumberOfServerLock);
	return(S_OK);
}


extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv) {
	CChemistryClassFactory *pCChemistryClassFactory = NULL;
	HRESULT hr;
	if (rclsid != CLSID_Chemistry)
		return(CLASS_E_CLASSNOTAVAILABLE);

	pCChemistryClassFactory = new CChemistryClassFactory;
	if (pCChemistryClassFactory == NULL)
		return(E_OUTOFMEMORY);
	
	hr=pCChemistryClassFactory->QueryInterface(riid, ppv);
	pCChemistryClassFactory->Release();
	return(hr);
}

extern "C" HRESULT __stdcall DllCanUnloadNow() {

	if ((glNumberOfActiveComponent == 0) && (glNumberOfServerLock == 0))
		return(S_OK);
	else
		return(S_FALSE);
}
