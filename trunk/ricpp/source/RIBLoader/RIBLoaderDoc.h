// RIBLoaderDoc.h : Schnittstelle der Klasse CRIBLoaderDoc
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIBLOADERDOC_H__3FE9E845_E938_4577_AEFF_F2926E3827B9__INCLUDED_)
#define AFX_RIBLOADERDOC_H__3FE9E845_E938_4577_AEFF_F2926E3827B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CRIBLoaderDoc : public CDocument
{
protected: // Nur aus Serialisierung erzeugen
	CRIBLoaderDoc();
	DECLARE_DYNCREATE(CRIBLoaderDoc)

// Attribute
public:
	std::string m_strPathName;
	bool m_invalid;
	RtContextHandle m_contextHandle;
// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CRIBLoaderDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
//	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
//	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CRIBLoaderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CRIBLoaderDoc)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileReopen();
	virtual void DeleteContents();
//	virtual void OnCloseDocument();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_RIBLOADERDOC_H__3FE9E845_E938_4577_AEFF_F2926E3827B9__INCLUDED_)
