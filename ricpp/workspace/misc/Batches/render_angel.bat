c:
cd C:\temp
if "%~1"=="" goto ribout
echo Rendering "%~1" with ANGEL
echo.
angel "%~1"
goto ende
:ribout
echo Rendering C:\temp\ribout.rib with ANGEL
echo.
rndr C:\temp\ribout.rib
goto ende
:ende
