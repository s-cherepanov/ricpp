// RIBPreviewDoc.cpp : Implementierung der Klasse CRIBPreviewDoc
//

#include "stdafx.h"
#include "RIBPreview.h"

#include "RIBPreviewDoc.h"
#include "RIBPreviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewDoc

IMPLEMENT_DYNCREATE(CRIBPreviewDoc, CDocument)

BEGIN_MESSAGE_MAP(CRIBPreviewDoc, CDocument)
	//{{AFX_MSG_MAP(CRIBPreviewDoc)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_RELOAD, OnFileReload)
//	ON_COMMAND(ID_FILE_RENDER, OnFileRender)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewDoc Konstruktion/Destruktion

CRIBPreviewDoc::CRIBPreviewDoc()
{
	// ZU ERLEDIGEN: Hier Code für One-Time-Konstruktion einfügen
	m_invalid = false;
	m_contextHandle = NULL;
}

CRIBPreviewDoc::~CRIBPreviewDoc()
{
}


BOOL CRIBPreviewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// ZU ERLEDIGEN: Hier Code zur Reinitialisierung einfügen
	// (SDI-Dokumente verwenden dieses Dokument)
	m_invalid = false;
	m_contextHandle = NULL;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewDoc Serialisierung

void CRIBPreviewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// ZU ERLEDIGEN: Hier Code zum Speichern einfügen
	}
	else
	{
		// ZU ERLEDIGEN: Hier Code zum Laden einfügen
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewDoc Diagnose

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

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewDoc Befehle

BOOL CRIBPreviewDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;
	
	// TODO: Add your specialized creation code here
	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	m_strPathName = lpszPathName;
	m_strRealPathName = lpszPathName;
	unsigned int i;
	for ( i = 0; i < m_strPathName.length(); ++i ) {
		if ( m_strPathName[i] == '\\' )
			m_strPathName[i] = '/';
	}
	SetModifiedFlag(FALSE);     // start off with unmodified

	if ( !m_strPathName.empty() && m_strPathName[0] != '\0' ) {
		TRi *ri = ((CRIBPreviewApp *)AfxGetApp())->GetRenderer();
		if ( ri ) {
			try {
				ri->begin();
				m_contextHandle = ri->getContext();
				m_invalid = m_contextHandle == NULL;
				ri->errorHandler(((CRIBPreviewApp *)AfxGetApp())->errorPrint);
			} catch (TRendererError &t) {
				m_invalid = TRUE;
				t.displayMessage();
			} catch ( ... ) {
				m_invalid = TRUE;
				::MessageBox(NULL, "Error while open document, RiBegin()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}

			if ( m_invalid && m_contextHandle ) {
				try {
					ri->synchronize(ri->RI_ABORT);
				} catch (TRendererError &t) {
					t.displayMessage();
				} catch ( ... ) {
					::MessageBox(NULL, "Error while open document, RiSynchronize()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
				}
			}

			((CRIBPreviewApp *)AfxGetApp())->ClearLog();
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

void CRIBPreviewDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	if ( !m_strPathName.empty() && m_strPathName[0] != '\0' ) {
		TRi *ri = ((CRIBPreviewApp *)AfxGetApp())->GetRenderer();
		if ( ri && m_contextHandle != NULL ) {
			try {
				ri->context(m_contextHandle);
				ri->end();
			} catch (TRendererError &t) {
				m_invalid = TRUE;
				t.displayMessage();
			} catch ( ... ) {
				m_invalid = TRUE;
				::MessageBox(NULL, "Error while delete contents, RiEnd()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}
			if ( m_invalid ) {
				try {
					ri->synchronize(ri->RI_ABORT);
				} catch (TRendererError &t) {
					t.displayMessage();
				} catch ( ... ) {
					::MessageBox(NULL, "Error while delete contents, RiSynchronize()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
				}
			}
		}
	}

	m_strPathName = "";
	m_contextHandle = NULL;
	m_invalid = false;
	CDocument::DeleteContents();
}

void CRIBPreviewDoc::OnFileReload()
{
	// TODO: Add your command handler code here

	if ( !m_strPathName.empty() && m_strPathName[0] != '\0' ) {
		TRi *ri = ((CRIBPreviewApp *)AfxGetApp())->GetRenderer();
		if ( ri && m_contextHandle != NULL ) {
			try {
				ri->context(m_contextHandle);
				ri->end();
			} catch (TRendererError &t) {
				m_invalid = TRUE;
				t.displayMessage();
			} catch ( ... ) {
				m_invalid = TRUE;
				::MessageBox(NULL, "Error while reloading, RiEnd()/RiBegin()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}

			if ( m_invalid ) {
				try {
					ri->synchronize(ri->RI_ABORT);
				} catch (TRendererError &t) {
					t.displayMessage();
				} catch ( ... ) {
					::MessageBox(NULL, "Error while reloading, RiESynchronize()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
				}
			}
		}

		m_contextHandle = NULL;
		m_invalid = false;

		if ( ri ) {
			try {
				ri->begin();
				m_contextHandle = ri->getContext();
				m_invalid = m_contextHandle == NULL;
				ri->errorHandler(((CRIBPreviewApp *)AfxGetApp())->errorPrint);
			} catch (TRendererError &t) {
				m_invalid = TRUE;
				t.displayMessage();
				ri->synchronize(ri->RI_ABORT);
			} catch ( ... ) {
				m_invalid = TRUE;
				::MessageBox(NULL, "Error while reloading, RiEnd()/RiBegin()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}
			if ( m_invalid ) {
				try {
					ri->synchronize(ri->RI_ABORT);
				} catch (TRendererError &t) {
					t.displayMessage();
				} catch ( ... ) {
					::MessageBox(NULL, "Error while reloading, RiEnd()/RiBegin()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
				}
			}
			((CRIBPreviewApp *)AfxGetApp())->ClearLog();
		}
	}

	// Display views
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
		CView* pView = GetNextView(pos);
		if ( pView ) {
			pView->InvalidateRect(NULL);
			pView->UpdateWindow();
		}
	}   
}

void CRIBPreviewDoc::OnCloseDocument()
{
	// TODO: Add your specialized code here and/or call the base class
	// Display views

	CDocument::OnCloseDocument();
}
