#!/bin/bash

# The following must bind the HOSTS variable as an array and can set other
# environment variables
. coordinatorlist.sh

echo
echo VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
echo

echo Found ${#HOSTS[*]} host addresses for coordinators:
if [ ${#HOSTS[*]} -eq 0 ] ; then
  echo No coordinators found, nothing to do
  exit 0
fi
for i in ${HOSTS[*]} ; do
  echo "  $i"
done

if [ -z "$NRSHARDS" ] ; then
  export NRSHARDS=${#HOSTS[*]}
fi
echo Using $NRSHARDS shards "[set NRSHARDS to change]"

if [ -z "${PORTS[*]}" ] ; then
    if [ -z "$PORT" ] ; then
      export PORT=8529
    fi
    LASTHOST=`expr ${#HOSTS[*]} - 1`
    PORTS=()
    for i in `seq 0 $LASTHOST` ; do
        PORTS[$i]=$PORT
    done
fi
echo Using ports: ">${PORTS[*]}<" "[set PORTS or PORT to change]"

if [ -z "$TIME" ] ; then
  export TIME=60
fi
echo Running for $TIME seconds "[set TIME to change]"

if [ -z "$READPERCENTS" ] ; then
  export READPERCENTS=0
fi
echo Using "$READPERCENTS%" reads "[set READPERCENTS to change]"

if [ -z "$COMPLEXITY" ] ; then
  export COMPLEXITY=1
fi
echo Using complexity "$COMPLEXITY" for documents "[set COMPLEXITY to change]"

echo
echo ---------------------------------------------------------------------
echo

if [ -z "$1" ] ; then
  # Initialisation mode
  echo Initializing cluster...
  echo ./arangoTestPrepareCluster ${HOSTS[0]} ${PORTS[0]} $NRSHARDS
  ./arangoTestPrepareCluster ${HOSTS[0]} ${PORTS[0]} $NRSHARDS $COMPLEXITY
  echo done.
  exit 0
fi

export TESTNAME=$1
shift

while [ ! -z "$1" ] ; do
  export HOST=$1
  shift
  if [ $HOST -lt 0 -o $HOST -ge ${#HOSTS[*]} ] ; then
    echo Host number "($HOST)" out of bounds, skipping request.
  else
    echo ./arangoTestReadWriteCluster ${HOSTS[$HOST]} ${PORTS[$HOST]} $TIME $READPERCENTS $COMPLEXITY $RESULT_URL $TESTNAME
    ./arangoTestReadWriteCluster ${HOSTS[$HOST]} ${PORTS[$HOST]} $TIME $READPERCENTS $COMPLEXITY $RESULT_URL $TESTNAME >/dev/null &
  fi
done

wait

echo
echo All done.

echo
echo ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
echo

