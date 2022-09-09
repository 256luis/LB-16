@echo off

echo cleaning...
if exist *.exe del /q *.exe
if exist build\*.o* del /q build\*.o*
echo done!
