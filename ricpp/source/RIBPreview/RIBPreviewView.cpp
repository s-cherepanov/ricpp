// RIBPreviewView.cpp : Implementierung der Klasse CRIBPreviewView
//

#include "stdafx.h"
#include "RIBPreview.h"

#include "RIBPreviewDoc.h"
#include "RIBPreviewView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewView

IMPLEMENT_DYNCREATE(CRIBPreviewView, CView)

BEGIN_MESSAGE_MAP(CRIBPreviewView, CView)
	//{{AFX_MSG_MAP(CRIBPreviewView)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
//	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_COMMAND(ID_FILE_RENDER, OnFileRender)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewView Konstruktion/Destruktion

CRIBPreviewView::CRIBPreviewView()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen,
  resetTransform();
}

CRIBPreviewView::~CRIBPreviewView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewView Zeichnen

void CRIBPreviewView::resetTransform()
{
	m_camTransform.identity();
	m_camRotate.identity();
	m_camTranslate.identity();
	m_stepSize = (RtFloat)0.1;
}

void CRIBPreviewView::OnDraw(CDC* pDC)
{
	CRIBPreviewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// ZU ERLEDIGEN: Hier Code zum Zeichnen der ursprünglichen Daten hinzufügen
	TRi *ri = ((CRIBPreviewApp *)AfxGetApp())->GetRenderer();

	if ( pDoc->m_contextHandle && !pDoc->m_invalid && !pDoc->m_strPathName.empty()  && pDoc->m_strPathName[0] != '\0' && ri ) {
        try {
			RtFloat *f;
			RtMatrix mat;
			RtBoolean b = ri->RI_TRUE;

			HDC hdc = pDC->GetSafeHdc();
			if ( ! hdc )
				return;
			HWND hwnd = this->GetSafeHwnd();
			if ( ! hwnd )
				return;

			m_camTransform.get(mat);
			
			f = (RtFloat *)&(mat[0][0]);
			ri->context(pDoc->m_contextHandle);
			ri->synchronize("reset");
			ri->option("ricpp.control", "ricpp.control.camtrans", &f, ri->RI_NULL);
			ri->display(pDoc->m_strPathName.c_str(),
				ri->RI_FRAMEBUFFER, ri->RI_RGB,
				"devicecontext", &hdc,
				"windowhandle", &hwnd,
				ri->RI_NULL);
			ri->clipping((RtFloat)0.1, (RtFloat)1000.0);
			ri->option("ricpp.control", "ricpp.control.disableformat", &b, ri->RI_NULL);
			ri->readArchive(
				pDoc->m_strPathName.c_str(), NULL,
				ri->RI_NULL);
			ri->errorHandler(ri->errorAbort);
        } catch (TRendererError &t) {
			pDoc->m_invalid = true;
            t.displayMessage();
        } catch (TSystemException &t) {
			pDoc->m_invalid = true;
            t.displayMessage();
		} catch ( ... ) {
			pDoc->m_invalid = true;
			MessageBox("Error while drawing", "Error");
		}

		if ( pDoc->m_invalid ) {
			try {
				ri->context(pDoc->m_contextHandle);
				ri->synchronize(ri->RI_ABORT);
			} catch ( TSystemException &t ) {
	            t.displayMessage();
			} catch ( ... ) {
				MessageBox("Error while drawing (synchronize)", "Error");
			}
			pDoc->m_contextHandle = TRi::RI_NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewView Diagnose

#ifdef _DEBUG
void CRIBPreviewView::AssertValid() const
{
	CView::AssertValid();
}

void CRIBPreviewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRIBPreviewDoc* CRIBPreviewView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRIBPreviewDoc)));
	return (CRIBPreviewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewView Nachrichten-Handler

void CRIBPreviewView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	bool keyPressed = true;
	m_camTransform.setPreMultiply(false);

	if ( nChar == 'x' ) {
		m_camTransform.rotateX((RtFloat)1.0*nRepCnt);
	} else if ( nChar == 'X' ) {
		m_camTransform.rotateX((RtFloat)-1.0*nRepCnt);
	} else if ( nChar == 'y' ) {
		m_camTransform.rotateY((RtFloat)1.0*nRepCnt);
	} else if ( nChar == 'Y' ) {
		m_camTransform.rotateY((RtFloat)-1.0*nRepCnt);
	} else if ( nChar == 'z' ) {
		m_camTransform.rotateZ((RtFloat)1.0*nRepCnt);
	} else if ( nChar == 'Z' ) {
		m_camTransform.rotateZ((RtFloat)-1.0*nRepCnt);
	} else if ( nChar == 't' ) {
		m_camTransform.translate(0.0, -m_stepSize, 0.0);
	} else if ( nChar == 'v' ) {
		m_camTransform.translate(0.0, m_stepSize, 0.0);
	} else if ( nChar == 'g' ) {
		m_camTransform.translate(-m_stepSize, 0.0, 0.0);
	} else if ( nChar == 'f' ) {
		m_camTransform.translate(m_stepSize, 0.0, 0.0);
	} else if ( nChar == 'r' ) {
		m_camTransform.translate(0.0, 0.0, -m_stepSize);
	} else if ( nChar == 'c' ) {
		m_camTransform.translate(0.0, 0.0, m_stepSize);
	} else if ( nChar == '+' ) {
		m_stepSize *= (RtFloat)2.0;
	} else if ( nChar == '-' ) {
		m_stepSize /= (RtFloat)2.0;
	} else if ( nChar == 'i' ) {
		resetTransform();
	} else {
		keyPressed = false;
	}
	if ( keyPressed ) {
		// m_camTransform = m_camRotate;
		// m_camTransform.concatTransform(m_camTranslate);
		InvalidateRect(NULL, FALSE);
		UpdateWindow();
	}

	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CRIBPreviewView::OnFileRender()
{
	CRIBPreviewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	TRi *ri = ((CRIBPreviewApp *)AfxGetApp())->GetRibout();

	if ( pDoc->m_invalid || pDoc->m_strPathName.empty() || pDoc->m_strPathName[0] == '\0' || ri == NULL )
		return;

	CString s;
#if _MSC_VER < 0x0500 || _MFC_VER < 0x0700
	char *str = getenv("TEMP");
	s = str ? str : "";
#else
	s.GetEnvironmentVariable("TEMP");
#endif
	if ( s == "" ) {
		s = "C:\\TEMP";
	}
	s += "\\ribout.rib";
	((CRIBPreviewApp *)AfxGetApp())->ClearLog();
    try {
		RtFloat *f;
		RtMatrix mat;
		RtBoolean b;

		m_camTransform.get(mat);
		
		f = (RtFloat *)&(mat[0][0]);
		b = ri->RI_TRUE;
		RtInt fl = ri->RI_FALSE;

		RECT r;
		this->GetClientRect(&r);
		ri->errorHandler(((CRIBPreviewApp *)AfxGetApp())->errorPrint);
		ri->begin(s);
        // ri->archiveRecord(ri->RI_STRUCTURE, "RenderMan RIB-Structure 1.1");
        // ri->archiveRecord(ri->RI_VERBATIM, "version 3.03\n");
	    ri->format(r.right, r.bottom, -1);
		ri->option("ricpp.control", "ricpp.control.camtrans", &f, ri->RI_NULL);
		ri->option("ricpp.control", "ricpp.control.writearchive", &b, ri->RI_NULL);
		ri->option("ricpp.control", "ricpp.control.disableformat", &b, ri->RI_NULL);
		ri->option("ricpp.control", "ricpp.control.writeobject", &b, ri->RI_NULL);
		ri->readArchive(
			pDoc->m_strPathName.c_str(), NULL,
			"ricpp.cachearchive", &fl,
			ri->RI_NULL);
		ri->end();
    } catch (TRendererError &t) {
        t.displayMessage();
		pDoc->m_invalid = true;
    } catch (TSystemException &t) {
        t.displayMessage();
		pDoc->m_invalid = true;
	} catch ( ... ) {
		MessageBox("Error while drawing", "Error");
		pDoc->m_invalid = true;
	}
	if ( pDoc->m_invalid ) {
		try {
			ri->synchronize(ri->RI_ABORT);
		} catch ( TSystemException &t ) {
	        t.displayMessage();
		} catch ( ... ) {
			MessageBox("Error while writing RIB", "Error");
		}
	} else {
		// Render View
		CString cmdline = ((CRIBPreviewApp *)AfxGetApp())->m_modulepath;
		cmdline += "\\Batches\\renderprev.bat ";
		cmdline += ((CRIBPreviewApp *)AfxGetApp())->m_modulepath;
		cmdline += " \"";
		cmdline += s;
		cmdline += "\"";
	    
		// Call render.bat to start the renderer
		system(cmdline);

		/*
		std::string cmdline = "start aqsis -progress -verbose -pause -type=\"framebuffer\" -mode=\"rgba\" \"";
		cmdline += m_strRealPathName.c_str();
		cmdline += "\"";
		system(cmdline.c_str());
		*/
	}
}
