@echo off

rem del /Q build\*.*
rem del /Q build\qol.exe

pushd .
cd tests

set INCLUDES=-I..\src
set WOPTIONS=-Wall -Wextra -pedantic -Wno-unused-variable -Wno-unused-parameter
set TARGET=-xc -std=c2y -ffast-math --target=x86_64-pc-windows-msvc -mtune=znver3

set OPTIONS=-g %INCLUDES% %WOPTIONS% %TARGET%
set FILES=main.c ..\src\qol.c

rem echo %OPTIONS%

:buildpie
clang %OPTIONS% %FILES% -o ../build/qol.exe
IF %ERRORLEVEL% NEQ 0 goto exit

:runpie
chcp 65001 > nul
cd ..\build

call qol.exe

:exit
popd
