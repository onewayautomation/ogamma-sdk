@echo off
SET SCR_PATH=%~dp0
for %%i in ("%~dp0.") do SET "SCR_PATH=%%~fi"

SET SDK_FOLDER=%SCR_PATH%\..\1WaOpcUaSdk

IF EXIST %SDK_FOLDER% GOTO END_SDK_CLONE
git clone --recursive --depth 3 https://github.com/onewayautomation/1WaOpcUaSdk.git %SDK_FOLDER%
:END_SDK_CLONE

call %SCR_PATH%\setup-environment.cmd

REM =============== Installing boost ... ======================
call %SCR_PATH%\build-boost-full.cmd
REM =============== boost has been installed! =================

REM =============== Installing botan ... ======================
call %SCR_PATH%\build-botan.cmd
REM =============== botan has been installed! =================
