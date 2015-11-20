#!/bin/bash
function waitvoll() {
  while [ `date +%S` != "00" ] ; do
    echo Waiting for full minute...
    sleep 0.5
  done
}
export ACTION="WARMUP"
export READPERCENTS=100
waitvoll
./run.sh ${ACTION} 0
export ACTION="READ"
export READPERCENTS=100
waitvoll
./run.sh ${ACTION}1x20 0
waitvoll
./run.sh ${ACTION}1x40 0 0
waitvoll
./run.sh ${ACTION}1x60 0 0 0
waitvoll
./run.sh ${ACTION}1x80 0 0 0 0
waitvoll
./run.sh ${ACTION}1x100 0 0 0 0 0
export ACTION="WRITE"
export READPERCENTS=0
waitvoll
./run.sh ${ACTION}1x20 0
waitvoll
./run.sh ${ACTION}1x40 0 0
waitvoll
./run.sh ${ACTION}1x60 0 0 0
waitvoll
./run.sh ${ACTION}1x80 0 0 0 0
waitvoll
./run.sh ${ACTION}1x100 0 0 0 0 0
export ACTION="RW"
export READPERCENTS=50
waitvoll
./run.sh ${ACTION}1x20 0
waitvoll
./run.sh ${ACTION}1x40 0 0
waitvoll
./run.sh ${ACTION}1x60 0 0 0
waitvoll
./run.sh ${ACTION}1x80 0 0 0 0 
waitvoll
./run.sh ${ACTION}1x100 0 0 0 0 0


