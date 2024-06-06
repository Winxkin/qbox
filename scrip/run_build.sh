#!/bin/bash

# # Configure the build with CMake
cd ..
cmake -B build -DCPM_SOURCE_CACHE=$(pwd)/Packages

# # Change directory to the build directory
cd build
make

