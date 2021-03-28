// w32PropPage.cpp : Implementation of the Cw32PropPage property page class.

#include "stdafx.h"
#include "w32.h"
#include "w32PropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(Cw32PropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(Cw32PropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(Cw32PropPage, "W32.w32PropPage.1",
	0xdb8e5a30, 0x5b46, 0x470d, 0xa4, 0x80, 0x4f, 0x4d, 0x26, 0x76, 0x13, 0xb)



// Cw32PropPage::Cw32PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for Cw32PropPage

BOOL Cw32PropPage::Cw32PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_W32_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// Cw32PropPage::Cw32PropPage - Constructor

Cw32PropPage::Cw32PropPage() :
	COlePropertyPage(IDD, IDS_W32_PPG_CAPTION)
{
}



// Cw32PropPage::DoDataExchange - Moves data between page and properties

void Cw32PropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// Cw32PropPage message handlers
