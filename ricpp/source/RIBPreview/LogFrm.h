// ChildFrm.h : Schnittstelle der Klasse CLogFrame
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _RICPP_RIBPREVIEW_LOGFRM_H
#define _RICPP_RIBPREVIEW_LOGFRM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLogFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLogFrame)
public:
	CLogFrame();

// Attribute
public:

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CLogFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CLogFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CLogFrame)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnWindowNew();
	afx_msg void OnUpdateWindowNew(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // _RICPP_RIBPREVIEW_LOGFRM_H

