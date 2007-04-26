// RIBPreview.h : main header file for the RIBPreview application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "rendererloader.h"


// CRIBPreviewApp:
// See RIBPreview.cpp for the implementation of this class
//

class CRIBPreviewApp : public CWinApp
{
	TRendererLoader *m_rendererLoader;
	TRi             *m_ri;

public:
	CRIBPreviewApp();
	~CRIBPreviewApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	inline TRi *GetRenderer() { return m_ri; }

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRIBPreviewApp theApp;