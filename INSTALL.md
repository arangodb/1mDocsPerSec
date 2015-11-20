Installation of this DB latency test
====================================

 0. Make sure git, make, g++, scons, libboost, libcurl are all installed.

 1. Install ArangoDB on machine A and run it on port 8529
    Make sure that arangob is somewhere in PATH

 2. Install MongoDB on machine A and run it on port 27017

 3. Install MongoDB CXX-driver locally using 

    make installMongoDBCXXdriver

 4. Compile MongoDB tests

    make mongoTests

 5. Compile ArangoDB tests

    make arangoTests

 6. Run tests on same machine A:

    make tests

 7. Run tests on another machine with DBs on A:

    edit ARANGODB_HOST, ARANGODB_PORT, MONGODB_HOST, MONGODB_PORT in Makefile
    make tests
