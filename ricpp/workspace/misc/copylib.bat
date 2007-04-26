@echo off

setlocal

REM Copies current files to ricpplib
REM

rem lib package
REM

set RICPPTO=.\ricpplib

if not exist .\exe\ goto ERROR

if exist %RICPPTO% rmdir /S /Q %RICPPTO%

if not exist %RICPPTO% mkdir %RICPPTO%
if not exist %RICPPTO%\lib mkdir %RICPPTO%\lib
if not exist %RICPPTO%\dll mkdir %RICPPTO%\dll
if not exist %RICPPTO%\dll\Debug mkdir %RICPPTO%\dll\Debug
if not exist %RICPPTO%\dll\Release mkdir %RICPPTO%\dll\Release
if not exist %RICPPTO%\include mkdir %RICPPTO%\include

copy .\exe\debug\contexts\*.dll %RICPPTO%\dll\Debug
copy .\exe\release\contexts\*.dll %RICPPTO%\dll\Release
copy ..\..\source\rendererloader\sysexcept.h %RICPPTO%\include
copy ..\..\source\rendererloader\ricpp.h %RICPPTO%\include
copy ..\..\source\rendererloader\rendererloader.h %RICPPTO%\include
copy .\rendererloader\debug\librendererloaderd.lib %RICPPTO%\lib\librendererloaderd.lib
copy .\rendererloader\release\librendererloader.lib %RICPPTO%\lib\librendererloader.lib

goto END

:ERROR
echo Call only in workspace directories, use prepare.bat to copy
echo to appropriate places.

:END
endlocal
