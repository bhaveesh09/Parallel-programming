#!/bin/bash

#number of threads:
for t in 1 4
do
    echo ""
    echo NUMT = $t
    g++ -DNUMT=$t project0.cpp -o project0 -lm -fopenmp
    ./project0
done