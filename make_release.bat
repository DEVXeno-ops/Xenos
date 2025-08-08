@echo off
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

:: =========================
::   Release Packaging Script
:: =========================

:: Root path = folder ที่ไฟล์ .bat อยู่
set "ROOT=%~dp0"

:: ===== Check version argument =====
if "%~1"=="" (
    echo [ERROR] Release version must be specified as an argument.
    echo Usage: %~nx0 ^<version^>
    goto :quit
)

set "ver=%~1"
set "zipper=C:\Program Files\7-Zip\7z.exe"

:: ===== Check if 7-Zip exists =====
if not exist "%zipper%" (
    echo [ERROR] 7-Zip not found at "%zipper%".
    echo Please install 7-Zip or update the path in the script.
    goto :quit
)

:: ===== Create temp release folder =====
set "TMPDIR=%ROOT%%ver%"
if exist "%TMPDIR%" rmdir /S /Q "%TMPDIR%"
mkdir "%TMPDIR%" 2>nul

:: ===== Copy release files =====
echo [INFO] Copying release files...
if exist "%ROOT%build\Win32\Release\Xenos.exe" (
    robocopy "%ROOT%build\Win32\Release" "%TMPDIR%" "Xenos.exe" /NFL /NDL /NJH /NJS /NC /NS >nul
) else (
    echo [WARN] Win32 Release build not found.
)

if exist "%ROOT%build\x64\Release\Xenos64.exe" (
    robocopy "%ROOT%build\x64\Release" "%TMPDIR%" "Xenos64.exe" /NFL /NDL /NJH /NJS /NC /NS >nul
) else (
    echo [WARN] x64 Release build not found.
)

for %%F in (Changelog.txt Readme.txt) do (
    if exist "%ROOT%Releases\%%F" (
        robocopy "%ROOT%Releases" "%TMPDIR%" "%%F" /NFL /NDL /NJH /NJS /NC /NS >nul
    ) else (
        echo [WARN] %%F not found in Releases folder.
    )
)

:: ===== Compress using 7-Zip =====
pushd "%TMPDIR%"
echo [INFO] Creating archive Xenos_%ver%.7z ...
"%zipper%" a -t7z "Xenos_%ver%.7z" * -mx=9 -myx=9 >nul
popd

:: ===== Move archive to Releases =====
if not exist "%ROOT%Releases" mkdir "%ROOT%Releases"
move "%TMPDIR%\Xenos_%ver%.7z" "%ROOT%Releases\Xenos_%ver%.7z" >nul

:: ===== Clean up temp folder =====
rmdir /S /Q "%TMPDIR%"

:: ===== Copy PDB files =====
echo [INFO] Copying PDB files...
set "PDBDIR=%ROOT%Releases\Pdb\%ver%"
if not exist "%PDBDIR%" mkdir "%PDBDIR%"

if exist "%ROOT%build\Win32\Release\Xenos.pdb" (
    copy "%ROOT%build\Win32\Release\Xenos.pdb" "%PDBDIR%\Xenos.pdb" >nul
) else (
    echo [WARN] Win32 PDB not found.
)

if exist "%ROOT%build\x64\Release\Xenos64.pdb" (
    copy "%ROOT%build\x64\Release\Xenos64.pdb" "%PDBDIR%\Xenos64.pdb" >nul
) else (
    echo [WARN] x64 PDB not found.
)

echo [INFO] Packaging complete: %ROOT%Releases\Xenos_%ver%.7z
goto :eof

:quit
endlocal
