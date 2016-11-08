#include <iostream>
#include "Client.hpp"

using namespace std;
using namespace MongoUtils;

int main(int argc, char** argv) {
  string uri_string = "mongodb://localhost";
  if (argc > 1) {
    uri_string = argv[1];
  }
  try {
    Client client{uri_string};
    client.connect();
  } catch (const exception &ex) {
    cout << "Exception: " << ex.what() << endl;
  }
}
