// RIBPreviewDoc.h : Schnittstelle der Klasse CRIBPreviewDoc
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIBPREVIEWDOC_H__26F9AAB3_8B5D_4254_86E8_B39D238FF3A8__INCLUDED_)
#define AFX_RIBPREVIEWDOC_H__26F9AAB3_8B5D_4254_86E8_B39D238FF3A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "../../context3d/rimacro.h"
#include "rendercontext/matrix3d.h"

class CRIBPreviewDoc : public CDocument
{
protected: // Nur aus Serialisierung erzeugen
	CRIBPreviewDoc();
	DECLARE_DYNCREATE(CRIBPreviewDoc)

// Attribute
public:
	std::string m_strPathName, m_strRealPathName;
	bool m_invalid;
	RtContextHandle m_contextHandle;

// Operationen
public:
// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CRIBPreviewDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CRIBPreviewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	//{{AFX_MSG(CRIBPreviewDoc)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void DeleteContents();
	afx_msg void OnFileReload();
	virtual void OnCloseDocument();
//	afx_msg void OnFileRender();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_RIBPREVIEWDOC_H__26F9AAB3_8B5D_4254_86E8_B39D238FF3A8__INCLUDED_)
