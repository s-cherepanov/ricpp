// RIBPreview.cpp : Legt das Klassenverhalten für die Anwendung fest.
//

#include "stdafx.h"
#include "RIBPreview.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "RIBPreviewDoc.h"
#include "RIBPreviewView.h"

#include "LogView.h"
#include "LogFrm.h"

#include <fcntl.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewApp

BEGIN_MESSAGE_MAP(CRIBPreviewApp, CWinApp)
	//{{AFX_MSG_MAP(CRIBPreviewApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
	// Dateibasierte Standard-Dokumentbefehle
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_WINDOW_ERRORLOG, OnWindowErrorlog)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_ERRORLOG, OnUpdateWindowErrorlog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewApp Konstruktion

CRIBPreviewApp::CRIBPreviewApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
	setlocale( LC_ALL, "C" );
	m_rendererLoader = NULL;
	m_rc = NULL;
	m_rib = NULL;
	m_log = NULL;
}

CRIBPreviewApp::~CRIBPreviewApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// Das einzige CRIBPreviewApp-Objekt

CRIBPreviewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewApp Initialisierung

RtVoid CRIBPreviewApp::errorPrint(RtInt code, RtInt severity, const char *message) {
	if ( theApp.m_log ) {
		theApp.m_log->errorPrint(code, severity, message);
	}
}

BOOL CRIBPreviewApp::InitInstance()
{
	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht nutzen und die Größe Ihrer fertigen 
	//  ausführbaren Datei reduzieren wollen, sollten Sie die nachfolgenden
	//  spezifischen Initialisierungsroutinen, die Sie nicht benötigen, entfernen.

#if _MSC_VER < 0x0500
#ifdef _AFXDLL
	Enable3dControls();			// Diese Funktion bei Verwendung von MFC in gemeinsam genutzten DLLs aufrufen
#else
	Enable3dControlsStatic();	// Diese Funktion bei statischen MFC-Anbindungen aufrufen
#endif
#endif

	// Create a console window for output if there is none
	// s.a. INFO: Calling CRT Output Routines from a GUI Application [Q105305]
	/*
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if ( hStdOutput == NULL ) {
		int hCrt;
		FILE *hf;

		AllocConsole();
		SetConsoleTitle("RIBPreview Console Window - Do not close!");
		hCrt = _open_osfhandle(
					(long) GetStdHandle(STD_OUTPUT_HANDLE),
					_O_TEXT
				);
		hf = _fdopen( hCrt, "w" );
		*stdout = *hf;
		int i = setvbuf( stdout, NULL, _IONBF, 0 ); 
	}
	*/

	// Ändern des Registrierungsschlüssels, unter dem unsere Einstellungen gespeichert sind.
	// ZU ERLEDIGEN: Sie sollten dieser Zeichenfolge einen geeigneten Inhalt geben
	// wie z.B. den Namen Ihrer Firma oder Organisation.
	// SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Standard INI-Dateioptionen laden (einschließlich MRU)

    // Mask blank
	char path[512];
	DWORD fsize = GetModuleFileName(NULL, path, sizeof(path));
    while ( fsize-- ) {
        if ( path[fsize] == '\\' ) {
            path[fsize] = (char)0;
            break;
        }
    }
	DWORD i;
	for ( i=0; path[i]; /**/ ) {
        if ( path[i] == ' ' ) {
            m_modulepath += '^';
        }
		m_modulepath += path[i++];
    }

	// Initializes rendererloader for subdirectory "contexts"
	try {
		m_rendererLoader = new TRendererLoader("contexts");
		if ( !m_rendererLoader )
			return FALSE;
	} catch (TSystemException &t) {
		t.displayMessage();
		if ( m_rendererLoader )
			delete m_rendererLoader;
		return FALSE;
	} catch (...) {
		return FALSE;
	}

	try {
		m_rc = m_rendererLoader->loadRenderer("glrenderer");
		if ( m_rc )
			m_rc->errorHandler(this->errorPrint);
	} catch (TSystemException &t) {
		t.displayMessage();
	} catch (TRendererError &t) {
		t.displayMessage();
	} catch (...) {
	}

	try {
		m_rib = m_rendererLoader->loadRenderer("ribout");
		if ( m_rib )
			m_rib->errorHandler(this->errorPrint);
	} catch (TSystemException &t) {
		t.displayMessage();
	} catch (TRendererError &t) {
		t.displayMessage();
	} catch (...) {
	}

	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_RIBTYPE,
		RUNTIME_CLASS(CRIBPreviewDoc),
		RUNTIME_CLASS(CChildFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CRIBPreviewView));
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(
		IDR_LOGTYPE,
		RUNTIME_CLASS(CLogDoc),
		RUNTIME_CLASS(CLogFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CLogView));
	AddDocTemplate(pDocTemplate);

	// Haupt-MDI-Rahmenfenster erzeugen
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Öffnen per DragDrop aktivieren
	m_pMainWnd->DragAcceptFiles();

	// DDE-Execute-Open aktivieren
	EnableShellOpen();
	// RegisterShellFileTypes(TRUE);

	// Befehlszeile parsen, um zu prüfen auf Standard-Umgebungsbefehle DDE, Datei offen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if ( cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew )
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Verteilung der in der Befehlszeile angegebenen Befehle
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
   
	pDocTemplate->OpenDocumentFile(NULL);

	// Das Hauptfenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CRIBPreviewApp::ExitInstance() 
{
	int rval = 0;
	__try {
		if ( m_rendererLoader ) {
			m_rendererLoader->freeAll();
			delete m_rendererLoader;
		}
		m_rendererLoader = NULL;
	} __except ( EXCEPTION_EXECUTE_HANDLER ){
		rval = 0;
	}

	__try {
		rval = CWinApp::ExitInstance();
	} __except ( EXCEPTION_EXECUTE_HANDLER ){
		rval = 0;
	}

	return rval;
}


void CRIBPreviewApp::ClearLog() {
	if ( m_log ) {
		m_log->Clear();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialog für Info über Anwendung

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogdaten
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// Überladungen für virtuelle Funktionen, die vom Anwendungs-Assistenten erzeugt wurden
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	//{{AFX_MSG(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// Keine Nachrichten-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Anwendungsbefehl zum Ausführen des Dialogfelds
void CRIBPreviewApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CRIBPreviewApp-Nachrichtenbehandlungsroutinen

void CRIBPreviewApp::OnWindowErrorlog()
{
	// TODO: Fügen Sie hier Ihren Befehlsbehandlungscode ein.
	if ( m_log ) {
		m_log->HideView(m_log->VisibleView());
	}
}

void CRIBPreviewApp::OnUpdateWindowErrorlog(CCmdUI *pCmdUI)
{
	// TODO: Fügen Sie hier Ihren Befehlsaktualisierungs-UI-Behandlungscode ein.
	pCmdUI->Enable(m_log ? TRUE : FALSE);
	pCmdUI->SetCheck(m_log && m_log->VisibleView() ? TRUE : FALSE);
}
