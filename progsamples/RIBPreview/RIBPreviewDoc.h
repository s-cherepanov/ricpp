// RIBPreviewDoc.h : interface of the CRIBPreviewDoc class
//

#pragma once

class CRIBPreviewDoc : public CDocument
{
protected: // create from serialization only
	CRIBPreviewDoc();
	DECLARE_DYNCREATE(CRIBPreviewDoc)

// Attributes
public:
	std::string m_strPathName;			// Filename of the document
	bool m_invalid;						// found errors in RIB file
	RtContextHandle m_contextHandle;	// Handle of the documents renderer context

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CRIBPreviewDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileReload();
	virtual void OnCloseDocument();
	virtual void DeleteContents();
};
