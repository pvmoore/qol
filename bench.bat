@echo off

rem del /Q build\*.*
rem del /Q build\bench.exe

pushd .
cd tests

set INCLUDES=-I..\src
set WOPTIONS=-Wall -Wextra -pedantic -Wno-unused-variable -Wno-unused-parameter -O3
set TARGET=-xc -std=c2y -ffast-math --target=x86_64-pc-windows-msvc -mtune=znver3

set OPTIONS=%INCLUDES% %WOPTIONS% %TARGET%
set FILES=bench.c ..\src\qol.c

rem echo %OPTIONS%

:buildbench
clang %OPTIONS% %FILES% -o ../build/bench.exe
IF %ERRORLEVEL% NEQ 0 goto exit

:runtest
chcp 65001 > nul
cd ..\build

call bench.exe

:exit
popd
