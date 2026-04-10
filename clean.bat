@echo off
setlocal

:: Define the build directory
set BUILD_DIR=build

echo [*] Cleaning project...

:: Check if the build directory exists
if exist %BUILD_DIR% (
    echo [+] Removing %BUILD_DIR% folder...
    rmdir /s /q %BUILD_DIR%
    if exist %BUILD_DIR% (
        echo [!] Failed to delete %BUILD_DIR%. Is it open in another program?
    ) else (
        echo [^] Successfully cleaned.
    )
) else (
    echo [-] No %BUILD_DIR% folder found. Nothing to clean.
)

pause