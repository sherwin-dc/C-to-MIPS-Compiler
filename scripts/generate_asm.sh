#!/bin/bash

sourcefile=$1
IFS='.' read -ra OUT <<< "$sourcefile"
IFS='/' read -ra OUT2 <<< "${OUT[0]}"
fileasm=run/${OUT2[${#OUT2[@]}-1]}.asm

mipsel-linux-gnu-g++ -S -mips1 -mfp32 -fpic -Wall -fno-exceptions -O3 -std=c++11 -static $sourcefile -o $fileasm