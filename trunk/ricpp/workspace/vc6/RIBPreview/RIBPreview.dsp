# Microsoft Developer Studio Project File - Name="RIBPreview" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RIBPreview - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "RIBPreview.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "RIBPreview.mak" CFG="RIBPreview - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "RIBPreview - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "RIBPreview - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RIBPreview - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\..\..\..\source" /I ".\..\..\3rdparty\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:".\..\exe\Release\RIBPreview.exe" /libpath:".\..\..\3rdparty\lib"

!ELSEIF  "$(CFG)" == "RIBPreview - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\..\..\..\source" /I ".\..\..\3rdparty\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:".\..\exe\Debug\RIBPreview.exe" /pdbtype:sept /libpath:".\..\..\3rdparty\lib"

!ENDIF 

# Begin Target

# Name "RIBPreview - Win32 Release"
# Name "RIBPreview - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\cntritem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\LogDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\LogFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\LogView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\matrix3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\RIBPreview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\RIBPreview.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\RIBPreviewDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\RIBPreviewView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\cntritem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\LogDoc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\LogFrm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\LogView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\matrix3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\RIBPreview.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\RIBPreviewDoc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\RIBPreviewView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\StdAfx.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\res\idr_ribt.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\res\RIBPreview.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\res\RIBPreview.rc2
# End Source File
# Begin Source File

SOURCE=..\..\..\source\RIBPreview\res\RIBPreviewDoc.ico
# End Source File
# End Group
# Begin Group "RibFiles"

# PROP Default_Filter "rib"
# Begin Source File

SOURCE=..\..\..\..\ribsamples\bulb.rib
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ribsamples\cubits.rib
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ribsamples\morph.rib
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ribsamples\quadsmp.rib
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ribsamples\subdiv.rib
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ribsamples\subdiv2.rib
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ribsamples\testsubdiv2.rib
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ribsamples\tetra.rib
# End Source File
# End Group
# Begin Group "Documentation Files"

# PROP Default_Filter "html"
# End Group
# Begin Source File

SOURCE=.\..\..\..\subprojects\RIBPreview\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\..\..\..\subprojects\RIBPreview\RIBPreview.reg
# End Source File
# End Target
# End Project
