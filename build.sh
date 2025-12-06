#!/bin/bash
# Build script for Kaleidoscope on Linux/macOS
# Requires: CMake, LLVM, and a C++ compiler (GCC/Clang)

echo "========================================"
echo "Kaleidoscope LLVM Tutorial - Build Script"
echo "========================================"
echo

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake not found"
    echo "Please install CMake:"
    echo "  Ubuntu/Debian: sudo apt-get install cmake"
    echo "  macOS: brew install cmake"
    exit 1
fi

# Check for LLVM
if ! command -v llvm-config &> /dev/null; then
    echo "WARNING: llvm-config not found in PATH"
    echo "You may need to specify LLVM_DIR manually"
fi

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake ..
if [ $? -ne 0 ]; then
    echo
    echo "CMake configuration failed."
    echo
    echo "If LLVM was not found, try:"
    echo "  cmake .. -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm"
    echo
    cd ..
    exit 1
fi

# Build
echo
echo "Building..."
cmake --build .
if [ $? -ne 0 ]; then
    echo "Build failed."
    cd ..
    exit 1
fi

echo
echo "========================================"
echo "Build successful!"
echo "Executable: build/bin/kaleidoscope"
echo "========================================"
echo
echo "Run with: ./build/bin/kaleidoscope"

cd ..

