#!/bin/bash
export CLIENTS=4
export LASTCLIENT=`expr $CLIENTS - 1`
function waitvoll() {
  while [ `date +%S` != "00" ] ; do
    echo Waiting for full minute...
    sleep 0.5
  done
}
export ACTION="WARMUP"
export READPERCENTS=100
waitvoll
./run.sh ${ACTION} `seq 0 $LASTCLIENT`
export ACTION="READ"
export READPERCENTS=100
waitvoll
./run.sh ${ACTION}_${CLIENTS}_1 `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_2 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_3 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_4 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_5 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_6 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_7 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_8 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_9 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_10 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_11 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_12 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
export ACTION="WRITE"
export READPERCENTS=0
waitvoll
./run.sh ${ACTION}_${CLIENTS}_1 `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_2 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_3 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_4 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_5 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_6 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_7 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_8 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_9 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_10 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_11 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_12 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
export ACTION="RW"
export READPERCENTS=50
waitvoll
./run.sh ${ACTION}_${CLIENTS}_1 `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_2 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_3 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_4 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_5 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_6 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_7 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_8 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_9 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_10 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_11 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`
waitvoll
./run.sh ${ACTION}_${CLIENTS}_12 `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT` `seq 0 $LASTCLIENT`

