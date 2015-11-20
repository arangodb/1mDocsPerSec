#!/bin/bash
N=0
for f in *.times ; do
    M=`grep "Number of reads" $f | sed -e 's/^[^:]*: *\([0-9]*\) *$/\1/'`
    N=`expr $N + $M`
    echo New number of reads: $M, new sum: $N
done

