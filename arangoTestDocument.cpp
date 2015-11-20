#include "utils.h"

int N = 100000;

char buffer[1024];
size_t pos = 0;

size_t write_data(void* buf, size_t size, size_t nmemb, void* userp) {
  memcpy(buffer+pos, buf, size*nmemb);
  pos += size * nmemb;
  return size * nmemb;
}

int main (int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "Need HOST and PORT as command line arguments." << std::endl;
    return 1;
  }
  if (argc >= 4) {
    N = atoi(argv[3]);
  }
  std::string resulturl;
  if (argc >= 5) {
    resulturl = argv[4];
  }

  std::cout << "ArangoDB latency test, calling " << N 
            << " times GET /_api/document/c/mykey" << std::endl;

  std::vector<uint64_t> timings;
  timings.reserve(N);

  std::string url = std::string("http://") + argv[1] + ":" + argv[2];
  std::string urlcomplete;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURL* h = curl_easy_init();

  // Delete the collection c:
  std::cout << "Dropping collection c..." << std::endl;
  curl_easy_setopt(h, CURLOPT_CUSTOMREQUEST, "DELETE");
  urlcomplete = url + "/_api/collection/c";
  curl_easy_setopt(h, CURLOPT_URL, urlcomplete.c_str());
  curl_easy_perform(h);
  curl_easy_setopt(h, CURLOPT_CUSTOMREQUEST, NULL);
  std::cout << "\ndone." << std::endl;

  std::cout << "Creating collection c..." << std::endl;
  strcpy(buffer, "{\"name\": \"c\", \"numberOfShards\":13}");
  urlcomplete = url + "/_api/collection";
  curl_easy_setopt(h, CURLOPT_URL, urlcomplete.c_str());
  struct curl_slist *headers=NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(h, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(h, CURLOPT_POSTFIELDS, buffer);
  curl_easy_setopt(h, CURLOPT_POSTFIELDSIZE, strlen(buffer));
  curl_easy_perform(h);
  curl_slist_free_all(headers);
  headers = NULL;
  std::cout << "\ndone." << std::endl;

  std::cout << "Creating one document..." << std::endl;
  strcpy(buffer, "{\"_key\":\"mykey\", \"name\":\"Neunhöffer\", \"firstName\":\"Max\"}");
  urlcomplete = url + "/_api/document?collection=c";
  curl_easy_setopt(h, CURLOPT_URL, urlcomplete.c_str());
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(h, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(h, CURLOPT_POSTFIELDS, buffer);
  curl_easy_setopt(h, CURLOPT_POSTFIELDSIZE, strlen(buffer));
  curl_easy_perform(h);
  curl_slist_free_all(headers);
  std::cout << "\ndone." << std::endl;

  curl_easy_setopt(h, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(h, CURLOPT_HTTPHEADER, NULL);
  curl_easy_setopt(h, CURLOPT_TCP_NODELAY, 1);

  std::cout << "Now racing..." << std::endl;
  timePointType t = timeNow();
  urlcomplete = url + "/_api/document/c/mykey";
  curl_easy_setopt(h, CURLOPT_URL, urlcomplete.c_str());
  curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_data);
  timePointType t3, t4;
  for (int i = 0; i < N; i++) {
    pos = 0;
    t3 = timeNow();
    int success = curl_easy_perform(h);
    t4 = timeNow();
    timings.push_back(timeDiff(t3,t4));
    buffer[pos] = 0;
  }
  curl_easy_cleanup(h);
  timePointType t2 = timeNow();
  uint64_t d = timeDiff(t,t2);
  std::cout << "Total time: " << d << " ns" << std::endl;
  analyzeTimings("arangoTestDocument" + std::to_string(getpid()) 
                 + ".times", timings, N);
  dumpTimings("arangoTestDocument" + std::to_string(getpid()) 
              + ".times", timings);
  if (! resulturl.empty()) {
    submitTimings(resulturl, "readDocument", timings, N-1);
  }
  return 0;
}

