// LogView.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "RIBPreview.h"
#include "LogView.h"


// CLogView

IMPLEMENT_DYNCREATE(CLogView, CRichEditView)

CLogView::CLogView()
{
}

CLogView::~CLogView()
{
}

BEGIN_MESSAGE_MAP(CLogView, CRichEditView)
END_MESSAGE_MAP()


// CLogView-Diagnose

#ifdef _DEBUG
void CLogView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CLogView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif //_DEBUG

// Members
void CLogView::AddLine(const char *str) {
	CRichEditCtrl &ecl = this->GetRichEditCtrl();
//	int nStartChar, nEndChar;
//	ecl.GetSel(nStartChar, nEndChar);
//	ecl.SetSel(-1, -1, 1);
	ecl.ReplaceSel(str);
	ecl.ReplaceSel("\r\n");
//	ecl.SetSel(nStartChar, nEndChar, 1);
}

void CLogView::Clear() {
	CRichEditCtrl &ecl = this->GetRichEditCtrl();
	ecl.SetReadOnly(0);
	ecl.SetSel(0, -1);
	ecl.Clear();
	ecl.EmptyUndoBuffer();
	ecl.SetReadOnly(1);
}

// CLogView-Meldungshandler

void CLogView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.
	CRichEditCtrl &ecl = this->GetRichEditCtrl();
	ecl.SetReadOnly();
}
