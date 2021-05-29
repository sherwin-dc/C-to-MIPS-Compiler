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

# Get all test files
sourcefiles=$(find "compiler_tests/" -type f ! -name "*_driver.c")


for i in $sourcefiles; do
  line=${i%.c}

  ((TESTED++))

  # Run compiler
  bin/c_compiler -S "${line}".c -o test_program.s

  if [ $? -ne 0 ]; then
    echo -e "$RED""Test ""${line}"" failed to compile with compiler""$NC"
    continue
  fi

  # Use GCC to assemble
  mips-linux-gnu-gcc -march=mips1 -mfp32 -o test_program.o -c test_program.s
  if [ $? -ne 0 ]; then
    echo -e "$RED""Test ""${line}"" failed to assemble with gcc""$NC"
    continue
  fi

  # Use GCC to create executible with driver
  mips-linux-gnu-gcc -march=mips1 -mfp32 -static -o test_program test_program.o "${line}"_driver.c
  if [ $? -ne 0 ]; then
    echo -e "$RED""Test ""${line}"" failed to link with driver using gcc""$NC"
    continue
  fi

  # Run in qemu
  timeout 10s qemu-mips test_program
  status=$?
  if [ $status -eq 124 ]; then
    echo -e "$RED""Test ""${line}"" did not complete within 10s""$NC"
    continue
  fi
  if [ $status -ne 0 ]; then
    echo -e "$RED""Test ""${line}"" failed with qemu""$NC"
    continue
  fi
  echo -e "$GREEN""Test ""${line}"" passed""$NC"
  ((PASSED++))


done

echo -e "$ORANGE""$SWAP""Result: ""$ORANGE"" Passed ""$PASSED"" out of ""$TESTED"" tests""$NC"

# clean
rm *.s
rm *.o
rm passed.txt
rm tested.txt

echo $PASSED > passed.txt
echo $TESTED > tested.txt

exit 0