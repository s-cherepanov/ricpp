setlocal
if "%SHADERS%"=="" set SHADERS=%BMRTHOME%\Shaders
c:
cd C:\temp
if "%~1"=="" goto ribout
echo Rendering "%~1" with BMRT-RGL
echo.
rgl "%~1"
goto ende
:ribout
echo Rendering C:\temp\ribout.rib with BMRT-RGL
echo.
rgl C:\temp\ribout.rib
goto ende
:ende
endlocal