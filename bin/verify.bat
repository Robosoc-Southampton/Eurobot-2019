
@echo off

set @usage=Usage: "bin/verify" MODE;  where MODE = primary or secondary

if "%1" == "" (
	echo %@usage:;=&echo.%
	exit /b
)

call "%~dp0prep" %1
call "%~dp0arduino-cmd" --verify --preserve-temp-files
