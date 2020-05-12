# Copyright 2019, One-Way Automation.Inc. (www.onewayautomation.com)
# License: MIT

# Command line options
param (
	[string]$targetOsName="ubuntu1804", # Name of the OS. Possible values are: ubuntu1804, debian95, centos75 etc
	[string]$versionTag="2.12.1",		# Version tag
	[string]$arch="x64"
 )

 if ($Env:OS -eq "Windows_NT")  {
	$targetOsName="windows"
 }
$scriptRoot=$PSScriptRoot
if ($scriptRoot.Equals("")) { 
    $scriptRoot = "."
}

# Root folder of source code repositories:
$repoRoot=$scriptRoot+"/.."

# Name of the folder to get source files:
$projectFolder="botan_"+$versionTag

#Full path to installtion folder, where build output results will be copied to.
$installDir=$repoRoot+"/"+$projectFolder+"/"+$targetOsName

$vsArch="x86"
if ($arch.Equals("x64")) {
	$vsArch="amd64"
}

Push-Location $repoRoot

if ( -not(Test-Path $projectFolder) ) {
	git clone --recursive --branch $versionTag https://github.com/randombit/botan.git $projectFolder
}

Push-Location $projectFolder

# Create dir to install files:
if ( -not(Test-Path $installDir) ) {
	mkdir -p $installDir
}

if ($Env:OS -eq "Windows_NT")  {
	# Set environment variables for Visual Studio:
	#if ($env:APPVEYOR_BUILD_WORKER_IMAGE -eq "Visual Studio 2017") {
		cmd.exe /c "call ""C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"" $vsArch && set > ./vcvars.txt"
	#} 
	#else { # TODO: For now setting for VS 2015.Set for other version of Visual Studio too.
	#	cmd.exe /c "call `"C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd`" /x64 && call `"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat $vsArch`" x86_amd64 && set > %temp%\vcvars.txt"
	#}
	  
	Get-Content "./vcvars.txt" | Foreach-Object {
		if ($_ -match "^(.*?)=(.*)$") {
			Set-Content "env:\$($matches[1])" $matches[2]
		}
	}

	if ( -not(Test-Path $installDir/debug/botan.lib) ) {
		python configure.py --debug-mode --enable-static-library --prefix=$installDir/debug --cxxflags=/std:c++latest
		nmake.exe BUILD=debug
		nmake.exe install
	}
	if ( -not(Test-Path $installDir/release/botan.lib) ) {
		python configure.py --enable-static-library --prefix=$installDir/release --cxxflags=/std:c++latest --cxxflags=/std:c++latest
		nmake.exe BUILD=release
		nmake.exe install
	}
}
else {
	python ./configure.py --enable-static-library --prefix=$installDir --cxxflags=/std:c++latest
	make BUILD=debug
	make install

	make BUILD=release
	make install
}

Pop-Location
Pop-Location

