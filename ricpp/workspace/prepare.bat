@echo off

echo Preparing vc6, vc7, vc8, DevCpp and CBuilderX directories
echo Copy Batches and Ribs

echo.
echo ---------------------------------------------------------------------------
echo Prepare vc6 directory
echo ---------------------------------------------------------------------------

if not exist .\vc6\exe\ mkdir .\vc6\exe
if not exist .\vc6\exe\Debug\ mkdir .\vc6\exe\Debug
if not exist .\vc6\exe\Debug\Batches\ mkdir .\vc6\exe\Debug\Batches
if not exist .\vc6\exe\Debug\Contexts\ mkdir .\vc6\exe\Debug\Contexts
if not exist .\vc6\exe\Debug\Ribs\ mkdir .\vc6\exe\Debug\Ribs

copy .\misc\Batches\*.bat .\vc6\exe\Debug\Batches
copy .\misc\Ribs\*.rib .\vc6\exe\Debug\Ribs
copy .\3rdparty\bin\*.DLL .\vc6\exe\Debug

if not exist .\vc6\exe\Release\ mkdir .\vc6\exe\Release
if not exist .\vc6\exe\Release\Batches\ mkdir .\vc6\exe\Release\Batches
if not exist .\vc6\exe\Release\Contexts\ mkdir .\vc6\exe\Release\Contexts
if not exist .\vc6\exe\Release\Ribs\ mkdir .\vc6\exe\Release\Ribs

copy .\misc\Batches\*.bat .\vc6\exe\Release\Batches
copy .\misc\Ribs\*.rib .\vc6\exe\Release\Ribs
copy .\3rdparty\bin\*.DLL .\vc6\exe\Release

copy .\misc\*.bat .\vc6

echo.
echo ---------------------------------------------------------------------------
echo Prepare vc7 directory
echo ---------------------------------------------------------------------------

if not exist .\vc7\exe\ mkdir .\vc7\exe
if not exist .\vc7\exe\Debug\ mkdir .\vc7\exe\Debug
if not exist .\vc7\exe\Debug\Batches\ mkdir .\vc7\exe\Debug\Batches
if not exist .\vc7\exe\Debug\Contexts\ mkdir .\vc7\exe\Debug\Contexts
if not exist .\vc7\exe\Debug\Ribs\ mkdir .\vc7\exe\Debug\Ribs

copy .\misc\Batches\*.bat .\vc7\exe\Debug\Batches
copy .\misc\Ribs\*.rib .\vc7\exe\Debug\Ribs
copy .\3rdparty\bin\*.DLL .\vc7\exe\Debug

if not exist .\vc7\exe\Release\ mkdir .\vc7\exe\Release
if not exist .\vc7\exe\Release\Batches\ mkdir .\vc7\exe\Release\Batches
if not exist .\vc7\exe\Release\Contexts\ mkdir .\vc7\exe\Release\Contexts
if not exist .\vc7\exe\Release\Ribs\ mkdir .\vc7\exe\Release\Ribs

copy .\misc\Batches\*.bat .\vc7\exe\Release\Batches
copy .\misc\Ribs\*.rib .\vc7\exe\Release\Ribs
copy .\3rdparty\bin\*.DLL .\vc7\exe\Release

copy .\misc\*.bat .\vc7

echo.
echo ---------------------------------------------------------------------------
echo Prepare vc8 directory
echo ---------------------------------------------------------------------------

if not exist .\vc8\exe\ mkdir .\vc8\exe
if not exist .\vc8\exe\Debug\ mkdir .\vc8\exe\Debug
if not exist .\vc8\exe\Debug\Batches\ mkdir .\vc8\exe\Debug\Batches
if not exist .\vc8\exe\Debug\Contexts\ mkdir .\vc8\exe\Debug\Contexts
if not exist .\vc8\exe\Debug\Ribs\ mkdir .\vc8\exe\Debug\Ribs

copy .\misc\Batches\*.bat .\vc8\exe\Debug\Batches
copy .\misc\Ribs\*.rib .\vc8\exe\Debug\Ribs
copy .\3rdparty\bin\*.DLL .\vc8\exe\Debug

if not exist .\vc8\exe\Release\ mkdir .\vc8\exe\Release
if not exist .\vc8\exe\Release\Batches\ mkdir .\vc8\exe\Release\Batches
if not exist .\vc8\exe\Release\Contexts\ mkdir .\vc8\exe\Release\Contexts
if not exist .\vc8\exe\Release\Ribs\ mkdir .\vc8\exe\Release\Ribs

copy .\misc\Batches\*.bat .\vc8\exe\Release\Batches
copy .\misc\Ribs\*.rib .\vc8\exe\Release\Ribs
copy .\3rdparty\bin\*.DLL .\vc8\exe\Release

copy .\misc\*.bat .\vc8

echo.
echo ---------------------------------------------------------------------------
echo Prepare DevCPP directory
echo ---------------------------------------------------------------------------

if not exist .\DevCPP\exe\ mkdir .\DevCPP\exe
if not exist .\DevCPP\exe\Debug\ mkdir .\DevCPP\exe\Debug
if not exist .\DevCPP\exe\Debug\out mkdir .\DevCPP\exe\Debug\out
if not exist .\DevCPP\exe\Debug\out\Batches\ mkdir .\DevCPP\exe\Debug\out\Batches
if not exist .\DevCPP\exe\Debug\out\Contexts\ mkdir .\DevCPP\exe\Debug\out\Contexts
if not exist .\DevCPP\exe\Debug\out\Ribs\ mkdir .\DevCPP\exe\Debug\out\Ribs

if not exist .\DevCPP\glcpp\Debug\obj mkdir .\DevCPP\glcpp\Debug\obj
if not exist .\DevCPP\glcpp\Debug\out mkdir .\DevCPP\glcpp\Debug\out
if not exist .\DevCPP\libglrenderer\Debug\obj mkdir .\DevCPP\libglrenderer\Debug\obj
if not exist .\DevCPP\libglrenderer\Debug\out mkdir .\DevCPP\libglrenderer\Debug\out
if not exist .\DevCPP\glrenderer\Debug\obj mkdir .\DevCPP\glrenderer\Debug\obj
if not exist .\DevCPP\glrenderer\Debug\out mkdir .\DevCPP\glrenderer\Debug\out
if not exist .\DevCPP\libribout\Debug\obj mkdir .\DevCPP\libribout\Debug\obj
if not exist .\DevCPP\libribout\Debug\out mkdir .\DevCPP\libribout\Debug\out
if not exist .\DevCPP\ribout\Debug\obj mkdir .\DevCPP\ribout\Debug\obj
if not exist .\DevCPP\ribout\Debug\out mkdir .\DevCPP\ribout\Debug\out
if not exist .\DevCPP\libmesh\Debug\obj mkdir .\DevCPP\libmesh\Debug\obj
if not exist .\DevCPP\libmesh\Debug\out mkdir .\DevCPP\libmesh\Debug\out
if not exist .\DevCPP\mesh\Debug\obj mkdir .\DevCPP\mesh\Debug\obj
if not exist .\DevCPP\mesh\Debug\out mkdir .\DevCPP\mesh\Debug\out
if not exist .\DevCPP\rendercontext\Debug\obj mkdir .\DevCPP\rendercontext\Debug\obj
if not exist .\DevCPP\rendercontext\Debug\out mkdir .\DevCPP\rendercontext\Debug\out
if not exist .\DevCPP\rendererloader\Debug\obj mkdir .\DevCPP\rendererloader\Debug\obj
if not exist .\DevCPP\rendererloader\Debug\out mkdir .\DevCPP\rendererloader\Debug\out
if not exist .\DevCPP\ribparser\Debug\obj mkdir .\DevCPP\ribparser\Debug\obj
if not exist .\DevCPP\ribparser\Debug\out mkdir .\DevCPP\ribparser\Debug\out
if not exist .\DevCPP\ricpptest\Debug\obj mkdir .\DevCPP\ricpptest\Debug\obj
if not exist .\DevCPP\ricpptest\Debug\out mkdir .\DevCPP\ricpptest\Debug\out
if not exist .\DevCPP\wxTest\Debug\obj mkdir .\DevCPP\wxTest\Debug\obj
if not exist .\DevCPP\wxTest\Debug\out mkdir .\DevCPP\wxTest\Debug\out
if not exist .\DevCPP\testrib\Debug\obj mkdir .\DevCPP\testrib\Debug\obj
if not exist .\DevCPP\testrib\Debug\out mkdir .\DevCPP\testrib\Debug\out
rem if not exist .\DevCPP\qt3Test\Debug\obj mkdir .\DevCPP\qt3Test\Debug\obj
rem if not exist .\DevCPP\qt3Test\Debug\out mkdir .\DevCPP\qt3Test\Debug\out

copy .\misc\Batches\*.bat .\DevCPP\exe\Debug\out\Batches
copy .\misc\Ribs\*.rib .\DevCPP\exe\Debug\out\Ribs
copy .\3rdparty\bin\*.DLL .\DevCPP\exe\Debug\out

if not exist .\DevCPP\exe\Release\ mkdir .\DevCPP\exe\Release
if not exist .\DevCPP\exe\Release\out mkdir .\DevCPP\exe\Release\out
if not exist .\DevCPP\exe\Release\out\Batches\ mkdir .\DevCPP\exe\Release\out\Batches
if not exist .\DevCPP\exe\Release\out\Contexts\ mkdir .\DevCPP\exe\Release\out\Contexts
if not exist .\DevCPP\exe\Release\out\Ribs\ mkdir .\DevCPP\exe\Release\out\Ribs

if not exist .\DevCPP\glcpp\Release\obj mkdir .\DevCPP\glcpp\Release\obj
if not exist .\DevCPP\glcpp\Release\out mkdir .\DevCPP\glcpp\Release\out
if not exist .\DevCPP\libglrenderer\Release\obj mkdir .\DevCPP\libglrenderer\Release\obj
if not exist .\DevCPP\libglrenderer\Release\out mkdir .\DevCPP\libglrenderer\Release\out
if not exist .\DevCPP\glrenderer\Release\obj mkdir .\DevCPP\glrenderer\Release\obj
if not exist .\DevCPP\glrenderer\Release\out mkdir .\DevCPP\glrenderer\Release\out
if not exist .\DevCPP\libribout\Release\obj mkdir .\DevCPP\libribout\Release\obj
if not exist .\DevCPP\libribout\Release\out mkdir .\DevCPP\libribout\Release\out
if not exist .\DevCPP\ribout\Release\obj mkdir .\DevCPP\ribout\Release\obj
if not exist .\DevCPP\ribout\Release\out mkdir .\DevCPP\ribout\Release\out
if not exist .\DevCPP\libmesh\Release\obj mkdir .\DevCPP\libmesh\Release\obj
if not exist .\DevCPP\libmesh\Release\out mkdir .\DevCPP\libmesh\Release\out
if not exist .\DevCPP\mesh\Release\obj mkdir .\DevCPP\mesh\Release\obj
if not exist .\DevCPP\mesh\Release\out mkdir .\DevCPP\mesh\Release\out
if not exist .\DevCPP\rendercontext\Release\obj mkdir .\DevCPP\rendercontext\Release\obj
if not exist .\DevCPP\rendercontext\Release\out mkdir .\DevCPP\rendercontext\Release\out
if not exist .\DevCPP\rendererloader\Release\obj mkdir .\DevCPP\rendererloader\Release\obj
if not exist .\DevCPP\rendererloader\Release\out mkdir .\DevCPP\rendererloader\Release\out
if not exist .\DevCPP\ribparser\Release\obj mkdir .\DevCPP\ribparser\Release\obj
if not exist .\DevCPP\ribparser\Release\out mkdir .\DevCPP\ribparser\Release\out
if not exist .\DevCPP\ricpptest\Release\obj mkdir .\DevCPP\ricpptest\Release\obj
if not exist .\DevCPP\ricpptest\Release\out mkdir .\DevCPP\ricpptest\Release\out
if not exist .\DevCPP\wxTest\Release\obj mkdir .\DevCPP\wxTest\Release\obj
if not exist .\DevCPP\wxTest\Release\out mkdir .\DevCPP\wxTest\Release\out
if not exist .\DevCPP\testrib\Release\obj mkdir .\DevCPP\testrib\Release\obj
if not exist .\DevCPP\testrib\Release\out mkdir .\DevCPP\testrib\Release\out
rem if not exist .\DevCPP\qt3Test\Release\obj mkdir .\DevCPP\qt3Test\Release\obj
rem if not exist .\DevCPP\qt3Test\Release\out mkdir .\DevCPP\qt3Test\Release\out

copy .\misc\Batches\*.bat .\DevCPP\exe\Release\out\Batches
copy .\misc\Ribs\*.rib .\DevCPP\exe\Release\out\Ribs
copy .\3rdparty\bin\*.DLL .\DevCPP\exe\Release\out

REM don't copy misc\*.bat


echo.
echo ---------------------------------------------------------------------------
echo Prepare CBuilderX directory
echo ---------------------------------------------------------------------------

if not exist .\CBuilderX\build\ mkdir .\CBuilderX\build
if not exist .\CBuilderX\build\gnuwin32 mkdir .\CBuilderX\build\gnuwin32

if not exist .\CBuilderX\build\gnuwin32\Debug\ mkdir .\CBuilderX\build\gnuwin32\Debug
if not exist .\CBuilderX\build\gnuwin32\Debug\exe\ mkdir .\CBuilderX\build\gnuwin32\Debug\exe
if not exist .\CBuilderX\build\gnuwin32\Debug\exe\Batches\ mkdir .\CBuilderX\build\gnuwin32\Debug\exe\Batches
if not exist .\CBuilderX\build\gnuwin32\Debug\exe\Contexts\ mkdir .\CBuilderX\build\gnuwin32\Debug\exe\Contexts
if not exist .\CBuilderX\build\gnuwin32\Debug\exe\Ribs\ mkdir .\CBuilderX\build\gnuwin32\Debug\exe\Ribs

copy .\misc\Batches\*.bat .\CBuilderX\build\gnuwin32\Debug\exe\Batches
copy .\misc\Ribs\*.rib .\CBuilderX\build\gnuwin32\Debug\exe\Ribs
copy .\3rdparty\bin\*.DLL .\CBuilderX\build\gnuwin32\Debug\exe

if not exist .\CBuilderX\build\gnuwin32\Release\ mkdir .\CBuilderX\build\gnuwin32\Release
if not exist .\CBuilderX\build\gnuwin32\Release\exe\ mkdir .\CBuilderX\build\gnuwin32\Release\exe
if not exist .\CBuilderX\build\gnuwin32\Release\exe\Batches\ mkdir .\CBuilderX\build\gnuwin32\Release\exe\Batches
if not exist .\CBuilderX\build\gnuwin32\Release\exe\Contexts\ mkdir .\CBuilderX\build\gnuwin32\Release\exe\Contexts
if not exist .\CBuilderX\build\gnuwin32\Release\exe\Ribs\ mkdir .\CBuilderX\build\gnuwin32\Release\exe\Ribs

copy .\misc\Batches\*.bat .\CBuilderX\build\gnuwin32\Release\exe\Batches
copy .\misc\Ribs\*.rib .\CBuilderX\build\gnuwin32\Release\exe\Ribs
copy .\3rdparty\bin\*.DLL .\CBuilderX\build\gnuwin32\Release\exe

REM don't copy misc\*.bat

echo.
echo ---------------------------------------------------------------------------
echo Configure DevCPP (MinGW) using .\DevCPP\ricpp.def
echo If you don't use DevCPP (MinGW), simply close the Notepad Windows
echo If you want to use DevCPP (MinGW), please enter the path
if exist .\DevCPP\ricpp.def copy .\DevCPP\ricpp.def .\DevCPP\ricpp.def.bak
if exist .\DevCPP\ricpp.def echo The old .\DevCPP\ricpp.def is copied to .\DevCPP\ricpp.def.bak
echo ---------------------------------------------------------------------------
copy .\DevCPP\ricpp.def.temp .\DevCPP\ricpp.def
notepad .\DevCPP\ricpp.def
