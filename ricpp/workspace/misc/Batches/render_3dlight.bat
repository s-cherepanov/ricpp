c:
cd C:\temp
if "%~1"=="" goto ribout
echo Rendering "%~1" with 3Dlight
echo.
renderdl "%~1"
goto ende
:ribout
echo Rendering C:\temp\ribout.rib with 3Dlight
echo.
renderdl C:\temp\ribout.rib
goto ende
:ende
