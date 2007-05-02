// RIBPreview.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RIBPreview.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "RIBPreviewDoc.h"
#include "RIBPreviewView.h"

#include <fcntl.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRIBPreviewApp

BEGIN_MESSAGE_MAP(CRIBPreviewApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CRIBPreviewApp construction

CRIBPreviewApp::CRIBPreviewApp() : m_rendererLoader(NULL), m_ri(NULL)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	setlocale( LC_ALL, "C" );
}

// CRIBPreviewApp destruction

CRIBPreviewApp::~CRIBPreviewApp()
{
}


int CRIBPreviewApp::ExitInstance()
{
	// Destroy the renderer loader, also frees all renderer libs loaded.
	if ( m_rendererLoader ) {
		m_rendererLoader->freeAll();
		delete m_rendererLoader;
	}
	m_rendererLoader = NULL;
	return CWinApp::ExitInstance();
}

// The one and only CRIBPreviewApp object

CRIBPreviewApp theApp;

// CRIBPreviewApp initialization

BOOL CRIBPreviewApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Create a console window for output if there is none
	// s.a. INFO: Calling CRT Output Routines from a GUI Application [Q105305]
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	if ( hStdOutput == NULL ) {
		int hCrt;
		FILE *hf;

		AllocConsole();
		SetConsoleTitle("RIBPreview Console Window - Do not close!");
		hCrt = _open_osfhandle(
					(intptr_t) GetStdHandle(STD_OUTPUT_HANDLE),
					_O_TEXT
				);
		hf = _fdopen( hCrt, "w" );
		*stdout = *hf;
		int i = setvbuf( stdout, NULL, _IONBF, 0 ); 
	}

	// Initializes rendererloader from the Module directory
	try {
		m_rendererLoader = new TRendererLoader();
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

	// Loads the OpenGL Renderer, abort rendering if an error occurs
	try {
		m_ri = m_rendererLoader->loadRenderer("glrenderer");
		if ( m_ri )
			m_ri->errorHandler(m_ri->errorPrint);
	} catch (TSystemException &t) {
		t.displayMessage();
	} catch (TRendererError &t) {
		t.displayMessage();
	} catch (...) {
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	// SetRegistryKey(_T("RibPreview"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_RIBPreviewTYPE,
		RUNTIME_CLASS(CRIBPreviewDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CRIBPreviewView));
	AddDocTemplate(pDocTemplate);
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	// Enable DDE Execute open
	EnableShellOpen();
	// RegisterShellFileTypes(TRUE);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Don't open an empty document window
	if ( cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew )
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CRIBPreviewApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CRIBPreviewApp message handlers

