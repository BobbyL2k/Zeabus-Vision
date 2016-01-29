#!/bin/bash
clear

# FILES="../src/utilities.cpp test.cpp"
FILES="test.cpp"

# COLORS http://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux
Red="\033[0;31m"
Green='\033[0;32m'
Cyan='\033[0;36m'
NoColor='\033[0m'

CheckCode () {
    if [ $2 == 0 ]
    then
      printf "${Green}"
      echo $1 "with code" $2
      printf "${NoColor}"
    else
      printf "${Red}"
      echo $1 "with code" $2
      printf "${NoColor}"
    fi
}

printf "${Cyan}"
echo "=========   Compiling test   ========="
printf "${NoColor}"
echo "Compiling ${FILES}"
g++ ${FILES} `pkg-config --libs opencv` -I /usr/local/include/ -I include/ -o test -std=c++11
rc=$?
CheckCode "compiled ended" $rc
if [ $rc == 0 ]
then
  printf "${Green}"
  echo "=========    Running test    ========="
  printf "${NoColor}"
  ./test
  CheckCode "program exited" $?
fi
