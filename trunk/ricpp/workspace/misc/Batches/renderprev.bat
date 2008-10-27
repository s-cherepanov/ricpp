@echo off
echo %*
echo.
rem pause
REM Calling rendering process
REM
REM if not exist c:\temp mkdir c:\temp
%~d2%
cd "%~p2"
rem %~d1%
rem cd "%~1"
rem cd Batches
rem call "%~1\Batches\render_3dlight.bat" "%~2"
rem call "%~1\Batches\render_air.bat" "%~2"
call "%~1\Batches\render_aqsis.bat" -progress -pause "%~2"
rem call "%~1\Batches\render_aqsis.bat" -progress -verbose -pause -type="framebuffer" -mode="rgba" "%~2"
rem call "%~1\Batches\render_bmrt.bat" "%~2"
rem call "%~1\Batches\preview.bat" "%~2"
rem call "%~1\Batches\render_pixie.bat" "%~2"
rem call "%~1\Batches\render_rdc.bat" "%~2"
rem call "%~1\Batches\render_angel.bat" "%~2"
rem pause