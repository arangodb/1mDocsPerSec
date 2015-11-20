var jobnr = 1;
var o = { _key: "job",
          TIME: 30,
          COMPLEXITY: 20,
          RESULT_URL: "http://104.155.62.222:8529/_api/document?collection=results",
          NRSHARDS: 8,
          clients: 8,
          waiters: 4,
          job: ""+jobnr};

var coordinators = [
  "http://10.0.1.211:1027",
  "http://10.0.3.172:1027",
  "http://10.0.2.28:1027",
  "http://10.0.0.226:1027",
  "http://10.0.2.47:1027",
  "http://10.0.1.208:1027",
  "http://10.0.2.238:1027",
  "http://10.0.2.234:1027"
];

var print = require("internal").print;
var wait = require("internal").wait;
var time = require("internal").time;

var hosts = [];
var ports = [];

for (var c in coordinators) {
  cc = coordinators[c];
  var pos = cc.indexOf("http://");
  if (pos === 0) {
    pos = cc.indexOf(":", 7);
    if (pos !== -1) {
      hosts.push(cc.substr(7,pos-7));
      ports.push(cc.substr(pos+1));
    }
  }
}

hosts = "(" + hosts.join(" ") + ")";
ports = "(" + ports.join(" ") + ")";

print("Hosts: "+hosts)
print("Ports: "+ports)

o.HOSTS = hosts;
o.PORTS = ports;

db = require("internal").db;
work = db.work;
done = db.done;
work.truncate();
done.truncate();

function init () {
  jobnr += 1
  o.job = "JOB"+jobnr
  o.name = "INIT";
  o.multiple = 0;
  o.READPERCENTS = 0;
  work.insert(o);
  print("Submitted init job");
  var t = time();
  while (done.count() !== 1) {
    print("Not finished.");
    wait(5);
    if (time() - t > 300) {
      print("Aborting!");
      break;
    }
  }
  work.truncate();
  print("Result:"+JSON.stringify(done.any()));
  done.truncate();
}

function helper (name, multiple, readpercents) {
  o.name = "" + o.clients + "_" + name + "_" + multiple;
  jobnr += 1
  o.job = "JOB"+jobnr
  print("Doing "+o.name+"...");
  o.multiple = multiple;
  o.READPERCENTS = readpercents;
  work.insert(o);
  print("Submitted job "+o.job);
  var t = time();
  var ok = true;
  while (done.count() < o.waiters) {
    print("Not finished, have " + done.count() + " answers.");
    wait(3);
    if (time() - t > 120) {
      print("Aborting!");
      ok = false;
      break;
    }
  }
  work.truncate();
  print("Result:"+JSON.stringify(done.toArray()));
  print("Have all "+done.count()+" answers.");
  done.truncate();
  return ok;
}

function doit (from, limit) {
  helper("WARMUP", 1, 100);

  for (var i = from; i <= limit; i++) {
    if (! helper("RD", i, 100)) {
      return;
    }
  }

  for (var i = from; i <= limit; i++) {
    if (! helper("WR", i, 0)) {
      return;
    }
  }

  for (var i = from; i <= limit; i++) {
    if (! helper("RW", i, 50)) {
      return;
    }
  }
}

