c:
cd C:\temp
if "%~1"=="" goto ribout
echo Rendering "%~1" with AIR
echo.
air "%~1"
goto ende
:ribout
echo Rendering C:\temp\ribout.rib with AIR
echo.
air C:\temp\ribout.rib
goto ende
:ende
