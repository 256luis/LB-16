@echo off

set CC=clang
set CFLAGS= -c -g -O3 -I../include -std=c99 -pedantic -Wall -Wextra -Wno-deprecated-declarations
set LFLAGS=-subsystem:console
set APP=%1
set SRC=%2

if not defined SRC (
    set SRC=*
)

pushd build

echo compiling with %CC%...
call %CC% ../src/%SRC%.c %CFLAGS%
echo done!
echo -----------------------------

if %errorlevel% == 0 (
    echo linking...
    call %CC% *.o -o ../lbvm.exe
    echo done!
    echo -----------------------------
)

popd
