#!/bin/bash

g++ main.cpp nbgc.cpp menu_files.cpp -o main.out

if [[ $* == *-run* ]]; then
    if [ $? -ne 0 ]; then
        exit
    fi
    ./main.out
fi