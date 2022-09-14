@echo off

set CC=gcc
set CFLAGS= -c -g -O3 -I../../include -std=c99 -pedantic -Wall -Wextra -Wno-deprecated-declarations
set LFLAGS=-subsystem:console
set APP=%1
set SRC=%2

if not defined SRC (
    set SRC=*
)

if not defined APP (
    call build.bat vm
    call build.bat asm
    exit /b 0
)

pushd build\%APP%

echo compiling %APP% with %CC%...
call %CC% ../../src/%APP%/%SRC%.c %CFLAGS%
echo done!
echo -----------------------------

if %errorlevel% == 0 (
    echo linking...
    call %CC% *.o -o ../../lb%APP%.exe
    echo done!
    echo -----------------------------
)

popd
