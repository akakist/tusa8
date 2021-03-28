// w32Ctrl.cpp : Implementation of the Cw32Ctrl ActiveX Control class.

#include "stdafx.h"
#include "w32.h"
#include "w32Ctrl.h"
#include "w32PropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(Cw32Ctrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(Cw32Ctrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(Cw32Ctrl, COleControl)
	DISP_FUNCTION_ID(Cw32Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(Cw32Ctrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(Cw32Ctrl, 1)
	PROPPAGEID(Cw32PropPage::guid)
END_PROPPAGEIDS(Cw32Ctrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(Cw32Ctrl, "W32.w32Ctrl.1",
	0xde7e1f64, 0x6bf1, 0x4454, 0xad, 0xea, 0x87, 0xb5, 0x81, 0x3b, 0xfa, 0x94)



// Type library ID and version

IMPLEMENT_OLETYPELIB(Cw32Ctrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_Dw32 =
		{ 0x7584FE7, 0x3832, 0x4C2B, { 0x96, 0xE9, 0x72, 0xEE, 0x7E, 0xD2, 0x8E, 0xA3 } };
const IID BASED_CODE IID_Dw32Events =
		{ 0xEDB99C32, 0xFA79, 0x4201, { 0x80, 0xBF, 0xC3, 0x14, 0x24, 0x35, 0xC1, 0xA } };



// Control type information

static const DWORD BASED_CODE _dww32OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(Cw32Ctrl, IDS_W32, _dww32OleMisc)



// Cw32Ctrl::Cw32CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for Cw32Ctrl

BOOL Cw32Ctrl::Cw32CtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_W32,
			IDB_W32,
			afxRegApartmentThreading,
			_dww32OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// Cw32Ctrl::Cw32Ctrl - Constructor

Cw32Ctrl::Cw32Ctrl()
{
	InitializeIIDs(&IID_Dw32, &IID_Dw32Events);
	// TODO: Initialize your control's instance data here.
}



// Cw32Ctrl::~Cw32Ctrl - Destructor

Cw32Ctrl::~Cw32Ctrl()
{
	// TODO: Cleanup your control's instance data here.
}



// Cw32Ctrl::OnDraw - Drawing function

void Cw32Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// Cw32Ctrl::DoPropExchange - Persistence support

void Cw32Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// Cw32Ctrl::OnResetState - Reset control to default state

void Cw32Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// Cw32Ctrl::AboutBox - Display an "About" box to the user

void Cw32Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_W32);
	dlgAbout.DoModal();
}



// Cw32Ctrl message handlers
