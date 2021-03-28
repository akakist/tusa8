#pragma once

// w32PropPage.h : Declaration of the Cw32PropPage property page class.


// Cw32PropPage : See w32PropPage.cpp for implementation.

class Cw32PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(Cw32PropPage)
	DECLARE_OLECREATE_EX(Cw32PropPage)

// Constructor
public:
	Cw32PropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_W32 };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

