@echo off

set CC=gcc
set LINKER=lld-link
set CFLAGS=-I../include -O3 -std=c99 -pedantic -Wall -Wextra -Wno-deprecated-declarations
set LFLAGS=-subsystem:console
set SRC=%1

if not defined SRC (
   set SRC=*
)

pushd build

call %CC% ../src/%SRC%.c -c %CFLAGS%
call %CC% *.o -o ../lb.exe

popd
