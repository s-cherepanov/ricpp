if "%~1"=="" goto ribout
echo Rendering %* with AQSIS
echo.
aqsis %*
goto ende
:ribout
echo Rendering C:\temp\ribout.rib with AQSIS
echo.
C:
cd \temp
aqsis C:\temp\ribout.rib
goto ende
:ende
