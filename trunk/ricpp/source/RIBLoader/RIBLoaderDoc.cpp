// RIBLoaderDoc.cpp : Implementierung der Klasse CRIBLoaderDoc
//

#include "stdafx.h"
#include "RIBLoader.h"
#include "RIBLoaderDoc.h"
#include "RIBLoaderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderDoc

IMPLEMENT_DYNCREATE(CRIBLoaderDoc, CDocument)

BEGIN_MESSAGE_MAP(CRIBLoaderDoc, CDocument)
	//{{AFX_MSG_MAP(CRIBLoaderDoc)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_REOPEN, OnFileReopen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderDoc Konstruktion/Destruktion

CRIBLoaderDoc::CRIBLoaderDoc()
{
	// ZU ERLEDIGEN: Hier Code für One-Time-Konstruktion einfügen
	m_invalid = false;
	m_contextHandle = NULL;
}

CRIBLoaderDoc::~CRIBLoaderDoc()
{
}

BOOL CRIBLoaderDoc::OnNewDocument()
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
// CRIBLoaderDoc Serialisierung

void CRIBLoaderDoc::Serialize(CArchive& ar)
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
// CRIBLoaderDoc Diagnose

#ifdef _DEBUG
void CRIBLoaderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRIBLoaderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderDoc Befehle

BOOL CRIBLoaderDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;
	
	// TODO: Add your specialized creation code here
	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	m_strPathName = lpszPathName;
	unsigned int i;
 	for ( i = 0; i < m_strPathName.length(); ++i ) {
		if ( m_strPathName[i] == '\\' )
			m_strPathName[i] = '/';
	}

	SetModifiedFlag(FALSE);     // start off with unmodified
	m_invalid = false;
	if ( !m_strPathName.empty() && m_strPathName[0] != '\0' ) {
		TRi *ri = ((CRIBLoaderApp *)AfxGetApp())->GetRenderer();
		if ( ri ) {
			try {
				ri->begin();
				m_contextHandle = ri->getContext();
				m_invalid = m_contextHandle == NULL;
			} catch (TRendererError &t) {
				m_invalid = TRUE;
				t.displayMessage();
				ri->synchronize(ri->RI_ABORT);
			} catch ( ... ) {
				m_invalid = TRUE;
				::MessageBox(NULL, "Error while loading, RiBegin()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}
		}
	}
	return TRUE;
}

BOOL CRIBLoaderDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	return TRUE;
	
	// return CDocument::OnSaveDocument(lpszPathName);
}

void CRIBLoaderDoc::OnFileReopen()
{
	SetModifiedFlag(FALSE);

	if ( !m_strPathName.empty() && m_strPathName[0] != '\0' ) {
		TRi *ri = ((CRIBLoaderApp *)AfxGetApp())->GetRenderer();
		if ( ri && !m_invalid && m_contextHandle > 0 ) {
			try {
				ri->context(m_contextHandle);
				ri->end();
			} catch (TRendererError &t) {
				m_invalid = true;
				t.displayMessage();
			} catch ( ... ) {
				m_invalid = true;
				::MessageBox(NULL, "Error while reloading, RiEnd()/RiBegin()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}
		}


		m_invalid = false;
		m_contextHandle = TRi::RI_NULL;

		if ( ri ) {
			try {
				ri->begin();
				m_contextHandle = ri->getContext();
				m_invalid = m_contextHandle == NULL;
			} catch (TRendererError &t) {
				m_invalid = true;
				t.displayMessage();
			} catch ( ... ) {
				m_invalid = true;
				::MessageBox(NULL, "Error while reloading, RiEnd()/RiBegin()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}
			if ( m_invalid ) {
				try {
					ri->synchronize(ri->RI_ABORT);
				} catch (TRendererError &t) {
					t.displayMessage();
				} catch ( ... ) {
					::MessageBox(NULL, "Error while reloading, RiEnd()/RiBegin(), synchronize", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
				}
			}
		}
	}

	UpdateAllViews(NULL);   
}

void CRIBLoaderDoc::DeleteContents()
{
	if ( !m_strPathName.empty()  && m_strPathName[0] != '\0' ) {
		TRi *ri = ((CRIBLoaderApp *)AfxGetApp())->GetRenderer();
		if ( ri && !m_invalid && m_contextHandle ) {
			try {
				ri->context(m_contextHandle);
				ri->end();
			} catch (TRendererError &t) {
				m_invalid = true;
				t.displayMessage();
			} catch ( ... ) {
				m_invalid = true;
				::MessageBox(NULL, "Error while deleting contents, RiEnd()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}

			if ( m_invalid ) {
				try {
					ri->synchronize(ri->RI_ABORT);
				} catch (TRendererError &t) {
					t.displayMessage();
				} catch ( ... ) {
					::MessageBox(NULL, "Error while deleting contents, RiEnd(), synchronize", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
				}
			}
		}
	}

	m_strPathName = "";
	m_invalid = false;
	m_contextHandle = TRi::RI_NULL;

	CDocument::DeleteContents();
}
