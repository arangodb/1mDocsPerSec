#include "utils.h"
#include <map>
#include <memory>
#include <exception>
#include "picojson.h"

using namespace std;

bool work (MyCurl& curl, string const& urlbase, string resultbody, 
           string& lastJob) {
  picojson::value json;
  string error = picojson::parse(json, resultbody);
  if (error.empty()) {
    if (! json.is<picojson::object>()) {
      cout << "No document!" << endl;
    }
    else {
      picojson::value::object& jsonobj = json.get<picojson::object>();
      if (jsonobj["end"].is<bool>() && jsonobj["end"].get<bool>()) {
        return true;
      }
      if (! jsonobj["job"].is<string>()) {
        cout << "Illegal value of \"job\" attribute." << endl;
        return false;
      }
      string job = jsonobj["job"].get<string>();
      if (job == lastJob) {
        cout << "Job already done." << endl;
        return false;
      }
      lastJob = job;
      cout << "Got:" << json << endl;
      fstream file("coordinatorlist.sh", ios::out | ios::trunc);
      file << "export HOSTS=" << jsonobj["HOSTS"].get<string>() << "\n"
           << "export PORTS=" << jsonobj["PORTS"].get<string>() << "\n"
           << "export TIME=" << jsonobj["TIME"] << "\n"
           << "export READPERCENTS=" << jsonobj["READPERCENTS"] << "\n"
           << "export COMPLEXITY=" << jsonobj["COMPLEXITY"] << "\n"
           << "export RESULT_URL=\"" << jsonobj["RESULT_URL"].get<string>() << "\"\n"
           << "export NRSHARDS=" << jsonobj["NRSHARDS"] << "\n";
      file.close();
      int nrclients = static_cast<int>(jsonobj["clients"].get<double>());
      int multiple = static_cast<int>(jsonobj["multiple"].get<double>());
      string command = "./run.sh";
      if (multiple == 0) {  // If multiple == 0, then this is an init
        // In this case, we have to race for action:
        cout << "Racing to do the initialisation..." << endl;
        string url = urlbase + "/_api/document?collection=work";
        string b = "{\"_key\": \"race\"}";
        string r = curl.post(url, b);
        cout << "Curl result: " << curl.getCurlResult()
             << "HTTP result code: " << curl.getHttpCode() << endl;
        cout << "Body of the result: " << r << endl;
        if (curl.getCurlResult() != 0 || curl.getHttpCode() != 202) {
          cout << "Lost the race, skipping job." << endl;
          return false;
        }
        cout << "Won the race, running ./run.sh ..." << endl;
      }
      else {
        command += " " + jsonobj["name"].get<string>();
      }
      for (int i = 0; i < multiple; i++) {
        for (int j = 0; j < nrclients; j++) {
          command += " " + to_string(j);
        }
      }
      cout << "Doing:\n" << command << endl;
      timePointType t = timeNow();
      int res = system(command.c_str());
      timePointType t2 = timeNow();
      uint64_t duration = timeDiff(t, t2);
      // Finally, report back:
      stringstream body;
      string bodycomplete;
      body << "{ \"result\": " << res << ", \"duration\": " << duration << "}";
      bodycomplete = body.str();
      string url = urlbase + "/_api/document?collection=done";
      resultbody = curl.post(url, bodycomplete);
      int success = curl.getCurlResult();
      if (success != 0) {
        cout << "Error from libcurl: " << success << endl;
        cout << "So we could not report back!" << endl;
      }
      else {
        int code = curl.getHttpCode();
        if (code != 202) {
          cout << "Got HTTP code " << code << endl;
          cout << "So we could not report back!" << endl;
        }
        else {
          // Now the body is a JSON, just dump it out:
          cout << "Got:\n" << resultbody << endl;
        }
      }
    }
  }
  return false;
}

int main (int argc, char* argv[]) {
  cout << "Hello, I am a waiter, I usually wait..." << endl;
  if (argc < 2) {
    cout << "Need URL for contact with ArangoDB "
              << "as command line arguments." << endl;
    return 1;
  }
  string urlbase = argv[1];
  while (urlbase.back() == '/') {
    urlbase.pop_back();
  }

  MyCurl curl;
  string lastjob = "";

  //curl.addheader("Content-Type: application/json");

  while (true) {   // will be left by break
    usleep(500000);  // Wait half a second
    string url = urlbase + "/_api/document/work/job";
    string resultbody = curl.get(url);
    int success = curl.getCurlResult();
    if (success != 0) {
      cout << "Error from libcurl: " << success << endl;
      cout << "So there is no job to be executed!" << endl;
    }
    else {
      int code = curl.getHttpCode();
      if (code != 200) {
        cout << "Got HTTP code " << code
             << " - so there is no job to be executed!" << endl;
      }
      else {
        if (work(curl, urlbase, resultbody, lastjob)) {
          break;
        }
      }
    }
  }
  return 0;
}

