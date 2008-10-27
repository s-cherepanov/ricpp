///////////////////////////////////////////////////////////////////////////////
// Name:        unnamed.h
// Purpose:     
// Author:      
// Modified by:
// Created:     
// Copyright:   
// Licence:     
///////////////////////////////////////////////////////////////////////////////

#ifndef __WXTEST_H

#include "rendererloader/rendererloader.h"

// The main Application

class MyCanvas;
class MainWindow;

class MainApp: public wxApp
{
  TRendererLoader *m_rendererLoader;
  public:
      // gl renderer
      TRi *m_ri;

      inline MainApp() : m_rendererLoader(NULL), m_ri(NULL) {}
      virtual ~MainApp();
      virtual bool OnInit();
      virtual void OnQuit();
};


// The main Window
class MainWindow : public wxFrame
{
  public:
      // Constructor
      MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);

      // Event handler (should *NOT* be virtual!)
      void OnQuit(wxCommandEvent &event);
      MainApp *m_pApp;
  private:
      // Any class wishing to process wxWindows events must use this macro
      DECLARE_EVENT_TABLE()

      wxTextCtrl *m_pTextCtrl;
      MyCanvas   *m_pCanvas;
      
      wxMenuBar  *m_pMenuBar;
      wxMenu     *m_pFileMenu;
      wxMenu     *m_pInfoMenu;

      // IDs for the menu command
      enum {
            MAIN_MENU_FILE_OPEN = 1,
            MAIN_MENU_FILE_SAVE,
            MAIN_MENU_FILE_QUIT,
            MAIN_MENU_INFO_ABOUT
      };
};

class MyCanvas : public wxWindow
{
public:
    MyCanvas( MainWindow *parent );

    void OnPaint(wxPaintEvent &event);

private:
    MainWindow *m_owner;
    DECLARE_EVENT_TABLE()
};

DECLARE_APP(MainApp)

#endif // __WXTEST_H

