FROM ubuntu:15.10
MAINTAINER Max Neunhoeffer <max@arangodb.com>

# add scripts to install and run ArangoDB
ADD ./scripts /scripts

# add Waiter code
ADD ./waiter /waiter

# add HELP file
ADD ./HELP.md /HELP.md

# install ubuntu package
RUN ./scripts/install.sh

# start script
ENTRYPOINT ["/scripts/runwaiter.sh"]
CMD ["help"]
