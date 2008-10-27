// ChildFrm.cpp : Implementierung der Klasse CLogFrame
//

#include "stdafx.h"
#include "RIBPreview.h"

#include "LogFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogFrame

IMPLEMENT_DYNCREATE(CLogFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CLogFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLogFrame)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_NEW, OnWindowNew)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEW, OnUpdateWindowNew)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogFrame Konstruktion/Destruktion

CLogFrame::CLogFrame()
{
	// ZU ERLEDIGEN: Hier Code zur Member-Initialisierung einfügen
	
}

CLogFrame::~CLogFrame()
{
}

BOOL CLogFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLogFrame Diagnose

#ifdef _DEBUG
void CLogFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CLogFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogFrame Nachrichten-Handler

void CLogFrame::OnClose()
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.
	// CloseWindow();
	ShowWindow(SW_HIDE);
	// CMDIChildWnd::OnClose();
}

void CLogFrame::OnWindowNew()
{
	// TODO: Fügen Sie hier Ihren Befehlsbehandlungscode ein.
}

void CLogFrame::OnUpdateWindowNew(CCmdUI *pCmdUI)
{
	// TODO: Fügen Sie hier Ihren Befehlsaktualisierungs-UI-Behandlungscode ein.
	if ( pCmdUI )
		pCmdUI->Enable(FALSE);
}
