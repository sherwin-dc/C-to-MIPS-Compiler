#!/bin/bash
set +e



RED='\033[0;31m'
GREEN='\033[1;32m'
NC='\033[0m' # No Color
PURPLE="\e[0;35m"
ORANGE="\e[0;33m"
BLUE="\e[0;34m"
SWAP="\e[7m"

# Make compiler
make clean
make bin/c_compiler

TESTED=0
PASSED=0
while read -r line; do
echo $line
  # ignore comments and empty lines
  IFS=' ' read -ra OUT <<< "${line}"
  if [[ ${OUT[0]} == "" ]] || [[ ${OUT[0]} == "#" ]]; then
    continue
  fi
  ((TESTED++))

  # Run compiler
  bin/c_compiler -S ./compiler_tests/"${line}".c -o run/test_program.s

  if [ $? -ne 0 ]; then
    echo -e "$RED""Test ""${line}""failed to compile with compiler""$NC"
    continue
  fi

  # Use GCC to assemble
  mips-linux-gnu-gcc -march=mips1 -mfp32 -o run/test_program.o -c run/test_program.s
  if [ $? -ne 0 ]; then
    echo -e "$RED""Test ""${line}""failed to assemble with gcc""$NC"
    continue
  fi

  # Use GCC to create executible with driver
  mips-linux-gnu-gcc -march=mips1 -mfp32 -static -o run/test_program run/test_program.o compiler_tests/"${line}"_driver.c
  if [ $? -ne 0 ]; then
    echo -e "$RED""Test ""${line}""failed to link with driver using gcc""$NC"
    continue
  fi

  # Run in qemu
  timeout 10s qemu-mips run/test_program
  status=$?
  if [ $status -eq 124 ]; then
    echo -e "$RED""Test ""${line}"" did not complete within 10s""$NC"
    continue
  fi
  if [ $status -ne 0 ]; then
    echo -e "$RED""Test ""${line}""failed with qemu""$NC"
    continue
  fi

  ((PASSED++))


done < scripts/WORKING_TESTS

echo -e "$ORANGE""$SWAP""Result: ""$ORANGE"" Passed ""$PASSED"" out of ""$TESTED"" tests""$NC"

# clean
rm -f run/*

if [[ $PASSED -ne $TESTED ]]; then
  echo -e "$RED""Exiting with error code""$NC"
  exit 1
else
  echo -e "$GREEN""All listed working tests have passed""$NC"
fi