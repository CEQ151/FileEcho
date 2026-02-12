@echo off
setlocal EnableDelayedExpansion

REM ========================================================
REM  FileEcho One-Click Release Builder
REM ========================================================

set "VERSION=1.1.1"

echo.
echo  FileEcho v%VERSION% - Release Builder
echo ========================================================
echo.
echo [0/5] Initializing Environment...

REM Switch to script directory
cd /d "%~dp0"

REM 1. Get the 8.3 Short Path of the CURRENT DIRECTORY (.)
REM This ensures CMake receives "D:\FILEEC~1" instead of "D:\FileEcho - Copy"
REM preventing all space-related errors.
for %%I in (.) do set "SOURCE_DIR=%%~sfsI"

set "BUILD_DIR=build"
set "DIST_DIR=dist\FileEcho-v%VERSION%"

echo   - Source: %SOURCE_DIR%
echo   - Build : %BUILD_DIR%
echo   - Dist  : %DIST_DIR%



echo.
echo [1/5] Packing Assets...
python pack_assets.py
if %ERRORLEVEL% NEQ 0 (
    echo [Error] Failed to pack assets.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo [Cleaning] Removing old build directory...
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"

echo.
echo [2/5] Configuring CMake (Release)...
REM Explicitly point to the SOURCE DIRECTORY, not the file
cmake -S "%SOURCE_DIR%" -B "%BUILD_DIR%" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% NEQ 0 (
    echo [Error] CMake configuration failed.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo [3/5] Building Project...
cmake --build "%BUILD_DIR%" --config Release
if %ERRORLEVEL% NEQ 0 (
    echo [Error] Build failed.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo [4/5] Creating Distribution...
if exist "%DIST_DIR%" rmdir /s /q "%DIST_DIR%"
mkdir "%DIST_DIR%"

REM Copy icon for installer
if exist "resources\logo.ico" (
    copy /Y "resources\logo.ico" "%DIST_DIR%\" > nul
    echo   + Copied logo.ico
)

if exist "%BUILD_DIR%\FileEcho.exe" (
    copy /Y "%BUILD_DIR%\FileEcho.exe" "%DIST_DIR%\" > nul
    echo   + Copied FileEcho.exe
) else (
    echo [Error] FileEcho.exe not found.
    pause
    exit /b 1
)

echo.
echo   (Static linking enabled - no DLL dependencies)
echo.
echo ========================================================
echo  Build Complete!  FileEcho v%VERSION%
echo  Output: %DIST_DIR%\
echo ========================================================
echo.
echo  To create installer: makensis installer.nsi
echo.
pause