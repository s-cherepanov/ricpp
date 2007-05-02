// MainFrm.cpp : Implementierung der Klasse CMainFrame
//

#include "stdafx.h"
#include "RIBLoader.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Konstruktion/Zerstörung

CMainFrame::CMainFrame()
{
	// ZU ERLEDIGEN: Hier Code zur Member-Initialisierung einfügen
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.
	RECT rectWin = {0, 0, 512, 384};
	DWORD dwStyle = cs.style;
	DWORD dwExStyle = cs.dwExStyle;
	AdjustWindowRectEx(&rectWin, dwStyle, FALSE, dwExStyle);

	cs.cx = rectWin.right;
	cs.cy = rectWin.bottom;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Diagnose

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame Nachrichten-Handler

