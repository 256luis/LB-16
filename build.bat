@echo off

set CC=clang
set LINKER=lld-link
set CFLAGS=-I../../include -O3 -std=c99 -pedantic -Wall -Wextra -Wno-deprecated-declarations
set LFLAGS=-subsystem:console
set APP=%1
set SRC=%2

if not defined SRC (
   set SRC=*
)

pushd build\%APP%

echo compiling with %CC%...
call %CC% ../../src/%APP%/%SRC%.c -c -g %CFLAGS%
echo done!
echo -----------------------------

if %errorlevel% == 0 (
    echo linking...
    call %CC% *.o -o ../../lb-%APP%.exe
    echo done!
    echo -----------------------------
)

popd
