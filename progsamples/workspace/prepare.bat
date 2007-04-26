@echo off

echo Preparing vc6 and vc7 directories
echo Copy Batches and Ribs

REM Source for Demo

copy ..\..\ricpp\source\ricpptest\ricpptest.cpp ..\ricpptest

REM VC6 files

if not exist .\vc6\ricpptest\Debug\ mkdir .\vc6\ricpptest\Debug

if not exist .\vc6\ricpptest\Debug\Batches\ mkdir .\vc6\ricpptest\Debug\Batches
if not exist .\vc6\ricpptest\Debug\Contexts\ mkdir .\vc6\ricpptest\Debug\Contexts
if not exist .\vc6\ricpptest\Debug\Ribs\ mkdir .\vc6\ricpptest\Debug\Ribs

copy ..\..\ricpp\workspace\misc\Batches\*.bat .\vc6\ricpptest\Debug\Batches
copy ..\..\ricpp\workspace\misc\Ribs\*.rib .\vc6\ricpptest\Debug\Ribs

if not exist .\vc6\ricpptest\Release\ mkdir .\vc6\ricpptest\Release

if not exist .\vc6\ricpptest\Release\Batches\ mkdir .\vc6\ricpptest\Release\Batches
if not exist .\vc6\ricpptest\Release\Contexts\ mkdir .\vc6\ricpptest\Release\Contexts
if not exist .\vc6\ricpptest\Release\Ribs\ mkdir .\vc6\ricpptest\Release\Ribs

copy ..\..\ricpp\workspace\misc\Batches\*.bat .\vc6\ricpptest\Release\Batches
copy ..\..\ricpp\workspace\misc\Ribs\*.rib .\vc6\ricpptest\Release\Ribs

REM VC7 files

if not exist .\vc7\ricpptest\Debug\ mkdir .\vc7\ricpptest\Debug

if not exist .\vc7\ricpptest\Debug\Batches\ mkdir .\vc7\ricpptest\Debug\Batches
if not exist .\vc7\ricpptest\Debug\Contexts\ mkdir .\vc7\ricpptest\Debug\Contexts
if not exist .\vc7\ricpptest\Debug\Ribs\ mkdir .\vc7\ricpptest\Debug\Ribs

copy ..\..\ricpp\workspace\misc\Batches\*.bat .\vc7\ricpptest\Debug\Batches
copy ..\..\ricpp\workspace\misc\Ribs\*.rib .\vc7\ricpptest\Debug\Ribs

if not exist .\vc7\ricpptest\Release\ mkdir .\vc7\ricpptest\Release

if not exist .\vc7\ricpptest\Release\Batches\ mkdir .\vc7\ricpptest\Release\Batches
if not exist .\vc7\ricpptest\Release\Contexts\ mkdir .\vc7\ricpptest\Release\Contexts
if not exist .\vc7\ricpptest\Release\Ribs\ mkdir .\vc7\ricpptest\Release\Ribs

copy ..\..\ricpp\workspace\misc\Batches\*.bat .\vc7\ricpptest\Release\Batches
copy ..\..\ricpp\workspace\misc\Ribs\*.rib .\vc7\ricpptest\Release\Ribs

REM DevCPP files

if not exist .\DevCPP\testrib\Debug\out mkdir .\DevCPP\testrib\Debug\out
if not exist .\DevCPP\testrib\Debug\obj mkdir .\DevCPP\testrib\Debug\obj

if not exist .\DevCPP\testrib\Release\out mkdir .\DevCPP\testrib\Release\out
if not exist .\DevCPP\testrib\Release\obj mkdir .\DevCPP\testrib\Release\obj
