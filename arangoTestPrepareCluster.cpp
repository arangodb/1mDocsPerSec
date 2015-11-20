#include "utils.h"
#include <curl/curl.h>

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
  if (argc < 5) {
    std::cout << "Need HOST, PORT, NRSHARDS and COMPLEXITY as command "
              << "line arguments." << std::endl;
    return 1;
  }
  std::string nrShards = argv[3];
  C = atoi(argv[4]);
  std::cout << "ArangoDB cluster load, preparing collection..." << std::endl;

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
  std::stringstream body;
  std::string bodycomplete;
  body << "{";
  body << "\"name\": \"c\", \"numberOfShards\": " << nrShards << "}";
  bodycomplete = body.str();
  urlcomplete = url + "/_api/collection";
  curl_easy_setopt(h, CURLOPT_URL, urlcomplete.c_str());
  struct curl_slist* headers=NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(h, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(h, CURLOPT_POSTFIELDS, bodycomplete.c_str());
  curl_easy_setopt(h, CURLOPT_POSTFIELDSIZE, bodycomplete.size());
  curl_easy_perform(h);
  curl_slist_free_all(headers);
  headers = NULL;
  std::cout << "\ndone." << std::endl;

  std::stringstream bodyBase;
  for (int j = 1; j <= C; j++) {
    bodyBase << "\"value" << j << "\":\"abcdefghijklmnopqrstuvwxyz\",";
  }
  std::string bodyBaseComplete = bodyBase.str();

  headers = curl_slist_append(headers, "Content-Type: application/json");
  urlcomplete = url + "/_api/document?collection=c";

  curl_easy_setopt(h, CURLOPT_URL, urlcomplete.c_str());
  curl_easy_setopt(h, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, write_data_dummy);
  std::cout << "Creating 100000 documents..." << std::endl;
  for (int i = 0; i < 100000;i++) {
    body.str("");  // set to empty
    body << "{\"_key\": \"K" << std::to_string(i) << "\",";
    body << bodyBaseComplete;
    body << "\"value\":" << i << "}";
    bodycomplete = body.str();
    curl_easy_setopt(h, CURLOPT_POSTFIELDS, bodycomplete.c_str());
    curl_easy_setopt(h, CURLOPT_POSTFIELDSIZE, bodycomplete.size());
    curl_easy_perform(h);
  }
  std::cout << "\ndone." << std::endl;

  curl_slist_free_all(headers);
  headers = NULL;
  curl_easy_cleanup(h);
  return 0;
}

