#!/bin/bash
echo "Cleaning..."
rm -rf ./build/
echo "Building..."
mkdir "build"
gcc -o ./build/Practical3 -O3 main.c Practical_Q_1f.o -lm
echo "Running..."
./build/Practical3
