Database latency and throughput tests
=====================================

The code in this repository has been used for two different purposes:

  1. Compare database access latency between MongoDB and ArangoDB
  2. Measure scalability of throughput for an ArangoDB cluster.

For the first, look at the file `INSTALL.md` for a description of the
building and running procedure.

For the ArangoDB cluster test, only the files

    arangoTestReadWriteCluster.cpp
    arangoTestPrepareCluster.cpp
    doit.js
    eval.js
    Makefile
    picojson.h
    run.sh
    utils.h
    waiter.cpp

are needed and the `Makefile` contains targets

    arangoTestPrepareCluster
    arangoTestReadWriteCluster
    waiter

to build the code. Furthermore, there is a subdirectory `Waiter-docker`
which contains the infrastructure to build the `neunhoef/waiter` Docker 
image. See instructions for building there.

If there are any questions, please ask

  "Max Neunhöffer" <max@arangodb.com>
