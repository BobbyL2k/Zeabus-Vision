#!/bin/bash
clear

# FILES="../src/utilities.cpp test.cpp"
FILES="test.cpp"

Green='\033[0;32m'
Cyan='\033[0;36m'
NoColor='\033[0m'

printf "${Cyan}"
echo "=========   Compiling test   ========="
printf "${NoColor}"
g++ ${FILES} `pkg-config --libs opencv` -I /usr/local/include/ -I include/ -o test -std=c++11
rc=$?
printf "${Cyan}"
echo "compiled ended with code" $rc
printf "${NoColor}"
if [[ $rc = 0 ]]; then
  printf "${Green}"
  echo "=========    Running test    ========="
  printf "${NoColor}"
  ./test
  printf "${Green}"
  echo "program exited with code" $?
  printf "${NoColor}"
fi
