REM This script clones full version of the boost library under the the parent folder of this script, and builds it, for Visual Studio 2017, 64 bit release and debug versions.
REM In case of success, output result will be located at ../boost/stage/x64
SET SCRIPT_PATH=%~dp0
for %%i in ("%~dp0.") do SET "SCRIPT_PATH=%%~fi"

REM need specific version of boost, which was used to build the OPC UA SDK binaries.
SET TAG_BOOST=boost-1.70.0

SET REPO_BASE_FOLDER=%SCRIPT_PATH%\..
IF EXIST %REPO_BASE_FOLDER%\boost GOTO M_FOLDER_EXITS

PUSHD %REPO_BASE_FOLDER%
git clone --recursive --branch %TAG_BOOST% https://github.com/boostorg/boost.git boost
PUSHD boost
call bootstrap
b2 -j4 toolset=msvc-14.1 variant=release,debug address-model=64 architecture=x86 link=static threading=multi runtime-link=shared --build-type=minimal stage --stagedir=stage/x64
POPD
POPD

GOTO M_END

:M_FOLDER_EXITS
echo Folder with boost library [%REPO_BASE_FOLDER%\boost] already exists, exiting.

:M_END
