// RIBLoaderView.cpp : Implementierung der Klasse CRIBLoaderView
//

#include "stdafx.h"
#include "RIBLoader.h"
#include "RIBLoaderDoc.h"
#include "RIBLoaderView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderView

IMPLEMENT_DYNCREATE(CRIBLoaderView, CView)

BEGIN_MESSAGE_MAP(CRIBLoaderView, CView)
	//{{AFX_MSG_MAP(CRIBLoaderView)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderView Konstruktion/Destruktion

CRIBLoaderView::CRIBLoaderView()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen,
}

CRIBLoaderView::~CRIBLoaderView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderView Zeichnen

void CRIBLoaderView::OnDraw(CDC* pDC)
{
	CRIBLoaderDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// ZU ERLEDIGEN: Hier Code zum Zeichnen der ursprünglichen Daten hinzufügen

	TRi *ri = ((CRIBLoaderApp *)AfxGetApp())->GetRenderer();


	if ( pDoc && !pDoc->m_invalid && !pDoc->m_strPathName.empty() && pDoc->m_strPathName[0] != '\0' && ri ) {
        try {
			HDC hdc = pDC->GetSafeHdc();
			if ( ! hdc )
				return;
			HWND hwnd = this->GetSafeHwnd();
			if ( ! hwnd )
				return;

			ri->context(pDoc->m_contextHandle);
			ri->synchronize("reset");
			ri->display(pDoc->m_strPathName.c_str(),
				ri->RI_FRAMEBUFFER, ri->RI_RGB,
				"devicecontext", &hdc,
				"windowhandle", &hwnd,
				ri->RI_NULL);
			ri->readArchive(pDoc->m_strPathName.c_str(), NULL, ri->RI_NULL);
        } catch (TRendererError &t) {
			pDoc->m_invalid = true;
            t.displayMessage();
		} catch ( ... ) {
			pDoc->m_invalid = true;
			MessageBox("Error while drawing, RiReadArchive()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
		}
		if ( pDoc->m_invalid ) {
			try {
				ri->synchronize(ri->RI_ABORT);
			} catch (TRendererError &t) {
				t.displayMessage();
			} catch ( ... ) {
				MessageBox("Error while drawing, synchronize, RiReadArchive()", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderView Diagnose

#ifdef _DEBUG
void CRIBLoaderView::AssertValid() const
{
	CView::AssertValid();
}

void CRIBLoaderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRIBLoaderDoc* CRIBLoaderView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRIBLoaderDoc)));
	return (CRIBLoaderDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderView Nachrichten-Handler

void CRIBLoaderView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	InvalidateRect(NULL);	
}

void CRIBLoaderView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
}
