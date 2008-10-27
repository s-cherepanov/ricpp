// RIBPreviewView.cpp : implementation of the CRIBPreviewView class
//

#include "stdafx.h"
#include "RIBPreview.h"

#include "RIBPreviewDoc.h"
#include "RIBPreviewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRIBPreviewView

IMPLEMENT_DYNCREATE(CRIBPreviewView, CView)

BEGIN_MESSAGE_MAP(CRIBPreviewView, CView)
END_MESSAGE_MAP()

// CRIBPreviewView construction/destruction

CRIBPreviewView::CRIBPreviewView()
{
	// TODO: add construction code here

}

CRIBPreviewView::~CRIBPreviewView()
{
}


// CRIBPreviewView drawing

void CRIBPreviewView::OnDraw(CDC* pDC)
{
	CRIBPreviewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
	TRi *ri = ((CRIBPreviewApp *)AfxGetApp())->GetRenderer();

	if ( !pDoc->m_invalid && !pDoc->m_strPathName.empty() && ri ) {
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
			ri->clipping((RtFloat).1, (RtFloat)1000.0);
			ri->readArchive(
				pDoc->m_strPathName.c_str(), NULL,
				ri->RI_NULL);
		} catch (TRendererError &t) {
            t.displayMessage();
			pDoc->m_invalid = true; // Stop drawing
        } catch (TSystemException &t) {
            t.displayMessage();
			pDoc->m_invalid = true; // Stop drawing
		} catch ( ... ) {
			MessageBox("Error while drawing", "Error");
			pDoc->m_invalid = true; // Stop drawing
		}
		if ( pDoc->m_invalid ) {
			try {
				ri->synchronize(ri->RI_ABORT);
			} catch ( TSystemException &t ) {
	            t.displayMessage();
			} catch ( ... ) {
				MessageBox("Error while drawing (synchronize)", "Error");
			}
		}
	}
}


// CRIBPreviewView diagnostics

#ifdef _DEBUG
void CRIBPreviewView::AssertValid() const
{
	CView::AssertValid();
}

void CRIBPreviewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRIBPreviewDoc* CRIBPreviewView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRIBPreviewDoc)));
	return (CRIBPreviewDoc*)m_pDocument;
}
#endif //_DEBUG

// CRIBPreviewView message handlers
