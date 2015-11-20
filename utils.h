#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <time.h>
#include <vector>
#include <stdint.h>
#include <chrono>
#include <algorithm>

#include <unistd.h>

#include <curl/curl.h>

typedef std::chrono::high_resolution_clock::time_point timePointType;
typedef std::chrono::duration<uint64_t, std::nano>     nanoSecondsType;

static inline timePointType timeNow () {
  return std::chrono::high_resolution_clock::now();
}

static inline uint64_t timeDiff (timePointType& a, timePointType& b) {
  return std::chrono::duration_cast<nanoSecondsType>(b-a).count();
}

void dumpTimings (std::string filename, std::vector<uint64_t>& timings) {
  std::fstream file(filename.c_str(), std::ios_base::out | std::ios_base::app);
  for (auto t : timings) {
    file << t << "\n";
  }
}

void analyzeTimings (std::string filename, std::vector<uint64_t>& timings,
                     uint64_t nrReads) {
  // We ignore the first, sort and compute minimum, maximum, average, median,
  // 95% percentile and 99% percentile.
  std::fstream file(filename.c_str(), std::ios_base::out | std::ios_base::app);
  std::vector<uint64_t> copy;
  copy.reserve(timings.size()-1);
  copy.insert(copy.end(), timings.begin()+1, timings.end());
  std::sort(copy.begin(), copy.end());
  uint64_t sum = 0;
  for (auto c : copy) {
    sum += c;
  }
  size_t nr = copy.size();
  file << "  Number of samples:   " << nr << "  (first ignored)" 
                                                            << std::endl
       << "  Number of reads:     " << nrReads              << std::endl
       << "  First [us]:          " << timings[0] / 1000.0  << std::endl
       << "  Minimum [us]:        " << copy[0] / 1000.0     << std::endl
       << "  Maximum [us]:        " << copy.back() / 1000.0 << std::endl
       << "  Average [us]:        " << (sum / 1000.0) / nr  << std::endl
       << "  Median [us]:         " << copy[nr/2] / 1000.0  << std::endl
       << "  95% percentile [us]: " << copy[static_cast<size_t>(nr*0.95)]
                                       / 1000.0             << std::endl
       << "  99% percentile [us]: " << copy[static_cast<size_t>(nr*0.99)]
                                       / 1000.0             << std::endl;
}

size_t ignore_data(void* buf, size_t size, size_t nmemb, void* userp) {
  return size * nmemb;
}

void submitTimings (std::string url, std::string testName,
                    std::vector<uint64_t>& timings, uint64_t nrReads,
                    uint64_t nrErrors = 0) {
  // We ignore the first, sort and compute minimum, maximum, average, median,
  // 95% percentile and 99% percentile.
  std::cout << "Submitting statistics to database..." << std::endl;
  std::stringstream json;
  std::vector<uint64_t> copy;
  copy.reserve(timings.size()-1);
  copy.insert(copy.end(), timings.begin()+1, timings.end());
  std::sort(copy.begin(), copy.end());
  uint64_t sum = 0;
  for (auto c : copy) {
    sum += c;
  }
  size_t nr = copy.size();
  uint64_t nrWrites = nr < nrReads ? 0 : nr - nrReads;
  json << "{ \"testName\":\"" << testName << "\","          << std::endl
       << "  \"nrSamples\":" << nr << ","                   << std::endl
       << "  \"nrReads\":" << nrReads << ","                << std::endl
       << "  \"nrWrites\":" << nrWrites << ","              << std::endl
       << "  \"first\":" << timings[0] / 1000.0  << ","     << std::endl
       << "  \"minimum\":" << copy[0] / 1000.0 << ","       << std::endl
       << "  \"maximum\":" << copy.back() / 1000.0 << ","   << std::endl
       << "  \"average\":" << (sum / 1000.0) / nr  << ","   << std::endl
       << "  \"median\":" << copy[nr/2] / 1000.0  << ","    << std::endl
       << "  \"perc95\":" << copy[static_cast<size_t>(nr*0.95)] / 1000.0 << ","
                                                            << std::endl
       << "  \"perc99\":" << copy[static_cast<size_t>(nr*0.99)] / 1000.0 << ","
                                                            << std::endl
       << "  \"nrErrors\":" << nrErrors                     << std::endl
       << "}"                                               << std::endl;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  CURL* h = curl_easy_init();
  curl_easy_setopt(h, CURLOPT_URL, url.c_str());
  struct curl_slist *headers=NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  std::string body = json.str();
  curl_easy_setopt(h, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(h, CURLOPT_POSTFIELDS, body.c_str());
  curl_easy_setopt(h, CURLOPT_POSTFIELDSIZE, body.size());
  curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, ignore_data);
  curl_easy_perform(h);
  curl_slist_free_all(headers);
  curl_easy_cleanup(h);
}

class MyCurl {

    CURL* _h;
    struct curl_slist* _headers;
    std::string _buffer;
    int _res;

  public:

    MyCurl () {
      _h = curl_easy_init();
      _headers = nullptr;
    }

    ~MyCurl () {
      if (_headers != nullptr) {
        curl_slist_free_all(_headers);
      }
      _headers = nullptr;
      curl_easy_cleanup(_h);
    }

    void clearHeaders () {
      curl_slist_free_all(_headers);
      _headers = nullptr;
    }

    void addHeader(std::string h) {
      _headers = curl_slist_append(_headers, h.c_str());
    }

    std::string&& get (std::string url) {
      std::string result;
      curl_easy_setopt(_h, CURLOPT_HTTPGET, 1);
      curl_easy_setopt(_h, CURLOPT_URL, url.c_str());
      curl_easy_setopt(_h, CURLOPT_HTTPHEADER, nullptr);
      curl_easy_setopt(_h, CURLOPT_WRITEFUNCTION, &MyCurl::write_data);
      curl_easy_setopt(_h, CURLOPT_WRITEDATA, static_cast<void*>(this));
      _buffer.clear();
      _res = curl_easy_perform(_h);
      if (_res != 0) {
        _buffer.clear();
      }
      return std::move(_buffer);
    }

    std::string&& post (std::string url, std::string& body) {
      std::string result;
      curl_easy_setopt(_h, CURLOPT_HTTPGET, 1);
      curl_easy_setopt(_h, CURLOPT_URL, url.c_str());
      curl_easy_setopt(_h, CURLOPT_HTTPHEADER, nullptr);
      curl_easy_setopt(_h, CURLOPT_POSTFIELDS, body.c_str());
      curl_easy_setopt(_h, CURLOPT_POSTFIELDSIZE, body.size());
      curl_easy_setopt(_h, CURLOPT_WRITEFUNCTION, &MyCurl::write_data);
      curl_easy_setopt(_h, CURLOPT_WRITEDATA, static_cast<void*>(this));
      _buffer.clear();
      _res = curl_easy_perform(_h);
      if (_res != 0) {
        _buffer.clear();
      }
      return std::move(_buffer);
    }

    int getCurlResult () {
      return _res;
    }

    int getHttpCode () {
      long code;
      curl_easy_getinfo(_h, CURLINFO_RESPONSE_CODE, &code);
      return static_cast<int>(code);
    }

    CURL* getCurlHandle () {
      return _h;
    }

    static size_t write_data(void* buf, size_t size, size_t nmemb, 
                             void* userp) {
      MyCurl* c = static_cast<MyCurl*>(userp);
      c->_buffer.append(static_cast<char*>(buf), size*nmemb);
      return size*nmemb;
    }
};

