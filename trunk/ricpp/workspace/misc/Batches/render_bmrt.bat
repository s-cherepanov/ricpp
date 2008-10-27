setlocal
if "%SHADERS%"=="" set SHADERS=%BMRTHOME%\Shaders
if "%1"=="" goto ribout
C:
cd \TEMP
echo Rendering "%~1" with BMRT
echo.
rendrib "%~1"
set outfile=""
if exist "C:\TEMP\%n1.tiff" (
  set outfile="C:\TEMP\%n1.tiff"
  goto view
)
if exist "C:\TEMP\%~n1.tif" (
  set outfile="C:\TEMP\%~n1.tif"
  goto view
)
if exist "C:\TEMP\ri.tif" set outfile="C:\TEMP\ri.tif"
:view
if NOT %outfile%=="" start iv %outfile%
goto ende
:ribout
echo Rendering C:\temp\ribout.rib with BMRT
echo.
C:
cd \temp
rendrib C:\temp\ribout.rib
if exist C:\temp\ribout.tif iv C:\temp\ribout.tif
if exist C:\temp\ribout.tiff iv C:\temp\ribout.tiff
goto ende
:ende
endlocal