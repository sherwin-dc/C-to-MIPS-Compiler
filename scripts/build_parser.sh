#!/bin/bash
export CLASSPATH="./java/src:/usr/local/lib/antlr-4.9.1-complete.jar:$CLASSPATH"
# In c++
# java -jar env/antlr-4.9.1-complete.jar -Dlanguage=Cpp -no-listener -visitor -package cparser src/parser/cparser.g4 
# In java

if [[ "$1" == "" ]]; then
echo "Error. No input file"
exit 1
fi

java -jar /usr/local/lib/antlr-4.9.1-complete.jar -no-listener -no-visitor src/cparser.g4 -o java/
javac java/src/cparser*.java
cat $1 | java org.antlr.v4.gui.TestRig cparser prog -ps parse_tree.ps
epstopdf parse_tree.ps
# antlr4 -Dlanguage=Cpp -listener -visitor -package cparser $1 
