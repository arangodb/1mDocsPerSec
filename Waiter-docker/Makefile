all:
	(cd .. ; ${MAKE} throughputTest)
	docker build -t neunhoef/waiter .
	docker push neunhoef/waiter
