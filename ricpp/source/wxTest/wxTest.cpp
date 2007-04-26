///////////////////////////////////////////////////////////////////////////////
// Name:        unnamed.cpp
// Purpose:     
// Author:      
// Modified by:
// Created:     
// Copyright:   
// Licence:     
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
   #include <wx/wx.h>
#endif

#include "wxTest.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "mondrian.xpm"
#endif


BEGIN_EVENT_TABLE(MainWindow, wxFrame)
   EVT_MENU(MAIN_MENU_FILE_QUIT, MainWindow::OnQuit)
END_EVENT_TABLE()

IMPLEMENT_APP(MainApp)


// Implementation

// Main Application
MainApp::~MainApp() {
    try {
      if ( m_ri ) {
         m_ri->errorHandler(m_ri->errorPrint);
         m_ri->begin();
      }
    } catch (TSystemException &e) {
        e.displayMessage();
    } catch (TRendererError &t) {
      t.displayMessage();
    } catch (...) {
        // Do nothing
    }

    try {
        delete m_rendererLoader;
    } catch (TSystemException &e) {
        e.displayMessage();
    } catch (...) {
        // Do nothing
    }

   m_rendererLoader = NULL;
   m_ri = NULL;
}

bool MainApp::OnInit()
{
   m_rendererLoader = NULL;
   m_ri = NULL;
   
   try {
       m_rendererLoader = new TRendererLoader("contexts");
       if ( !m_rendererLoader )
           return false;
   } catch (TSystemException &t) {
       t.displayMessage();
       if ( m_rendererLoader )
           delete m_rendererLoader;
       return false;
   } catch (...) {
       return false;
   }

   try {
      m_ri = m_rendererLoader->loadRenderer("glrenderer");
      if ( m_ri ) {
         m_ri->errorHandler(m_ri->errorPrint);
         m_ri->begin();
      } else {
         return false;
      }
   } catch (TSystemException &t) {
      t.displayMessage();
      return false;
   } catch (TRendererError &t) {
      t.displayMessage();
      return false;
   } catch (...) {
      return false;
   }

   MainWindow *win = new MainWindow(_T("Window"), wxPoint (100, 100),
     wxSize(450, 340));

   win->m_pApp = this;
   win->Show(TRUE);
   SetTopWindow(win);

   /* Do not return FALSE or the app will terminate immediately */
   return true;
}

void MainApp::OnQuit()
{
}

// Main Window

MainWindow::MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame((wxFrame *) NULL, -1, title, pos, size)
{
    SetIcon(wxICON(mondrian));

    // create a menu bars
    
    m_pFileMenu = new wxMenu();
    m_pFileMenu->Append(MAIN_MENU_FILE_OPEN, _T("&Open"), _T("Open an existing file"));
    m_pFileMenu->Append(MAIN_MENU_FILE_SAVE, _T("&Save"), _T("Save the content"));
    m_pFileMenu->AppendSeparator();
    m_pFileMenu->Append(MAIN_MENU_FILE_QUIT, _T("&Quit"), _T("Quit the application"));

    m_pInfoMenu = new wxMenu();
    m_pInfoMenu->Append(MAIN_MENU_INFO_ABOUT, _T("&About"), _T("Show information about the application"));

    // Add it to the menu bar
    m_pMenuBar  = new wxMenuBar();
    m_pMenuBar->Append(m_pFileMenu, _T("&File"));
    m_pMenuBar->Append(m_pInfoMenu, _T("&Info"));
    SetMenuBar(m_pMenuBar);

    // Create the status bar
    CreateStatusBar(3);
    SetStatusText(_T("Ready"), 0);

    // Edit
    // m_pTextCtrl = new wxTextCtrl(this, -1, _T("Type some text..."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

    // Drawing
    m_pCanvas = new MyCanvas( this );
}

void MainWindow::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    // Force the window to close
    Close(TRUE);
}

// ----------------------------------------------------------------------------
// MyCanvas
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(MyCanvas, wxWindow)
    EVT_PAINT  (MyCanvas::OnPaint)
END_EVENT_TABLE()

MyCanvas::MyCanvas(MainWindow *parent)
        : wxWindow(parent, -1)
{
    m_owner = parent;
}

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
   wxPaintDC dc(this);
   if ( m_owner && m_owner->m_pApp && m_owner->m_pApp->m_ri ) {
       TRi &ri = *(m_owner->m_pApp->m_ri);
   
       try {
           WXHDC hdc = dc.GetHDC();
           if ( !hdc )
              return;
           WXHWND hwnd = GetHWND();
           if ( !hwnd )
             return;

           ri.display("wxTest", ri.RI_FRAMEBUFFER, ri.RI_RGB,
                      "devicecontext", &hdc, "windowhandle", &hwnd, ri.RI_NULL);
           ri.clipping((RtFloat).1, (RtFloat)100.0);
           ri.frameBegin(1);
               RtFloat d = (RtFloat)39.6; // normal objektive 50mm on 35mm film
               ri.projection(ri.RI_PERSPECTIVE, ri.RI_FOV, &d, ri.RI_NULL);
               ri.worldBegin();
                   RtFloat intensity = (RtFloat)27.0;
                   ri.lightSource(ri.RI_POINTLIGHT, ri.RI_INTENSITY, &intensity, ri.RI_NULL);
                   ri.translate((RtFloat)0.0, (RtFloat)-1.0, (RtFloat)10.0);
                   ri.rotate((RtFloat)-135.0, (RtFloat)1.0, (RtFloat)0.0, (RtFloat)0.0);
                   ri.surface(ri.RI_MATTE, ri.RI_NULL);
                   ri.geometry("teapot", ri.RI_NULL);
               ri.worldEnd();
           ri.frameEnd();
       } catch (TSystemException &t) {
          t.displayMessage();
          try {
             ri.synchronize(ri.RI_ABORT);
          } catch (...) {
          }
       } catch (TRendererError &t) {
          t.displayMessage();
          try {
             ri.synchronize(ri.RI_ABORT);
          } catch (...) {
          }
       } catch (...) {
          try {
             ri.synchronize(ri.RI_ABORT);
          } catch (...) {
          }
       }
   }
}

