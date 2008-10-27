c:
cd C:\temp
if "%~1"=="" goto ribout
echo Rendering "%~1" with PIXIE
echo.
rndr "%~1"
goto ende
:ribout
echo Rendering C:\temp\ribout.rib with PIXIE
echo.
rndr C:\temp\ribout.rib
goto ende
:ende
