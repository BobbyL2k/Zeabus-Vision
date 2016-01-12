#!/bin/bash
clear

# FILES="../src/utilities.cpp local-test.cpp"
FILES="local-test.cpp"

Green='\033[0;32m'
Cyan='\033[0;36m'
NoColor='\033[0m'

printf "${Cyan}"
echo "=========   Compiling local-test   ========="
printf "${NoColor}"
g++ ${FILES} `pkg-config --libs opencv` -I /usr/local/include/ -I ../include/ -o localtest
rc=$?
printf "${Cyan}"
echo "compiled ended with code" $rc
printf "${NoColor}"
if [[ $rc = 0 ]]; then
  printf "${Green}"
  echo "=========    Running local-test    ========="
  printf "${NoColor}"
  ./localtest
  printf "${Green}"
  echo "program exited with code" $?
  printf "${NoColor}"
fi
