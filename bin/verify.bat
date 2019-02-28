
@echo off

set @usage=Usage: "bin/verify" MAIN;  where MAIN = primary, secondary or <test>

if "%1" == "" (
	echo %@usage:;=&echo.%
	exit /b
)

call "%~dp0prep" %1
call "%~dp0arduino-cmd" --verify --preserve-temp-files
