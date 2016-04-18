#!/usr/bin/bash

prog_name=$1
./DL $prog_name
./Assembler program.asm
echo
echo "####PROGRAMME####"
echo
./StackProcessor program.bin

exit 0
