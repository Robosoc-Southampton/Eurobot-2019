
@echo off

set @usage=Usage: "bin/verify" PORT MODE;  where MODE = primary or secondary;  note: find PORT using "bin/ports"

if "%1" == "" (
	echo %@usage:;=&echo.%
	exit /b
)

if "%2" == "" (
	echo %@usage:;=&echo.%
	exit /b
)

call "%~dp0prep" %2
call "%~dp0arduino-cmd" --upload --port %1
