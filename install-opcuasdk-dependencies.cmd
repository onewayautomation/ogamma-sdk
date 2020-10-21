@echo off
SET SCR_PATH=%~dp0
for %%i in ("%~dp0.") do SET "SCR_PATH=%%~fi"

IF NOT DEFINED VS_VERSION ( SET VS_VERSION=VS2019)

SET SDK_FOLDER=%SCR_PATH%\..\ogamma-sdk
SET REPO_BASE_FOLDER=%SCR_PATH%\..

IF EXIST %SDK_FOLDER% GOTO END_SDK_CLONE
git clone --recursive --depth 3 https://github.com/onewayautomation/ogamma-sdk.git %SDK_FOLDER%
:END_SDK_CLONE

call %SCR_PATH%\setup-environment.cmd

REM =============== Installing boost ... ======================
call %SCR_PATH%\build-boost-full.cmd boost-1.70.0
REM =============== boost has been installed! =================

REM =============== Installing botan ... ======================
pwsh %SCR_PATH%\build-botan.ps1
REM =============== botan has been installed! =================

PUSHD %REPO_BASE_FOLDER%
IF EXIST spdlog GOTO END_SPDLOG
git clone  --branch v1.4.2 https://github.com/gabime/spdlog.git
:END_SPDLOG

PUSHD %REPO_BASE_FOLDER%
IF EXIST Catch2 GOTO END_CATCH2
git clone  --branch v2.11.0 https://github.com/catchorg/Catch2.git
:END_CATCH2

POPD
POPD