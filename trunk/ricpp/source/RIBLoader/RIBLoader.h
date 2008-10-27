// RIBLoader.h : Haupt-Header-Datei für die Anwendung RIBLOADER
//

#if !defined(AFX_RIBLOADER_H__FD560265_C101_4CEC_A6F2_362755AB45BA__INCLUDED_)
#define AFX_RIBLOADER_H__FD560265_C101_4CEC_A6F2_362755AB45BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Hauptsymbole
#include "rendererloader/rendererloader.h"

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderApp:
// Siehe RIBLoader.cpp für die Implementierung dieser Klasse
//

class CRIBLoaderApp : public CWinApp
{
	TRendererLoader *m_rendererLoader;
	TRi *m_rc;
public:
	CRIBLoaderApp();
	virtual ~CRIBLoaderApp();

	// inline TRendererLoader *GetRendererLoader() { return m_rendererLoader; }
	inline TRi *GetRenderer() { return m_rc; }

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CRIBLoaderApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementierung
	//{{AFX_MSG(CRIBLoaderApp)
	afx_msg void OnAppAbout();
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_RIBLOADER_H__FD560265_C101_4CEC_A6F2_362755AB45BA__INCLUDED_)
