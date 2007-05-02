// RIBPreviewDoc.cpp : implementation of the CRIBPreviewDoc class
//

#include "stdafx.h"
#include "RIBPreview.h"

#include "RIBPreviewDoc.h"
#include "RIBPreviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRIBPreviewDoc

IMPLEMENT_DYNCREATE(CRIBPreviewDoc, CDocument)

BEGIN_MESSAGE_MAP(CRIBPreviewDoc, CDocument)
	ON_COMMAND(ID_FILE_RELOAD, OnFileReload)
END_MESSAGE_MAP()


// CRIBPreviewDoc construction/destruction

CRIBPreviewDoc::CRIBPreviewDoc() : m_invalid(false), m_contextHandle(TRi::RI_NULL)

{
	// TODO: add one-time construction code here

}

CRIBPreviewDoc::~CRIBPreviewDoc()
{
}

BOOL CRIBPreviewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	m_invalid = false;
	m_contextHandle = TRi::RI_NULL;

	return TRUE;
}




// CRIBPreviewDoc serialization

void CRIBPreviewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CRIBPreviewDoc diagnostics

#ifdef _DEBUG
void CRIBPreviewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRIBPreviewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRIBPreviewDoc commands

BOOL CRIBPreviewDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	// Don't call the CDocument functions
	// if (!CDocument::OnOpenDocument(lpszPathName))
	//     return FALSE;

	// TODO:  Add your specialized creation code here
	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	// Change the path seperators from \ to /
	m_strPathName = lpszPathName;
	unsigned int i;
	for ( i = 0; i < m_strPathName.length(); ++i ) {
		if ( m_strPathName[i] == '\\' )
			m_strPathName[i] = '/';
	}

	m_invalid = false;			// reload the file when displayed
	SetModifiedFlag(FALSE);     // file is unmodified
	
	if ( !m_strPathName.empty() ) {
		TRi *ri = ((CRIBPreviewApp *)AfxGetApp())->GetRenderer();
		if ( ri ) {
			try {
				ri->begin();
				m_contextHandle = ri->getContext();
			} catch (TRendererError &t) {
				m_invalid = TRUE;
				t.displayMessage();
				ri->synchronize(ri->RI_ABORT);
			} catch ( ... ) {
				m_invalid = TRUE;
				::MessageBox(NULL, "Error while open document, RiBegin()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}
		}
	}

	return TRUE;
}

BOOL CRIBPreviewDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class
	return TRUE; // don't save at all

	// return CDocument::OnSaveDocument(lpszPathName);
}

void CRIBPreviewDoc::OnFileReload()
{
	// TODO: Add your command handler code here
	m_invalid = false;			// reload the file when displayed

	if ( !m_strPathName.empty() ) {
		TRi *ri = ((CRIBPreviewApp *)AfxGetApp())->GetRenderer();
		if ( ri ) {
			try {
				ri->context(m_contextHandle);
				ri->end();
				m_contextHandle = TRi::RI_NULL;
				ri->begin();
				m_contextHandle = ri->getContext();
			} catch (TRendererError &t) {
				m_invalid = TRUE;
				t.displayMessage();
				ri->synchronize(ri->RI_ABORT);
			} catch ( ... ) {
				m_invalid = TRUE;
				::MessageBox(NULL, "Error while reloading, RiEnd()/RiBegin()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}
		}
	}

	// Display views
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
		CView* pView = GetNextView(pos);
		pView->InvalidateRect(NULL);
		pView->UpdateWindow();
	}   
}

void CRIBPreviewDoc::OnCloseDocument()
{
	// TODO: Add your specialized code here and/or call the base class
	CDocument::OnCloseDocument();
}

void CRIBPreviewDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	if ( !m_strPathName.empty() ) {
		TRi *ri = ((CRIBPreviewApp *)AfxGetApp())->GetRenderer();
		if ( ri ) {
			try {
				ri->context(m_contextHandle);
				ri->end();
				m_contextHandle = TRi::RI_NULL;
			} catch (TRendererError &t) {
				m_invalid = TRUE;
				t.displayMessage();
				ri->synchronize(ri->RI_ABORT);
			} catch ( ... ) {
				m_invalid = TRUE;
				::MessageBox(NULL, "Error while delete contents, RiEnd()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}
		}
	}

	m_strPathName.clear();

	CDocument::DeleteContents();
}
