// RIBLoader.cpp : Legt das Klassenverhalten für die Anwendung fest.
//

#include "stdafx.h"
#include "RIBLoader.h"
#include "MainFrm.h"
#include "RIBLoaderDoc.h"
#include "RIBLoaderView.h"

#include <fcntl.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderApp

BEGIN_MESSAGE_MAP(CRIBLoaderApp, CWinApp)
	//{{AFX_MSG_MAP(CRIBLoaderApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG_MAP
	// Dateibasierte Standard-Dokumentbefehle
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderApp Konstruktion

CRIBLoaderApp::CRIBLoaderApp()
{
	// ZU ERLEDIGEN: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance platzieren
	m_rendererLoader = NULL;
}

CRIBLoaderApp::~CRIBLoaderApp()
{
}

int CRIBLoaderApp::ExitInstance()
{
	if ( m_rendererLoader ) {
		delete m_rendererLoader;
	}
	m_rendererLoader = NULL;
	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// Das einzige CRIBLoaderApp-Objekt

CRIBLoaderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderApp Initialisierung

BOOL CRIBLoaderApp::InitInstance()
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

	// Ändern des Registrierungsschlüssels, unter dem unsere Einstellungen gespeichert sind.
	// ZU ERLEDIGEN: Sie sollten dieser Zeichenfolge einen geeigneten Inhalt geben
	// wie z.B. den Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Standard INI-Dateioptionen laden (einschließlich MRU)

	// Create a console window for output if there is none
	// s.a. INFO: Calling CRT Output Routines from a GUI Application [Q105305]
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if ( hStdOutput == NULL ) {
		int hCrt;
		FILE *hf;

		AllocConsole();
		SetConsoleTitle("RIBLoader Console Window - Do not close!");
		hCrt = _open_osfhandle(
					(long) GetStdHandle(STD_OUTPUT_HANDLE),
					_O_TEXT
				);
		hf = _fdopen( hCrt, "w" );
		*stdout = *hf;
		int i = setvbuf( stdout, NULL, _IONBF, 0 ); 
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
		::MessageBox(NULL, "Error while creating renderer loader", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
		return FALSE;
	}

	// Load the interface for OpenGL output
	try {
		m_rc = m_rendererLoader->loadRenderer("glrenderer");
		if ( m_rc )
			m_rc->errorHandler(m_rc->errorPrint);
	} catch (TSystemException &t) {
		t.displayMessage();
	} catch (TRendererError &t) {
		t.displayMessage();
	} catch (...) {
		::MessageBox(NULL, "Error loading context\\glrenderer.dll", "ERROR", MB_ICONEXCLAMATION|MB_OK) ;
	}

	// Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRIBLoaderDoc),
		RUNTIME_CLASS(CMainFrame),       // Haupt-SDI-Rahmenfenster
		RUNTIME_CLASS(CRIBLoaderView));
	AddDocTemplate(pDocTemplate);

	// DDE-Execute-Open aktivieren
	EnableShellOpen();
	// RegisterShellFileTypes(TRUE);

	// Befehlszeile parsen, um zu prüfen auf Standard-Umgebungsbefehle DDE, Datei offen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Verteilung der in der Befehlszeile angegebenen Befehle
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Das einzige Fenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Öffnen mit Drag & Drop aktivieren
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
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
void CRIBLoaderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CRIBLoaderApp-Nachrichtenbehandlungsroutinen
