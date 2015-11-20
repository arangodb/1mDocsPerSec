#include "utils.h"
#include <mongo/bson/bson.h>
#include <mongo/client/dbclient.h>

int N = 100000;

int main(int argc, char* argv[]) {
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

  std::vector<uint64_t> timings;
  timings.reserve(N);

  std::cout << "MongoDB latency test, calling " << N << " times count()"
            << std::endl;

  mongo::client::initialize();
  mongo::DBClientConnection c;
  std::string serverendpoint(argv[1]);
  serverendpoint.push_back(':');
  serverendpoint.append(argv[2]);
  std::cout << "MongoDB server endpoint: " << serverendpoint << std::endl;
  try {
    c.connect(serverendpoint);
    std::cout << "connected ok" << std::endl;
  } catch( const mongo::DBException &e ) {
    std::cout << "caught " << e.what() << std::endl;
    return 2;
  }

  timePointType t = timeNow();

  uint64_t s = 0;
  timePointType t3, t4;
  for (int i = 0; i < N; i++) {
    t3 = timeNow();
    s += c.count("test.guck");
    t4 = timeNow();
    timings.push_back(timeDiff(t3,t4));
  }

  timePointType t2 = timeNow();
  std::cout << "Total count: " << s << " (should be " << N << "!)" << std::endl;

  uint64_t d = timeDiff(t,t2);
  std::cout << "Total time: " << d << " ns" << std::endl;
  analyzeTimings("mongoTestCount" + std::to_string(getpid()) + ".times",
                 timings, 0);
  dumpTimings("mongoTestCount" + std::to_string(getpid()) + ".times", timings);
  if (! resulturl.empty()) {
    submitTimings(resulturl, "mongoCount", timings, 0);
  }
  return 0;
}
