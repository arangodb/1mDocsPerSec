ARANGODB_HOST=127.0.0.1
ARANGODB_PORT=8529
MONGODB_HOST=127.0.0.1
MONGODB_PORT=27017
REPETITIONS=100001
RESULT_URL=
#example: RESULT_URL="http://104.155.47.169:8529/_api/document?collection=results"

all:	installMongoDBCXXdriver mongoTests arangoTests waiter

mongoTests:	mongoTestCount mongoTestFindOne mongoTestWriteOne

arangoTests:	arangoTestVersion arangoTestDocument arangoTestWriteOne \
    		arangoTestPrepareCluster arangoTestReadWriteCluster

installMongoDBCXXdriver:
	./installMongoDBCXXdriver.sh

mongoTestCount:	mongoTestCount.cpp Makefile utils.h
	g++ -std=c++11 -I./include -L./lib mongoTestCount.cpp -pthread -lmongoclient -lboost_thread -lboost_system -lboost_regex -O3 -Wall -o mongoTestCount `curl-config --libs`

mongoTestFindOne:	mongoTestFindOne.cpp Makefile utils.h
	g++ -std=c++11 -I./include -L./lib mongoTestFindOne.cpp -pthread -lmongoclient -lboost_thread -lboost_system -lboost_regex -O3 -Wall -o mongoTestFindOne `curl-config --libs`

mongoTestWriteOne:	mongoTestWriteOne.cpp Makefile utils.h
	g++ -std=c++11 -I./include -L./lib mongoTestWriteOne.cpp -pthread -lmongoclient -lboost_thread -lboost_system -lboost_regex -O3 -Wall -o mongoTestWriteOne `curl-config --libs`

arangoTestVersion:	arangoTestVersion.cpp Makefile utils.h
	g++ arangoTestVersion.cpp -o arangoTestVersion -std=c++11 -Wextra `curl-config --libs` -O3

arangoTestDocument:	arangoTestDocument.cpp Makefile utils.h
	g++ arangoTestDocument.cpp -o arangoTestDocument -std=c++11 -Wextra `curl-config --libs` -O3

arangoTestWriteOne:	arangoTestWriteOne.cpp Makefile utils.h
	g++ arangoTestWriteOne.cpp -o arangoTestWriteOne -std=c++11 -Wextra `curl-config --libs` -O3

arangoTestPrepareCluster:	arangoTestPrepareCluster.cpp Makefile utils.h
	g++ arangoTestPrepareCluster.cpp -o arangoTestPrepareCluster -std=c++11 -Wextra `curl-config --libs` -O3

arangoTestReadWriteCluster:	arangoTestReadWriteCluster.cpp Makefile utils.h
	g++ arangoTestReadWriteCluster.cpp -o arangoTestReadWriteCluster -std=c++11 -Wextra `curl-config --libs` -O3

waiter:	waiter.cpp Makefile utils.h
	g++ waiter.cpp -o waiter -std=c++11 -Wextra `curl-config --libs` -O0 -g

runAllTests: runMongoTests runArangoBTests runArangoTests

runMongoTests: mongoTestCount mongoTestFindOne mongoTestWriteOne 
	@mongo --host ${MONGODB_HOST} --port ${MONGODB_PORT} < clearmongo.js
	@echo
	@echo
	@echo
	@./mongoTestCount ${MONGODB_HOST} ${MONGODB_PORT} ${REPETITIONS} ${RESULT_URL}
	sleep 1
	@echo
	@echo
	@echo
	@./mongoTestFindOne ${MONGODB_HOST} ${MONGODB_PORT} ${REPETITIONS} ${RESULT_URL}
	sleep 1
	@echo
	@echo
	@echo
	@./mongoTestWriteOne ${MONGODB_HOST} ${MONGODB_PORT} ${REPETITIONS} ${RESULT_URL}
	sleep 1
	@echo
	@echo
	@echo

runArangoBTests:
	echo ArangoDB /_api/version test "(directly executed in scheduler)"
	@arangob --server.disable-authentication true --test-case version --requests ${REPETITIONS} --quiet --progress false --server.endpoint tcp://${ARANGODB_HOST}:${ARANGODB_PORT} | tee datei
	@./extractTime.sh "arangob version "
	@echo
	@echo
	@echo
	echo ArangoDB single document read test
	@arangob --server.disable-authentication true --test-case document version --requests ${REPETITIONS} --quiet --progress false --server.endpoint tcp://${ARANGODB_HOST}:${ARANGODB_PORT} | tee datei
	@./extractTime.sh "arangob read    "
	@echo
	@echo
	@echo
	echo ArangoDB single document write/read test
	@arangob --server.disable-authentication true --test-case crud-write-read document version --requests ${REPETITIONS} --quiet --progress false --server.endpoint tcp://${ARANGODB_HOST}:${ARANGODB_PORT} | tee datei
	@./extractTime.sh "arangob wr/rd   "
	@echo
	@echo
	@echo

runArangoTests: arangoTestVersion arangoTestDocument arangoTestWriteOne
	@./arangoTestVersion ${ARANGODB_HOST} ${ARANGODB_PORT} ${REPETITIONS} ${RESULT_URL}
	@echo
	@echo
	@echo
	@./arangoTestDocument ${ARANGODB_HOST} ${ARANGODB_PORT} ${REPETITIONS} ${RESULT_URL}
	@echo
	@echo
	@echo
	@./arangoTestWriteOne ${ARANGODB_HOST} ${ARANGODB_PORT} ${REPETITIONS} ${RESULT_URL}
	@echo
	@echo
	@echo

clean:
	rm -rf include lib mongo-cxx-driver mongoTestCount mongoTestFindOne \
	       mongoTestWriteOne arangoTestVersion arangoTestDocument \
	       arangoTestWriteOne core datei arangoTestPrepareCluster \
	       arangoTestReadWriteCluster waiter

