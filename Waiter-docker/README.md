ArangoDB cluster throughput test
================================

First compile the waiter in `..` by doing

    make throughputTest
    
there.

Then build the docker image with:

    docker build -t neunhoef/waiter .
    docker push neunhoef/waiter

or simply

    make
