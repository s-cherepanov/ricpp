# Microsoft Developer Studio Project File - Name="qt3Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=qt3Test - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "qt3Test.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "qt3Test.mak" CFG="qt3Test - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "qt3Test - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "qt3Test - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "qt3Test - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "$(QT3DIR)\include" /I "..\..\..\source" /I ".\..\..\3rdparty\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /D "NO_DEBUG" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib imm32.lib wsock32.lib winmm.lib $(QT3DIR)\lib\qt-mteval323.lib $(QT3DIR)\lib\qtmain.lib /nologo /subsystem:windows /machine:I386 /out:"../exe/Release/qt3Test.exe" /libpath:".\..\..\3rdparty\lib"

!ELSEIF  "$(CFG)" == "qt3Test - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(QT3DIR)\include" /I "..\..\..\source" /I ".\..\..\3rdparty\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "QT_DLL" /D "QT_THREAD_SUPPORT" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib imm32.lib wsock32.lib winmm.lib $(QT3DIR)\lib\qt-mteval323.lib $(QT3DIR)\lib\qtmain.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /out:"../exe/Debug/qt3Test.exe" /pdbtype:sept /libpath:".\..\..\3rdparty\lib"

!ENDIF 

# Begin Target

# Name "qt3Test - Win32 Release"
# Name "qt3Test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\source\qtTest\lcdrange.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qtTest\main.cpp
# End Source File
# Begin Source File

SOURCE=moc_lcdrange.cpp
# End Source File
# Begin Source File

SOURCE=moc_teapot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qtTest\teapot.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\source\qtTest\lcdrange.h

!IF  "$(CFG)" == "qt3Test - Win32 Release"

# Begin Custom Build - Moc'ing lcdrange.h...
InputPath=..\..\..\source\qtTest\lcdrange.h

"moc_lcdrange.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QT3DIR%\bin\moc.exe ..\..\..\source\qtTest\lcdrange.h -o moc_lcdrange.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "qt3Test - Win32 Debug"

# Begin Custom Build - Moc'ing lcdrange.h...
InputPath=..\..\..\source\qtTest\lcdrange.h

"moc_lcdrange.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QT3DIR%\bin\moc.exe ..\..\..\source\qtTest\lcdrange.h -o moc_lcdrange.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\source\qtTest\teapot.h

!IF  "$(CFG)" == "qt3Test - Win32 Release"

# Begin Custom Build - Moc'ing teapot.h...
InputPath=..\..\..\source\qtTest\teapot.h

"moc_teapot.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QT3DIR%\bin\moc.exe ..\..\..\source\qtTest\teapot.h -o moc_teapot.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "qt3Test - Win32 Debug"

# Begin Custom Build - Moc'ing teapot.h...
InputPath=..\..\..\source\qtTest\teapot.h

"moc_teapot.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	%QT3DIR%\bin\moc.exe ..\..\..\source\qtTest\teapot.h -o moc_teapot.cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter "ui"
# End Group
# End Target
# End Project
