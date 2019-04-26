@echo off
SET vc_bat_name1="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
@echo on

rem SET ARCHITECTURE=amd64
SET ARCHITECTURE=x64

SET VS_ARCH=x86
IF "%ARCHITECTURE%"=="x64" SET VS_ARCH=amd64

IF "%1"=="NoVc" GOTO continue
IF EXIST %vc_bat_name1% GOTO VC1
echo Failed to find Visual Studio 2017 batch file to setup environment.
echo Please check in the Visual Studio Installer that option "Desktop development with C++" is selected for Visual Studio 2017 Community Edition.
echo If you have other edition of Visual Studio 2017, please modify this file accordingly.
exit
:VC1
call %vc_bat_name1% %VS_ARCH%
GOTO continue

:continue

%~d0

SET SCRIPTS_FOLDER=%~dp0
for %%i in ("%~dp0.") do SET "SCRIPTS_FOLDER=%%~fi"
SET REPO_BASE_FOLDER=%SCRIPTS_FOLDER%\..