#!/bin/bash
export t=`grep "^Time needed per operation:" datei | sed -e "s/^Time needed per operation: //"`
echo RESULT "$1: $t"
