REM This script clones full version of the boost library under the the parent folder of this script, and builds it, for Visual Studio 2017, 64 bit release and debug versions.
REM In case of success, output result will be located at ../boost/stage/x64
SET SCRIPT_PATH=%~dp0
for %%i in ("%~dp0.") do SET "SCRIPT_PATH=%%~fi"

SET REPO_BASE_FOLDER=%SCRIPT_PATH%\..

IF "%VS_VERSION%"=="VS2017" (
	echo building for Visual Studio 2017
	SET MSVC_VER=msvc-14.1
	GOTO M1
	) 
SET MSVC_VER=msvc-14.2

:M1

REM need specific version of boost, which was used to build the OPC UA SDK binaries.
IF "%1"=="" (
	SET TAG_BOOST=boost-1.72.0
	GOTO M2
	)
SET TAG_BOOST=%1

:M2

IF EXIST %REPO_BASE_FOLDER%\boost GOTO M_FOLDER_EXISTS

PUSHD %REPO_BASE_FOLDER%
git clone --recursive --branch %TAG_BOOST% https://github.com/boostorg/boost.git boost
POPD

:M_FOLDER_EXISTS
PUSHD %REPO_BASE_FOLDER%\boost
call bootstrap
b2 -j4 toolset=%MSVC_VER% variant=release,debug address-model=64 architecture=x86 link=static threading=multi runtime-link=shared --build-type=minimal stage --stagedir=stage/x64
POPD
GOTO M_END

echo Folder with boost library [%REPO_BASE_FOLDER%\boost] already exists, exiting.

:M_END
