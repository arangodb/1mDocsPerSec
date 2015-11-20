ArangoDB cluster throughput test
================================

First compile the waiter in `..` by doing

    make throughputTest
    
there and copy the executables to the `waiter` dir:

    cp ../waiter ../arangoTestPrepareCluster ../arangoTestReadWriteCluster ../run.sh waiter

Then build the docker image with:

    docker build -t neunhoef/waiter .
    docker push neunhoef/waiter

or simply type

    make
