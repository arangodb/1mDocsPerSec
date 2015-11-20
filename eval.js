function sum(l) { 
  var s = 0,i; 
  for (i = 0; i < l.length; i++) { 
    s += l[i]; 
  }
  return s; 
}

function avg(l) { 
  return sum(l)/l.length;
}

l = [];
clients=80;
from=2
k=3
for (i = from; i <= k; i++) { 
  l.push(clients+"_RD_"+i);
}
for (i = from; i <= k; i++) { 
  l.push(clients+"_WR_"+i);
};
for (i = from ; i <= k; i++) {
  l.push(clients+"_RW_"+i);
}
l

ll = {} 
for (x in l) { 
  ll[l[x]] = {
    read: sum(db._query('FOR r IN results FILTER r.testName == "arangoReadWriteCluster_'+l[x]+'" RETURN r.nrReads').toArray())/30.0, 
    write: sum(db._query('FOR r IN results FILTER r.testName == "arangoReadWriteCluster_'+l[x]+'" RETURN r.nrWrites').toArray())/30.0,
    latency: avg(db._query('FOR r IN results FILTER r.testName == "arangoReadWriteCluster_'+l[x]+'" RETURN r.median').toArray()),
    perc95: avg(db._query('FOR r IN results FILTER r.testName == "arangoReadWriteCluster_'+l[x]+'" RETURN r.perc95').toArray()),
    number: db._query('FOR r IN results FILTER r.testName == "arangoReadWriteCluster_'+l[x]+'" RETURN r.median').toArray().length, 
    errors: sum(db._query('FOR r IN results FILTER r.testName == "arangoReadWriteCluster_'+l[x]+'" RETURN r.nrErrors').toArray()) 
  };
}
ll



//w.insert({"_key":"job", "name":"READ", "HOSTS": "(127.0.0.1)", "TIME": 30, "COMPLEXITY":20, "PORTS":"(8529)", "RESULT_URL":"http://104.155.62.222:8529", "NRSHARDS":8, "multiple":0, "READPERCENT":0, "clients":1, "job":"Bla178" })


