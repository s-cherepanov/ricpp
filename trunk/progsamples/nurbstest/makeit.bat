@echo off

del nurbstest.exe

echo g++ -c src\*.cpp

g++ -Wall -fexceptions -g2 -c src\*.cpp

echo g++ -o nurbstest *.o

g++ -o nurbstest.exe *.o

del *.o

