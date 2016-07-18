1.  Use AWS cloud formation templates with instance types:

 - `i2.2xlarge` for private agents
 - `m3.2xlarge` for public agents

Use 8 private agents and 4 public agents for now.

2.  Open

 - the Mesosphere dash board ( `<master-url>/` )
 - the Mesos dash board ( `<master-url>/mesos` )
 - the Marathon dash board ( `<master-url>/marathon` )

3. Install dcos cli on your laptop (follow instructions on the
Mesosphere dash board).

4. Deploy ArangoDB cluster with:

dcos package install --options=config.json arangodb

with the following file config.json:

    { "arangodb": {
        "async-replication": true,
        "nr-dbservers": 8,
        "nr-coordinators": 8,
        "framework-cpus": 0.5,
        "role": "arangodb",
        "principal": "pri",
        "minimal-resources-agent": "mem(\*):512;cpus(\*):0.5;disk(\*):512",
        "minimal-resources-dbserver": "mem(\*):8192;cpus(\*):3;disk(\*):8192",
        "minimal-resources-secondary": "mem(\*):8192;cpus(\*):1;disk(\*):8192",
        "minimal-resources-coordinator": "mem(\*):8192;cpus(\*):3;disk(\*):8192",
        "secondaries-with-dbservers":true,
        "docker-image": "arangodb/arangodb-mesos:devel"
      }
    }

5. Find out the endpoints (internal IP addresses) of the coordinator
tasks:

    dcos arangodb endpoints

which gives something like

    URL of ArangoDB web frontend:
    http://52.24.224.199/service/arangodb/
    Coordinators running on:
    http://10.0.2.24:1027
    http://10.0.2.25:1027
    http://10.0.3.125:1027
    http://10.0.3.127:1027
    http://10.0.2.23:1028
    http://10.0.2.26:1027
    http://10.0.3.126:1027
    http://10.0.2.27:1027

Edit the list of coordinators in doit.js, the corresponding section must
look like:

    var coordinators = [
      "http://10.0.2.24:1027",
      "http://10.0.2.25:1027",
      "http://10.0.3.125:1027",
      "http://10.0.3.127:1027",
      "http://10.0.2.23:1028",
      "http://10.0.2.26:1027",
      "http://10.0.3.126:1027",
      "http://10.0.2.27:1027"
    ];

6. Deploy load servers

    curl -X POST <master-url>/service/marathon/v2/apps -d @load.json -H "Content-type: application/json"

with file load.json:

    {
      "id": "loadserver",
      "cpus": 7.5,
      "mem": 8192.0,
      "ports": [],
      "requirePorts": false,
      "instances": 4,
      "args": [],
      "env": {},
      "container": {
        "type": "DOCKER",
        "docker": {
          "image": "neunhoef/waiter",
          "network": "HOST",
          "forcePullImage": true
        }
      },
      "acceptedResourceRoles": [
        "slave_public"
      ]
    }

This will deploy 4 load servers to the public slaves.

7. I use a single ArangoDB instance somewhere, whose endpoint is
hard-wired into the `doit.js` script in this repository.

We adjust the number of clients (= number of coordinators), the number
of shards (number of ArangoDB nodes).

8. Attach an arangosh to the single ArangoDB server via

    arangosh --server.endpoint tcp://104.155.62.222:8529

and do

    require("internal").load("doit.js")
    init()
    doit(20, 21)

20 is the minimum number of connections per load server to try to each
coordinator and 21 is the maximum. For example, with 20 (and 4 load
servers and 8 coordinators), each coordinator will get 80=4\*20 incoming
connections and each load server will thus open 160=20\*8 outgoing
connections. The optimal number is 80-100 incoming connections per
coordinator, since we run 60 worker threads.

9. Evaluate the results with this script called `eval.js` in the repository:

    arangosh --server.endpoint tcp://104.155.62.222:8529

and do

    require("internal").load("eval.js")

adjust the clients and from and k numbers.
