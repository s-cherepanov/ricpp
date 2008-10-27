// RIBLoaderView.h : Schnittstelle der Klasse CRIBLoaderView
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIBLOADERVIEW_H__80460FB9_07D1_473A_A56A_DD330EA97F99__INCLUDED_)
#define AFX_RIBLOADERVIEW_H__80460FB9_07D1_473A_A56A_DD330EA97F99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CRIBLoaderView : public CView
{
protected: // Nur aus Serialisierung erzeugen
	CRIBLoaderView();
	DECLARE_DYNCREATE(CRIBLoaderView)

// Attribute
public:
	CRIBLoaderDoc* GetDocument();

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CRIBLoaderView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CRIBLoaderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CRIBLoaderView)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Testversion in RIBLoaderView.cpp
inline CRIBLoaderDoc* CRIBLoaderView::GetDocument()
   { return (CRIBLoaderDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_RIBLOADERVIEW_H__80460FB9_07D1_473A_A56A_DD330EA97F99__INCLUDED_)
