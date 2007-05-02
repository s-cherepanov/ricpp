@echo off
echo %*
echo.
rem pause
REM Calling rendering process
REM
if not exist c:\temp mkdir c:\temp
%~d1%
cd "%~1"
cd Batches
rem call render_3dlight.bat "%~2"
rem call render_air.bat "%~2"
call render_aqsis.bat "%~2"
rem call render_bmrt.bat "%~2"
rem call preview.bat "%~2"
rem call render_pixie.bat "%~2"
rem call render_rdc.bat "%~2"
rem call render_aqsis.bat "%~2"

REM Show the RIB file
notepad "%~2"
