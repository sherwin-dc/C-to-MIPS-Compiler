#!/bin/bash
set +e



RED='\033[0;31m'
GREEN='\033[1;32m'
NC='\033[0m' # No Color
PURPLE="\e[0;35m"
ORANGE="\e[0;33m"
BLUE="\e[0;34m"
SWAP="\e[7m"


TESTED=0
PASSED=0
line=$1


  # Use GCC to assemble
  mips-linux-gnu-gcc -march=mips1 -mfp32 -std=c90 -S compiler_tests/"${line}".c -o test_program.s

  if [ $? -ne 0 ]; then
    echo -e "$RED""Test ""${line}""failed to compile with compiler""$NC"
  fi

  # Use GCC to assemble
  mips-linux-gnu-gcc -march=mips1 -std=c90 -mfp32 -o test_program.o -c test_program.s
  if [ $? -ne 0 ]; then
    echo -e "$RED""Test ""${line}""failed to assemble with gcc""$NC"
  fi

  # Use GCC to create executible with driver
  mips-linux-gnu-gcc -march=mips1 -std=c90 -mfp32 -static -o test_program test_program.o compiler_tests/"${line}"_driver.c
  if [ $? -ne 0 ]; then
    echo -e "$RED""Test ""${line}""failed to link with driver using gcc""$NC"
  fi

  # Run in qemu
  timeout 10s qemu-mips test_program
  status=$?
  if [ $status -eq 124 ]; then
    echo -e "$RED""Test ""${line}"" did not complete within 10s""$NC"
  fi
  if [ $status -ne 0 ]; then
    echo -e "$RED""Test ""${line}""failed with qemu""$NC"
  else
    echo -e "$GREEN""Test ok""$NC"
  fi




# clean
rm *.s
rm *.o
