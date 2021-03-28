// w32.cpp : Implementation of Cw32App and DLL registration.

#include "stdafx.h"
#include "w32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Cw32App NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xEC737BBF, 0xC029, 0x4267, { 0x86, 0x4E, 0xF7, 0x10, 0x35, 0x47, 0x7F, 0x54 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// Cw32App::InitInstance - DLL initialization

BOOL Cw32App::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// Cw32App::ExitInstance - DLL termination

int Cw32App::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
