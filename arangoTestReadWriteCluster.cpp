#include "utils.h"

uint64_t T = 15;   // seconds of work
int P = 50;   // percentage of reads
int C = 1;    // complexity of documents

char buffer[1024];
size_t pos = 0;

size_t write_data_dummy(void* buf, size_t size, size_t nmemb, void* userp) {
  return size * nmemb;
}

size_t write_data(void* buf, size_t size, size_t nmemb, void* userp) {
  memcpy(buffer+pos, buf, size*nmemb);
  pos += size * nmemb;
  return size * nmemb;
}

int main (int argc, char* argv[]) {
  if (argc < 6) {
    std::cout << "Need HOST, PORT, RUNTIME, READ_PERCENTAGE and COMPLEXITY"
              << "as command line arguments." << std::endl;
    return 1;
  }
  T = atol(argv[3]);
  P = atoi(argv[4]);
  C = atoi(argv[5]);
  std::string resulturl;
  if (argc >= 7) {
    resulturl = argv[6];
  }
  std::string testname;
  if (argc >= 8) {
    testname = argv[7];
  }

  uint64_t errors = 0;

  std::cout << "ArangoDB cluster throughput test, running for " << T 
            << " seconds with " << P << "% reads, complexity " << C 
            << std::endl;

  std::vector<uint64_t> timings;
  timings.reserve(T*100000);

  std::string url = std::string("http://") + argv[1] + ":" + argv[2];
  std::string urlcompleteread, urlcompletewrite;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURL* h = curl_easy_init();
  struct curl_slist* headers=NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");

  std::stringstream body;
  std::string bodycomplete;
  
  int action = 0;

  urlcompleteread = url + "/_api/document/c/";
  urlcompletewrite = url + "/_api/document?collection=c";

  timePointType t3, t4;
  uint64_t d;
  double count = 0.0;
  int docToRead = 0;
  uint64_t nrReads = 0;

  std::stringstream bodyBase;
  for (int j = 1; j <= C; j++) {
    bodyBase << "\"value" << j << "\":\"abcdefghijklmnopqrstuvwxyz\",";
  }
  std::string bodyBaseComplete = bodyBase.str();
  timePointType t = timeNow();
  do {
    pos = 0;
    t3 = timeNow();
    if (action >= P) {
      // Do a write
      body.str("");
      body << "{";
      body << bodyBaseComplete;
      body << "\"count\": " << count << "}";
      bodycomplete = body.str();
      count += 1.0;
      curl_easy_setopt(h, CURLOPT_URL, urlcompletewrite.c_str());
      curl_easy_setopt(h, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(h, CURLOPT_POSTFIELDS, bodycomplete.c_str());
      curl_easy_setopt(h, CURLOPT_POSTFIELDSIZE, bodycomplete.size());
      curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_data_dummy);
      // curl_easy_setopt(h, CURLOPT_TCP_NODELAY, 1);
      // Leave this off for now, since we are after throughput, not latency.
      int success = curl_easy_perform(h);
      if (success != 0) {
        std::cout << "Error from libcurl: " << success << std::endl;
        errors++;
      }
    }
    else {
      urlcompleteread = url + "/_api/document/c/K" + std::to_string(docToRead);
      docToRead += 1447;
      if (docToRead >= 100000) {
        docToRead -= 100000;
      }
      curl_easy_setopt(h, CURLOPT_HTTPGET, 1);
      curl_easy_setopt(h, CURLOPT_URL, urlcompleteread.c_str());
      curl_easy_setopt(h, CURLOPT_HTTPHEADER, NULL);
      curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_data);
      // curl_easy_setopt(h, CURLOPT_TCP_NODELAY, 1);
      // Leave this off for now, since we are after throughput, not latency.
      pos = 0;
      int success = curl_easy_perform(h);
      if (success != 0) {
        std::cout << "Error from libcurl: " << success << std::endl;
        errors++;
      }
      buffer[pos] = 0;
      nrReads++;
    }
    t4 = timeNow();
    timings.push_back(timeDiff(t3,t4));
    action += 47; if (action >= 100) {
      action -= 100;
    }
    d = timeDiff(t,t4);
  } while (d < T * 1000000000);
  timePointType t2 = timeNow();
  curl_slist_free_all(headers);
  curl_easy_cleanup(h);
  d = timeDiff(t,t2);
  std::cout << "Total time: " << d << " ns" << std::endl;
  analyzeTimings("arangoTestReadWriteCluster" + std::to_string(getpid())
                 + ".times", timings, nrReads);
  dumpTimings("arangoTestReadWriteCluster" + std::to_string(getpid()) 
              + ".times", timings);
  if (! resulturl.empty()) {
    submitTimings(resulturl, "arangoReadWriteCluster_"+testname, timings, nrReads, errors);
  }
  return 0;
}

