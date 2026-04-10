@echo off
SETLOCAL

:: Create the build directory name
set BUILD_DIR=build

:: Create the build directory if it doesn't exist
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
    echo [+] Created %BUILD_DIR% folder.
)

:: Navigate into the build directory
pushd %BUILD_DIR%

echo [*] Generating VS 2022 x64 Project Files...

:: Run CMake with explicit Generator and Architecture
cmake -G "Visual Studio 17 2022" -A x64 ..

:: if generation was successful
if %errorlevel% neq 0 (
    echo [!] Configuration failed. Check if VS 2022 is installed.
    pause
    exit /b %errorlevel%
)

echo [^] Project files generated successfully in /%BUILD_DIR%
popd

pause