c:
cd C:\temp
if "%~1"=="" goto ribout
echo Rendering "%~1" with RDC
echo.
renderdc "%~1"
goto ende
:ribout
echo Rendering C:\temp\ribout.rib with RDC
echo.
renderdc C:\temp\ribout.rib
goto ende
:ende
