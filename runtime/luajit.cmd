@echo off
rem luajit wrapper that runs the 32bit luajit on 32bit windows
rem and the 64bit luajit on 64bit windows.
if [%PROCESSOR_ARCHITECTURE%] == [AMD64] goto win64
if [%PROCESSOR_ARCHITEW6432%] == [AMD64] goto win64
call "%~dp0bin_Windows_x86\luajit.exe" -e "dofile [[%~dp0_setup_package_paths.lua]]" %*
goto end
:win64
call "%~dp0bin_Windows_x64\luajit.exe" -e "dofile [[%~dp0_setup_package_paths.lua]]" %*
:end
