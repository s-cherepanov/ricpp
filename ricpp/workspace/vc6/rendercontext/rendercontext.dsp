# Microsoft Developer Studio Project File - Name="rendercontext" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=rendercontext - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "rendercontext.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "rendercontext.mak" CFG="rendercontext - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "rendercontext - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "rendercontext - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rendercontext - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\..\..\..\source" /I ".\..\..\3rdparty\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\librendercontext.lib"

!ELSEIF  "$(CFG)" == "rendercontext - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\..\..\..\source" /I ".\..\..\3rdparty\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\librendercontextd.lib"

!ENDIF 

# Begin Target

# Name "rendercontext - Win32 Release"
# Name "rendercontext - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\source\rendercontext\baseclass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\declarationmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\displaydesc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\light.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\matrix3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\optionmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\parameter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\parameterdeclaration.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\parametermap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\parametric.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\pathstack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\polygons.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\renderattributes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\rendercontext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\renderoptions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\ricppbinding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\rimacro.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\riscene.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\shader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\subdivision.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\surfaces.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\trimcurve.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\validcontext.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\source\rendercontext\baseclass.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\declarationmap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\displaydesc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\handles.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\light.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\matrix3d.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\optionmap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\parameter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\parameterdeclaration.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\parametermap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\parametric.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\pathstack.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\polygons.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\renderattributes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\rendercontext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\renderoptions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\ricppbinding.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\rimacro.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\riscene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\shader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\subdivision.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\surfaces.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\trimcurve.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\rendercontext\validcontext.h
# End Source File
# End Group
# Begin Group "Documentation Files"

# PROP Default_Filter "html"
# End Group
# End Target
# End Project
