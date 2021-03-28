#pragma once

// w32Ctrl.h : Declaration of the Cw32Ctrl ActiveX Control class.


// Cw32Ctrl : See w32Ctrl.cpp for implementation.

class Cw32Ctrl : public COleControl
{
	DECLARE_DYNCREATE(Cw32Ctrl)

// Constructor
public:
	Cw32Ctrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~Cw32Ctrl();

	DECLARE_OLECREATE_EX(Cw32Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(Cw32Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(Cw32Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(Cw32Ctrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	};
};

