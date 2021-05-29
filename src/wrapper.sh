#!/bin/bash
echo "Compiling to MIPS..."
# cat $2 | ./bin/compiler 2> /dev/null 1> $4

# Get source file name
sourcefile=$2
destfile=$4
# IFS='.' read -ra OUT <<< "$sourcefile"
# IFS='/' read -ra OUT2 <<< "${OUT[0]}"
barename=$(basename $sourcefile)
file3AC="run/""${barename}"".3ac"
mkdir -p "$(dirname ${destfile})"
$(>$destfile)
if [ $? -ne 0 ]; then
  echo -e "$RED""Cannot create output file""$NC"
  exit 1
fi
# remove file if it exists and recreate it
rm -f $file3AC
mkdir -p run/
touch $file3AC
./bin/mainparser $sourcefile $destfile $file3AC
