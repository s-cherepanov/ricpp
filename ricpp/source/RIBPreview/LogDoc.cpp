// LogDoc.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "RIBPreview.h"
#include "LogDoc.h"
#include "LogView.h"
#include "cntritem.h"

// CLogDoc

IMPLEMENT_DYNCREATE(CLogDoc, CRichEditDoc)

CLogDoc::CLogDoc()
{
}

BOOL CLogDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;
	((CRIBPreviewApp *)AfxGetApp())->m_log = this;
	return TRUE;
}

CLogDoc::~CLogDoc()
{
}


BEGIN_MESSAGE_MAP(CLogDoc, CRichEditDoc)
END_MESSAGE_MAP()


// CLogDoc-Diagnose

#ifdef _DEBUG
void CLogDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CLogDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// CLogDoc-Serialisierung

void CLogDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: Hier Code zum Speichern einfügen
	}
	else
	{
		// TODO: Hier Code zum Laden einfügen
	}
}

CRichEditCntrItem* CLogDoc::CreateClientItem(REOBJECT* preo) const {
	return new CLogCntrItem(preo, (CLogDoc*)this);
}


void CLogDoc::Clear() {
	CLogView *pView = (CLogView *)GetView();
	if ( pView ) {
		pView->Clear();
	}
}


// CLogDoc-Befehle
RtVoid CLogDoc::errorPrint(RtInt code, RtInt severity, const char *message) {
	// Display views
	CLogView *pView = (CLogView *)GetView();
	if ( pView ) {
		std::string strCaption;
		pView->AddLine(TRendererError::formatError(strCaption, code, severity));
		pView->AddLine(message);
		pView->InvalidateRect(NULL);
		pView->UpdateWindow();
	}
}

BOOL CLogDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.

	return TRUE;
	// return CRichEditDoc::CanCloseFrame(pFrame);
}

void CLogDoc::OnCloseDocument()
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.
	((CRIBPreviewApp *)AfxGetApp())->m_log = NULL;

	CRichEditDoc::OnCloseDocument();
}

BOOL CLogDoc::VisibleView() {
	CLogView *pView = (CLogView *)GetView();
	if ( pView ) {
		return pView->IsWindowVisible();
	}
	return FALSE;
}

void CLogDoc::HideView(BOOL hide) {
	CLogView *pView = (CLogView *)GetView();
	if ( pView ) {
		CFrameWnd *fw = pView->GetParentFrame();
		if ( fw ) {
			fw->ShowWindow(hide ? SW_HIDE : SW_SHOW);
		}
	}
}

void CLogDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.

	CRichEditDoc::SetTitle("Error Log");
	// CRichEditDoc::SetTitle(lpszTitle);
}
