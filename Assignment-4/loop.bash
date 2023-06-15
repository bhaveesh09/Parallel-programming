#!/bin/bash
for n in 1 10 100 1000 10000 100000 500000 1000000
do
    g++ all04.cpp -DARRAYSIZE=$n -o all04 -lm -fopenmp
    ./all04
done
