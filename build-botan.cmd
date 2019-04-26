call setup-environment.cmd

SET TAG_BOTAN=2.9.0
set PATH_BOTAN=botan

PUSHD %REPO_BASE_FOLDER%

IF EXIST %PATH_BOTAN% GOTO BUILD_BOTAN
echo Cloning Botan ...
git clone --recursive --branch %TAG_BOTAN% --depth 1 https://github.com/randombit/botan.git %PATH_BOTAN%

:BUILD_BOTAN
PUSHD %PATH_BOTAN%
IF EXIST %REPO_BASE_FOLDER%\%PATH_BOTAN%\install\debug\botan.lib GOTO BUILD_BOTAN_RELEASE
python configure.py --debug-mode --enable-static-library --prefix=%REPO_BASE_FOLDER%\%PATH_BOTAN%\install\debug
nmake BUILD=debug
nmake install
:BUILD_BOTAN_RELEASE
IF EXIST %REPO_BASE_FOLDER%\%PATH_BOTAN%\install\release\botan.lib GOTO END_BUILD_BOTAN
python configure.py --enable-static-library --prefix=%REPO_BASE_FOLDER%\%PATH_BOTAN%\install\release
nmake BUILD=release
nmake install

:END_BUILD_BOTAN
POPD

POPD