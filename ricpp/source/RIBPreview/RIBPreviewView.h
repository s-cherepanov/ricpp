// RIBPreviewView.h : Schnittstelle der Klasse CRIBPreviewView
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIBPREVIEWVIEW_H__4856363C_B337_4633_84DA_A74A11140FA2__INCLUDED_)
#define AFX_RIBPREVIEWVIEW_H__4856363C_B337_4633_84DA_A74A11140FA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRIBPreviewView : public CView
{
protected: // Nur aus Serialisierung erzeugen
	CRIBPreviewView();
	DECLARE_DYNCREATE(CRIBPreviewView)

// Attribute
public:
	TMatrix3D m_camTransform,m_camRotate,m_camTranslate;
	RtFloat m_stepSize;
	void resetTransform();

	CRIBPreviewDoc* GetDocument();

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CRIBPreviewView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CRIBPreviewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CRIBPreviewView)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileRender();
};

#ifndef _DEBUG  // Testversion in RIBPreviewView.cpp
inline CRIBPreviewDoc* CRIBPreviewView::GetDocument()
   { return (CRIBPreviewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_RIBPREVIEWVIEW_H__4856363C_B337_4633_84DA_A74A11140FA2__INCLUDED_)
