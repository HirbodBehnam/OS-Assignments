#!/bin/sh
gcc -O3 -o main main.c -lpthread
./main input.txt 8