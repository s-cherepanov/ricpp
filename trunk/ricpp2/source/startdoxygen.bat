if not exist .\startdoxygen.bat goto ERROR1
if exist .\Doxydir rmdir /S /Q Doxydir
if exist .\doxystderr.txt del doxystderr.txt
if exist .\doxystdout.txt del doxystdout.txt
if "%1"=="clean" goto END
mkdir Doxydir
doxygen Doxyfile 2>doxystderr.txt 1>doxystdout.txt
start "Doxygen Output" .\Doxydir\html\index.html
start doxystderr.txt
goto END
:ERROR1
echo "Please start the batch file at the source directory of ricpp2"
goto END
:END
