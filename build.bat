@echo off
REM Build script for Kaleidoscope on Windows
REM Requires: CMake, LLVM, and a C++ compiler (MSVC/Clang)

echo ========================================
echo Kaleidoscope LLVM Tutorial - Build Script
echo ========================================
echo.

REM Check for CMake
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: CMake not found in PATH
    echo Please install CMake from https://cmake.org/download/
    exit /b 1
)

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo CMake configuration failed.
    echo.
    echo If LLVM was not found, try:
    echo   cmake .. -DLLVM_DIR=C:\path\to\llvm\lib\cmake\llvm
    echo.
    cd ..
    exit /b 1
)

REM Build
echo.
echo Building...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo Build failed.
    cd ..
    exit /b 1
)

echo.
echo ========================================
echo Build successful!
echo Executable: build\bin\Release\kaleidoscope.exe
echo ========================================

cd ..

