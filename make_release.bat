@echo off
setlocal

:: Check for version argument
if "%~1"=="" (
    echo [ERROR] Release version must be specified as an argument.
    goto :quit
)

set "ver=%~1"
set "zipper=C:\Program Files\7-Zip\7z.exe"

:: Check if 7-Zip exists
if not exist "%zipper%" (
    echo [ERROR] 7-Zip not found at "%zipper%". Please install 7-Zip or update the path.
    goto :quit
)

:: Create temp release folder
mkdir "%ver%" 2>nul

pushd "%ver%"
copy "..\build\Win32\Release\Xenos.exe" >nul
copy "..\build\x64\Release\Xenos64.exe" >nul
copy "..\Releases\Changelog.txt" >nul
copy "..\Releases\Readme.txt" >nul

:: Compress using 7-Zip
"%zipper%" a -t7z "Xenos_%ver%.7z" * -mx=9 -myx=9 >nul

:: Move archive to Releases
copy "Xenos_%ver%.7z" "..\Releases\Xenos_%ver%.7z" >nul
popd

:: Clean up temp folder
rmdir /S /Q "%ver%"

:: Copy PDB files
mkdir "Releases\Pdb\%ver%" 2>nul
copy "build\Win32\Release\Xenos.pdb" "Releases\Pdb\%ver%\Xenos.pdb" >nul
copy "build\x64\Release\Xenos64.pdb" "Releases\Pdb\%ver%\Xenos64.pdb" >nul

:quit
endlocal
