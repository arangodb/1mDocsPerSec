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

```json
{
    "arangodb": {
        "async-replication": true,
        "nr-dbservers": 8,
        "nr-coordinators": 8,
        "nr-agents": 1,
        "framework-cpus": 0.5,
        "role": "arangodb",
        "principal": "pri",
        "minimal-resources-agent": "mem(*):1024;cpus(*):0.5;disk(*):1024",
        "minimal-resources-dbserver": "mem(*):8192;cpus(*):3;disk(*):8192",
        "minimal-resources-secondary": "mem(*):8192;cpus(*):1;disk(*):8192",
        "minimal-resources-coordinator": "mem(*):8192;cpus(*):3;disk(*):8192",
        "secondaries-with-dbservers": true
    }
}
```

5. Deploy load servers

    dcos marathon app add load.json

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

6. Then you need to create a separate single server instance of arangodb somewhere. This will control the test for us

The easiest is probably to startup arangodb via docker like this

    docker run -d -p 8529:8529 -e ARANGO_NO_AUTH=1 --name arangodb arangodb/arangodb

7. Attach an arangosh to the single ArangoDB server from your local computer

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

8. Evaluate the results with this script called `eval.js` in the repository:

    arangosh --server.endpoint tcp://104.155.62.222:8529

and do

    require("internal").load("eval.js")

adjust the clients and from and k numbers.
