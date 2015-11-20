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

  std::cout << "ArangoDB latency test, calling " << N << " times /_api/version"
            << std::endl;

  std::vector<uint64_t> timings;
  timings.reserve(N);

  std::string url = std::string("http://") + argv[1] + ":" + argv[2] 
                    + "/_api/version";

  timePointType t = timeNow();
  curl_global_init(CURL_GLOBAL_DEFAULT);
  auto h = curl_easy_init();
  curl_easy_setopt(h, CURLOPT_URL, url.c_str());
  curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(h, CURLOPT_TCP_NODELAY, 1);
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
  analyzeTimings("arangoTestVersion" + std::to_string(getpid()) + ".times",
                 timings, 0);
  dumpTimings("arangoTestVersion" + std::to_string(getpid()) + ".times",
              timings);
  if (! resulturl.empty()) {
    submitTimings(resulturl, "arangoApiVersion", timings, 0);
  }
  return 0;
}

