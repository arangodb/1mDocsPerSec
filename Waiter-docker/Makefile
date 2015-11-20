all:
	(cd .. ; ${MAKE} throughputTest)
	cp ../waiter ../arangoTestPrepareCluster ../arangoTestReadWriteCluster ../run.sh waiter
	docker build -t neunhoef/waiter .
	docker push neunhoef/waiter
