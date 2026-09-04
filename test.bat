@echo off

rem del /Q build\*.*
rem del /Q build\test.exe

pushd .
cd tests

set INCLUDES=-I..\src

set WOPTIONS=-Wall -Wextra -pedantic
set WOPTIONS=%WOPTIONS% -Wno-unused-variable -Wno-unused-parameter -Wno-gnu-empty-struct

set TARGET=-xc -std=c2y -ffast-math --target=x86_64-pc-windows-msvc -mtune=znver3 -mavx2

set OPTIONS=-g %INCLUDES% %WOPTIONS% %TARGET%
set FILES=test.c ..\src\qol.c

rem echo %OPTIONS%

:buildtest
clang %OPTIONS% %FILES% -o ../build/test.exe
IF %ERRORLEVEL% NEQ 0 goto compile_error

:runtest
chcp 65001 > nul
cd ..\build

call test.exe
IF %ERRORLEVEL% NEQ 0 goto run_error
goto exit

:compile_error
echo Compilation failed
goto exit

:run_error
echo test.exe returned %ERRORLEVEL%

:exit
popd
