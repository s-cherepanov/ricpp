; CLW-Datei enthält Informationen für den MFC-Klassen-Assistenten

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "RIBPreview.h"
LastPage=0

ClassCount=6
Class1=CRIBPreviewApp
Class2=CRIBPreviewDoc
Class3=CRIBPreviewView
Class4=CMainFrame

ResourceCount=11
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDR_RIBTYPE
Resource8=IDR_MAINFRAME (English (U.S.))
Resource9=IDR_RIBTYPE (English (U.S.))
Class5=CChildFrame
Class6=CAboutDlg
Resource10=IDR_LOGTYPE (English (U.S.))
Resource11=IDD_ABOUTBOX (English (U.S.))

[CLS:CRIBPreviewApp]
Type=0
HeaderFile=RIBPreview.h
ImplementationFile=RIBPreview.cpp
Filter=N

[CLS:CRIBPreviewDoc]
Type=0
HeaderFile=RIBPreviewDoc.h
ImplementationFile=RIBPreviewDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CRIBPreviewDoc

[CLS:CRIBPreviewView]
Type=0
HeaderFile=RIBPreviewView.h
ImplementationFile=RIBPreviewView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CRIBPreviewView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CMDIFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=RIBPreview.cpp
ImplementationFile=RIBPreview.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command4=ID_APP_EXIT
Command5=ID_APP_ABOUT
CommandCount=5

CommandCount=5
Command3=ID_FILE_MRU_FILE1

[MNU:IDR_RIBTYPE]
Type=1
Class=CRIBPreviewView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_WINDOW_NEW
Command13=ID_WINDOW_CASCADE
Command14=ID_WINDOW_TILE_HORZ
CommandCount=16
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_UNDO
Command15=ID_WINDOW_ARRANGE
Command16=ID_APP_ABOUT

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_MRU_FILE1
Command3=ID_APP_EXIT
Command4=ID_WINDOW_ERRORLOG
Command5=ID_APP_ABOUT
CommandCount=5

[MNU:IDR_RIBTYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_RELOAD
Command3=ID_FILE_RENDER
Command4=ID_FILE_CLOSE
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_WINDOW_NEW
Command8=ID_WINDOW_CASCADE
Command9=ID_WINDOW_TILE_HORZ
Command10=ID_WINDOW_ARRANGE
Command11=ID_WINDOW_ERRORLOG
Command12=ID_APP_ABOUT
CommandCount=12

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_RELOAD
Command3=ID_NEXT_PANE
Command4=ID_PREV_PANE
Command5=ID_FILE_RENDER
CommandCount=5

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_LOGTYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_CLOSE
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_WINDOW_NEW
Command6=ID_WINDOW_CASCADE
Command7=ID_WINDOW_TILE_HORZ
Command8=ID_WINDOW_ARRANGE
Command9=ID_WINDOW_ERRORLOG
Command10=ID_APP_ABOUT
CommandCount=10

