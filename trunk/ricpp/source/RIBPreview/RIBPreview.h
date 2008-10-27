// RIBPreview.h : Haupt-Header-Datei für die Anwendung RIBPREVIEW
//

#if !defined(AFX_RIBPREVIEW_H__008FBED1_5D98_4469_B6F1_DB1844A2B99F__INCLUDED_)
#define AFX_RIBPREVIEW_H__008FBED1_5D98_4469_B6F1_DB1844A2B99F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Hauptsymbole
#include "rendererloader/rendererloader.h"
#include "rendercontext/matrix3d.h"

#include "LogDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewApp:
// Siehe RIBPreview.cpp für die Implementierung dieser Klasse
//

class CRIBPreviewApp : public CWinApp
{
	TRendererLoader *m_rendererLoader;
	TRi *m_rc;
	TRi *m_rib;
public:
	CRIBPreviewApp();
	virtual ~CRIBPreviewApp();

	inline TRendererLoader *GetRendererLoader() { return m_rendererLoader; }
	inline TRi *GetRenderer() { return m_rc; }
	inline TRi *GetRibout() { return m_rib; }

	CString m_modulepath;

	CLogDoc *m_log;
	void ClearLog();
	static RtVoid errorPrint (RtInt code, RtInt severity, const char *message);

	// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CRIBPreviewApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementierung
	//{{AFX_MSG(CRIBPreviewApp)
	afx_msg void OnAppAbout();
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnWindowErrorlog();
	afx_msg void OnUpdateWindowErrorlog(CCmdUI *pCmdUI);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_RIBPREVIEW_H__008FBED1_5D98_4469_B6F1_DB1844A2B99F__INCLUDED_)
