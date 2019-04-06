
@echo off

set @usage=Usage: "bin/upload" MAIN PORT;  where MAIN = primary, secondary or <test>;  note: find PORT using "bin/ports"

if "%1" == "" (
	echo %@usage:;=&echo.%
	exit /b
)

if "%2" == "" (
	echo %@usage:;=&echo.%
	exit /b
)

call "%~dp0prep" %1

if "%3" == "" (
	call "%~dp0arduino-cmd" --upload --port %2 --preserve-temp-files
) else (
	call "%~dp0arduino-cmd" --upload --port %2 --preserve-temp-files --board "arduino:avr:mega:cpu=atmega2560"
)
