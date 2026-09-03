@echo off

rem del /Q build\*.*
del /Q build\pie.exe

pushd .
cd src

set WOPTIONS=-Wall -Wextra -pedantic -Wno-unused-variable -Wno-unused-parameter -O3
set TARGET=-xc -std=c2y -ffast-math --target=x86_64-pc-windows-msvc -mtune=znver3

set OPTIONS=%WOPTIONS% %TARGET%
set FILES=main.c

rem echo %OPTIONS%

:buildpie
clang %OPTIONS% %FILES% -o ../build/pie.exe
IF %ERRORLEVEL% NEQ 0 goto exit

:runpie
chcp 65001 > nul
cd ..\build

call pie.exe 

:exit
popd
